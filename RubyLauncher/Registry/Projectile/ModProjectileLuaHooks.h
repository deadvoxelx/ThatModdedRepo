#pragma once

#include "../../../Minecraft.World/ParticleTypes.h"
#include "sol/sol.hpp"

namespace ModProjectileLuaHooks
{
	void registerHooks(int specId, sol::function onHit, sol::function onTick);
}
