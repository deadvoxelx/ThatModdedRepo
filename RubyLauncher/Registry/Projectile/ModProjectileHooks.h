#pragma once

class Entity;
class Level;

namespace ModProjectileHooks
{
	bool fireHit(int specId, Entity *projectile, Entity *target, Level *level);

	void fireTick(int specId, Entity *projectile, Level *level);
}
