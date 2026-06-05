#include "stdafx.h"
#include "net.minecraft.world.damagesource.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.phys.h"
#include "ZephyrFireball.h"

ZephyrFireball::ZephyrFireball(Level *level) : Fireball(level)
{
	setSize(5 / 16.0f, 5 / 16.0f);
}

ZephyrFireball::ZephyrFireball(Level *level, double x, double y, double z, double xa, double ya, double za) : Fireball(level, x, y, z, xa, ya, za)
{
	setSize(5 / 16.0f, 5 / 16.0f);
}

ZephyrFireball::ZephyrFireball(Level *level, shared_ptr<LivingEntity> mob, double xa, double ya, double za) : Fireball(level, mob, xa, ya, za)
{
	setSize(5 / 16.0f, 5 / 16.0f);
}

bool ZephyrFireball::isPickable()
{
	return false;
}

void ZephyrFireball::onHit(HitResult *res)
{
	if (!level->isClientSide)
	{
		if (res->entity != nullptr)
		{
			DamageSource *damageSource = DamageSource::fireball(dynamic_pointer_cast<Fireball>( shared_from_this() ), owner);
			res->entity->hurt(damageSource, 6);
			delete damageSource;
		}
		remove();
	}
}

bool ZephyrFireball::shouldBurn()
{
	return false;
}

void ZephyrFireball::addAdditonalSaveData(CompoundTag *tag)
{
	Fireball::addAdditonalSaveData(tag);
}

void ZephyrFireball::readAdditionalSaveData(CompoundTag *tag)
{
	Fireball::readAdditionalSaveData(tag);
}