#include "stdafx.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.entity.projectile.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.damagesource.h"
#include "net.minecraft.stats.h"
#include "Aerwhale.h"
#include "..\Minecraft.Client\Textures.h"
#include "LevelEvent.h"
#include "SoundTypes.h"

void Aerwhale::_init()
{
	floatDuration = 0;
	target = nullptr;

	xTarget = 0.0f;
	yTarget = 0.0f;
	zTarget = 0.0f;
}

Aerwhale::Aerwhale(Level *level) : FlyingMob( level )
{
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());

	_init();

	setSize(4, 3);
	fireImmune = true;
	xpReward = Enemy::XP_REWARD_MEDIUM;
}

int Aerwhale::getAmbientSound() 
{
	return eSoundType_MOB_AERWHALE_CALL;
}

int Aerwhale::getHurtSound() 
{
	return eSoundType_MOB_AERWHALE_CALL;
}

int Aerwhale::getDeathSound() 
{
	return eSoundType_MOB_AERWHALE_DEATH;
}

bool Aerwhale::hurt(DamageSource *source, float dmg)
{
	if (isInvulnerable()) return false;

	return FlyingMob::hurt(source, dmg);
}

void Aerwhale::defineSynchedData() 
{
	FlyingMob::defineSynchedData();
}

void Aerwhale::registerAttributes()
{
	FlyingMob::registerAttributes();

	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(20);
}

void Aerwhale::serverAiStep() 
{
	checkDespawn();

	double xd = xTarget - x;
	double yd = yTarget - y;
	double zd = zTarget - z;

	double dd = xd * xd + yd * yd + zd * zd;

	if (dd < 1 * 1 || dd > 60 * 60)
	{
		xTarget = x + (random->nextFloat() * 2 - 1) * 16;
		yTarget = y + (random->nextFloat() * 2 - 1) * 16;
		zTarget = z + (random->nextFloat() * 2 - 1) * 16;
	}

	if (floatDuration-- <= 0)
	{
		floatDuration += random->nextInt(5) + 2;

		dd = sqrt(dd);

		if (canReach(xTarget, yTarget, zTarget, dd)) 
		{
			this->xd += xd / dd * 0.1;
			this->yd += yd / dd * 0.1;
			this->zd += zd / dd * 0.1;
		}
		else
		{
			xTarget = x;
			yTarget = y;
			zTarget = z;
		}
	}

	{
		yBodyRot = yRot = -static_cast<float>(atan2(this->xd, this->zd)) * 180 / PI;
	}
}

bool Aerwhale::canReach(double xt, double yt, double zt, double dist) 
{
	double xd = (xTarget - x) / dist;
	double yd = (yTarget - y) / dist;
	double zd = (zTarget - z) / dist;

	AABB *bb = this->bb->copy();
	for (int d = 1; d < dist; d++)
	{
		bb->move(xd, yd, zd);
		if (!level->getCubes( shared_from_this(), bb)->empty()) return false;
	}

	return true;
}

bool Aerwhale::canSpawn()
{
	return (random->nextInt(20) == 0 && FlyingMob::canSpawn());
}

int Aerwhale::getMaxSpawnClusterSize()
{
	return 1;
}
void Aerwhale::addAdditonalSaveData(CompoundTag *tag)
{
	FlyingMob::addAdditonalSaveData(tag);
}

void Aerwhale::readAdditionalSaveData(CompoundTag *tag)
{
	FlyingMob::readAdditionalSaveData(tag);
}
