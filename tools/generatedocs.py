#!/usr/bin/env python

import os
import sys
import shutil
import json
import re
import codecs
import fnmatch
import pystache

from optparse import OptionParser

def capitalize(s):
    if len(s) == 0: return s
    return s[0].upper() + s[1:]

def is_cpp(filetype):
    return filetype in ["cpp", "c", "h"]

def is_lua(filetype):
    return filetype in ["lua", "script", "gui_script", "render_script"]

def render(data, templatefile, outfile):
    with open(templatefile, 'r') as f:
        template = f.read()
        result = pystache.render(template, data)
        with codecs.open(outfile, "wb", encoding="utf-8") as f:
            f.write(result)
            # f.write(html.unescape(result))


def find_files(root_dir, file_pattern):
    matches = []
    for root, dirnames, filenames in os.walk(root_dir):
        for filename in filenames:
            fullname = os.path.join(root, filename)
            if fnmatch.fnmatch(filename, file_pattern):
                matches.append(os.path.join(root, filename))
    matches.sort()
    return matches

# M.foobar = function
# function M.foobar
# local function foobar
# M.foobar = 
# local foobar =
def get_lua_field_name(line):
    m = re.match(r"(\w*)\.(\w*) = function.*", line)
    if m:
        return m.groups()[1]
    m = re.match(r"function (\w*)\.(\w*)", line)
    if m:
        return m.groups()[1]
    m = re.match(r"(\w*)\.(\w*) = .*", line)
    if m:
        return m.groups()[1]
    m = re.match(r"local function (\w*)", line)
    if m:
        return m.groups()[0]
    m = re.match(r"local (\w*) = .*", line)
    if m:
        return m.groups()[0]
    return None

# int SteamUserStats_RequestCurrentStats(lua_State* L) {
def get_cpp_field_name(line):
    m = re.match(r".*?\s+(.*?)\(.*\)", line)
    if m:
        return m.groups()[0]
    return None

def get_field_name(line, filetype):
    if is_cpp(filetype):
        return get_cpp_field_name(line)
    elif is_lua(filetype):
        return get_lua_field_name(line)
    else:
        return None

def parse_field(line):
    line = line.replace("@field", "").strip()
    parts = line.split(" ")
    field_name = parts[0]
    field_type = parts[1] if len(parts) >= 2 else ""
    field_desc = line.removeprefix(field_name).strip().removeprefix(field_type).strip()
    return {
        "name": field_name,
        "description": capitalize(field_desc),
        "type": field_type
    }

def parse_param(line, param, original = None):
    line = line.replace("@" + (param if not original else original) + " ", "")
    param_name = line.split(' ', 1)[0]
    param_desc = line.removeprefix(param_name).strip()
    return {
        "name": param_name,
        "description": capitalize(param_desc),
        "type": None if param == "param" else param
    }

def entry_start_tokens(filetype):
    if is_lua(filetype):
        return [ r"^\-\-\- (.*)" ]
    elif is_cpp(filetype):
        return [ r"^\/\*\* (.*)", r"^\/\*\*$" ]
    else:
        return None

def entry_line_token(filetype):
    if is_lua(filetype):
        return "--"
    elif is_cpp(filetype):
        return "*"
    else:
        return None

def entry_end_token(filetype):
    if is_lua(filetype):
        return ""
    elif is_cpp(filetype):
        return "*/"
    else:
        return None

def pop_line(lines, filetype):
    line_token = entry_line_token(filetype)
    end_token = entry_end_token(filetype)
    line = lines.pop(0).strip()
    # end of line through an end token
    if len(end_token) > 0 and line.startswith(end_token):
        return None
    # end of line by no longer finding a line token
    if not line.startswith(line_token):
        lines.insert(0, line)
        return None
    return line.removeprefix(line_token).strip()

def process_entry(line, lines, filetype):
    entry = {}
    entry["summary"] = capitalize(line.strip())
    entry["description"] = None
    entry["usage"] = None
    entry["params"] = []
    entry["returns"] = []
    while len(lines) > 0:
        line = pop_line(lines, filetype)

        if line is None:
            # try to figure out name of entry (unless it has been explicitly set)
            if "name" not in entry:
                entry["name"] = get_field_name(lines[0].strip(), filetype)
            break

        # generic parameter
        if line.startswith("@param"):
            entry["params"].append(parse_param(line, "param"))
        # typed parameter
        elif line.startswith("@number"):
            entry["params"].append(parse_param(line, "number"))
        elif line.startswith("@string"):
            entry["params"].append(parse_param(line, "string"))
        elif line.startswith("@table"):
            param = parse_param(line, "table")
            param["fields"] = []
            while len(lines) > 0:
                next_line = pop_line(lines, filetype)
                if not next_line or not next_line.startswith("@field"):
                    break
                param["fields"].append(parse_field(next_line))
            entry["params"].append(param)
        elif line.startswith("@function"):
            entry["params"].append(parse_param(line, "function"))
        elif line.startswith("@boolean"):
            entry["params"].append(parse_param(line, "boolean"))
        elif line.startswith("@bool"):
            entry["params"].append(parse_param(line, "boolean", original = "bool"))
        elif line.startswith("@vec2"):
            entry["params"].append(parse_param(line, "vec2"))
        elif line.startswith("@vec3"):
            entry["params"].append(parse_param(line, "vec3"))
        elif line.startswith("@vec4"):
            entry["params"].append(parse_param(line, "vec4"))

        # generic return
        elif line.startswith("@return"):
            line = line.replace("@return", "").strip()
            m = re.match(r"(\w*?) (.*)", line)
            if m:
                return_name = m.groups()[0]
                return_desc = m.groups()[1]
                entry["returns"].append({
                    "name": return_name,
                    "description": capitalize(return_desc)
                })
        # typed return
        elif line.startswith("@treturn"):
            line = line.replace("@treturn", "").strip()
            m = re.match(r"(\w+)\s*(\w+)\s*(.*)", line)
            if m:
                return_type = m.groups()[0]
                return_name = m.groups()[1]
                return_desc = m.groups()[2]
                entry["returns"].append({
                    "name": return_name,
                    "type": return_type,
                    "description": capitalize(return_desc)
                })
        # typed parameter
        elif line.startswith("@tparam"):
            line = line.replace("@tparam", "").strip()
            m = re.match(r"(\w+)\s*(\w+)\s*(.*)", line)
            if m:
                param_type = m.groups()[0]
                param_name = m.groups()[1]
                param_desc = m.groups()[2]
                entry["params"].append({
                    "name": param_name,
                    "type": param_type,
                    "description": capitalize(param_desc)
                })
        elif line.startswith("@field"):
            field = parse_field(line)
            entry["field"] = True
            if not "name" in entry:
                entry["name"] = field.get("name")
            entry["field_type"] = field.get("type", None)
        elif line.startswith("@document"):
            entry["document"] = True
        elif line.startswith("@language"):
            line = line.replace("@language", "").strip()
            entry["language"] = line
        elif line.startswith("@namespace"):
            line = line.replace("@namespace", "").strip()
            entry["namespace"] = line
        elif line.startswith("@name"):
            line = line.replace("@name", "").strip()
            entry["name"] = line
        elif line.startswith("@usage"):
            entry["usage"] = line.replace("@usage", "")
        elif line.startswith("@"):
            m = re.match(r"\@(\w*?) (.*)", line)
            if m:
                tag = m.groups()[0]
                text = m.groups()[1]
                entry[tag] = text
            else:
                print("Found unknown tag: " + line)
        else:
            if entry.get("usage") is not None:
                entry["usage"] = entry["usage"] + line + "\n"
            else:
                if entry.get("description") is None:
                    entry["description"] = ""
                entry["description"] = entry["description"] + line + " "

    has_params = len(entry["params"]) > 0
    if has_params:
        params = []
        for p in entry["params"]:
            params.append(p["name"])
        entry["params_string"] = ",".join(params)

    entry["has_params"] = has_params
    entry["has_returns"] = len(entry["returns"]) > 0
    if entry.get("description") is not None:
        entry["description"] = capitalize(entry["description"].strip())
    return entry

def check_and_process_entry(line, lines, filetype):
    start_tokens = entry_start_tokens(filetype)
    for start_token in start_tokens:
        m = re.match(start_token, line)
        if m:
            line = "" if len(m.groups()) == 0 else m.groups()[0]
            return process_entry(line, lines, filetype)
    return None


def process_lines(lines, filename, filetype):
    print("Processing {}".format(filename))
    entries = []
    while len(lines) > 0:
        line = lines.pop(0).lstrip()
        entry = check_and_process_entry(line, lines, filetype)
        if entry:
            entries.append(entry)
            print("  Found", entry["name"])
    return entries


def generate(directory):
    namespaces_lookup = {}

    for filetype in ["lua", "cpp"]:

        pattern = "*." + filetype
        filenames = find_files(directory, pattern)
        for filename in filenames:
            # if not filename.endswith("steam_apps.cpp"):
            #     continue
            with open(filename, encoding='utf-8', errors='ignore') as f:
                lines = f.readlines()
                file_description = None
                first_line = lines.pop(0).strip()
                file_description = check_and_process_entry(first_line, lines, filetype)
                if not file_description:
                    file_description = {}
                
                file_entries = []
                for file_entry in process_lines(lines, filename, filetype):
                    if "document" in file_entry:
                        if "namespace" not in file_entry:
                            file_entry["namespace"] = os.path.basename(filename).replace("." + filetype, "")
                        file_description["namespace"] = file_entry["namespace"]
                        file_description["summary"] = file_entry["summary"]
                        file_description["description"] = file_entry["description"]
                    else:
                        file_entries.append(file_entry)

                if len(file_entries) > 0:
                    if "namespace" not in file_description:
                        file_description["namespace"] = os.path.basename(filename).replace("." + filetype, "")
                    if "module" not in file_description:
                        file_description["module"] = os.path.basename(filename).replace("." + filetype, "")
                    file_description["entries"] = file_entries
                    file_description["filename"] = filename

                    namespace = file_description["namespace"]
                    if namespace not in namespaces_lookup:
                        namespaces_lookup[namespace] = {
                            "namespace": namespace,
                            "files": [],
                            "description": "",
                        }
                    namespaces_lookup[namespace]["files"].append(file_description)
                    if file_description["description"]:
                        namespaces_lookup[namespace]["description"] = namespaces_lookup[namespace]["description"] + file_description["description"]

    namespaces = []
    for namespace in namespaces_lookup.keys():
        namespaces.append(namespaces_lookup.get(namespace))

    return namespaces


def run():
    script_dir = os.path.dirname(__file__)
    extension_dir = sys.argv[1]
    build_dir = os.path.abspath("build")

    namespaces = generate(os.path.abspath(extension_dir))

    api = { "namespaces": namespaces }
    api_path = os.path.join(build_dir, "api.json")
    print("Writing api %s" % api_path)
    with open(api_path, "w", encoding='utf-8') as f:
        json.dump(api, f, indent=4, sort_keys=True)

    render(api, os.path.join(script_dir, "api_markdown.mtl"), os.path.join(build_dir, "api.md"))
    render(api, os.path.join(script_dir, "api_script.mtl"), os.path.join(extension_dir, "api", extension_dir + ".script_api"))


if __name__ == '__main__':
    run()