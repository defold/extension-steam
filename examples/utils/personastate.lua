local M = {}

PERSONASTATES = {
	[steamworks.PERSONA_STATE_OFFLINE] = "offline",
	[steamworks.PERSONA_STATE_ONLINE] = "online",
	[steamworks.PERSONA_STATE_BUSY] = "busy",
	[steamworks.PERSONA_STATE_AWAY] = "away",
	[steamworks.PERSONA_STATE_SNOOZE] = "snooze",
	[steamworks.PERSONA_STATE_LOOKING_TO_TRADE] = "looking to trade",
	[steamworks.PERSONA_STATE_LOOKING_TO_PLAY] = "looking to play",
}

function M.to_string(state)
	return PERSONASTATES[state]
end

return setmetatable(M, {
	__call = function(t, state)
		return M.to_string(state)
	end
})