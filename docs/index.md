---
title: Defold Steam extension API documentation
brief: This manual covers how to integrate a game with the Steamworks SDK.
---

# Defold Steam extension API documentation

This extension provides an interface to integrate a game with the Steamworks SDK.


## Installation

To use Steam in your Defold project, add a version of the Steam extension to your `game.project` dependencies from the list of available [Releases](https://github.com/defold/extension-steam/releases). Find the version you want, copy the URL to ZIP archive of the release and add it to the project dependencies.

![](add-dependency.png)

Select `Project->Fetch Libraries` once you have added the version to `game.project` to download the version and make it available in your project.

### macOS

To be able to build and run from the editor you need to copy `steam/lib/osx/*.dylib` to `/usr/local/lib`.


## Usage

```lua

local function on_steam_event(self, event, data)
	if event == "GameOverlayActivated_t" then
		print("Overlay is", data.m_bActive)
	end
end

function init(self)
	-- initialize the Steamworks SDK
	local status, error = steam.init()
	if not status then
		print("Error: " .. error)
		return
	end

	steam.set_listener(on_steam_event)
end

function update(self, dt)
	steam.update()
end

```


## Example

[Refer to the example project](https://github.com/defold/extension-steam/blob/master/examples) to see a complete example of how the integration works.


## Source code

The source code is available on [GitHub](https://github.com/defold/extension-steam)
