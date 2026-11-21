#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.damagesource.h"
#include "com.mojang.nbt.h"
#include "ParticleTypes.h"
#include "ModProjectile.h"
#include "../RubyLauncher/Registry/Projectile/ModProjectileHooks.h"
#include "../RubyLauncher/Registry/Projectile/ModProjectileRegistry.h"

namespace
{
	float throwPowerFor(int specId)
	{
		const ModProjectileSpec *spec = ModProjectileRegistry::get(specId);
		return (spec != nullptr && spec->throwPower > 0.0f) ? spec->throwPower : 1.5f;
	}
}

ModProjectile::ModProjectile(Level *level) : Throwable(level)
{
	specId = -1;
}

ModProjectile::ModProjectile(Level *level, shared_ptr<LivingEntity> mob) : Throwable(level, mob)
{
	specId = -1;
}

ModProjectile::ModProjectile(Level *level, shared_ptr<LivingEntity> mob, int specId) : Throwable(level, mob, throwPowerFor(specId))
{
	this->specId = specId;
}

ModProjectile::ModProjectile(Level *level, double x, double y, double z) : Throwable(level, x, y, z)
{
	specId = -1;
}

float ModProjectile::getGravity()
{
	const ModProjectileSpec *spec = ModProjectileRegistry::get(specId);
	return (spec != nullptr) ? spec->gravity : Throwable::getGravity();
}

void ModProjectile::tick()
{
	Throwable::tick();

	if (!removed && !level->isClientSide)
	{
		ModProjectileHooks::fireTick(specId, this, level);
	}

	const ModProjectileSpec *spec = ModProjectileRegistry::get(specId);
	if (spec != nullptr && spec->trailParticleType >= 0 && !removed && !inGround && level->isClientSide)
	{
		level->addParticle((ePARTICLE_TYPE) spec->trailParticleType, x, y, z, 0, 0, 0);
	}
}

void ModProjectile::onHit(HitResult *res)
{
	const ModProjectileSpec *spec = ModProjectileRegistry::get(specId);
	if (spec == nullptr)
	{
		if (!level->isClientSide) remove();
		return;
	}

	bool replaceDefault = false;
	if (!level->isClientSide)
	{
		replaceDefault = ModProjectileHooks::fireHit(specId, this, res->entity.get(), level);
	}

	if (!replaceDefault && res->entity != nullptr && spec->damage > 0)
	{
		DamageSource *damageSource = DamageSource::thrown(shared_from_this(), getOwner());
		res->entity->hurt(damageSource, spec->damage);
		delete damageSource;
	}
	for (int i = 0; i < 8; i++)
	{
		level->addParticle((ePARTICLE_TYPE) spec->particleType, x, y, z, 0, 0, 0);
	}
	if (!level->isClientSide)
	{
		remove();
		if (!replaceDefault && spec->explosionPower > 0)
		{
			level->explode(shared_from_this(), x, y, z, spec->explosionPower, true);
		}
	}
}

void ModProjectile::addAdditonalSaveData(CompoundTag *tag)
{
	Throwable::addAdditonalSaveData(tag);
	tag->putInt(L"rubyProjectile", specId);
}

void ModProjectile::readAdditionalSaveData(CompoundTag *tag)
{
	Throwable::readAdditionalSaveData(tag);
	specId = tag->getInt(L"rubyProjectile");
}
