local M = {}

PERSONASTATES = {
	[steam.EPersonaStateOffline] = "offline",
	[steam.EPersonaStateOnline] = "online",
	[steam.EPersonaStateBusy] = "busy",
	[steam.EPersonaStateAway] = "away",
	[steam.EPersonaStateSnooze] = "snooze",
	[steam.EPersonaStateLookingToTrade] = "looking to trade",
	[steam.EPersonaStateLookingToPlay] = "looking to play",
	[steam.EPersonaStateInvisible] = "invisible",
}

function M.to_string(state)
	return PERSONASTATES[state]
end

return setmetatable(M, {
	__call = function(t, state)
		return M.to_string(state)
	end
})