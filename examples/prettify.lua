local M = {}

function M.prettify(value, indentation)
	indentation = indentation or ""
	-- non table value
	if type(value) ~= "table" then
		return ("%s%s"):format(indentation, tostring(value))
	end

	-- empty table
	if not next(value) then
		return ("%s{}"):format(indentation)
	end

	local s = ""
	local keys = {}
	for k,v in pairs(value) do
		keys[#keys + 1] = k
	end
	table.sort(keys)
	for i=1,#keys do
		local k = keys[i]
		local v = value[k]
		local last = (i == #keys) and "" or ",\n"
		local t = type(v)
		if t == "table" then
			s = s .. ("%s%s = {\n%s\n%s}%s"):format(indentation, k, M.prettify(v, indentation .. "  "), indentation, last)
		elseif t == "string" then
			s = s .. ("%s%s = '%s'%s"):format(indentation, k, v, last)
		else
			s = s .. ("%s%s = %s%s"):format(indentation, k, tostring(v), last)
		end
	end
	return s
end


return setmetatable(M, {
	__call = function(t, ...)
		return M.prettify(...)
	end
})