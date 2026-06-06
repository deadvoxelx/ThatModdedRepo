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
	//retargetTime = 0;
	//oCharge = 0;
	//charge = 0;

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

	setSize(3, 3);
	fireImmune = true;
	xpReward = Enemy::XP_REWARD_MEDIUM;
}

//bool Aerwhale::isCharging()
//{
//	return entityData->getByte(DATA_IS_CHARGING) != 0;
//}

bool Aerwhale::hurt(DamageSource *source, float dmg)
{
	if (isInvulnerable()) return false;

	return FlyingMob::hurt(source, dmg);
}

void Aerwhale::defineSynchedData() 
{
	FlyingMob::defineSynchedData();

	//entityData->define(DATA_IS_CHARGING, static_cast<byte>(0));
}

void Aerwhale::registerAttributes()
{
	FlyingMob::registerAttributes();

	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(20);
}

void Aerwhale::serverAiStep() 
{
	checkDespawn();

	//oCharge = charge;
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

	/*if (target != nullptr && target->removed) target = nullptr;
	if (target == nullptr || retargetTime-- <= 0)
	{
		target = level->getNearestAttackablePlayer(shared_from_this(), 100);
		if (target != nullptr) 
		{
			retargetTime = 20;
		}
	}*/

	//double maxDist = 32.0f;
	/*if (target != nullptr && target->distanceToSqr(shared_from_this()) < maxDist * maxDist) 
	{
		double xdd = target->x - x;
		double ydd = (target->bb->y0 + target->bbHeight / 2) - (y + bbHeight / 2);
		double zdd = target->z - z;
		yBodyRot = yRot = -static_cast<float>(atan2(xdd, zdd)) * 180 / PI;
	}
	else*/
	{
		yBodyRot = yRot = -static_cast<float>(atan2(this->xd, this->zd)) * 180 / PI;
		//if (charge > 0) charge--;
	}

	/*if (!level->isClientSide) 
	{
		byte old = entityData->getByte(DATA_IS_CHARGING);
		byte current = static_cast<byte>(charge > 10 ? 1 : 0);
		if (old != current)
		{
			entityData->set(DATA_IS_CHARGING, current);
		}
	}*/
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
