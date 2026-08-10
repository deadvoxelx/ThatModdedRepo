#include "stdafx.h"
#include "com.mojang.nbt.h"
#include "net.minecraft.stats.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.entity.ai.goal.h"
#include "net.minecraft.world.entity.ai.navigation.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.global.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.h"
#include "SunSpirit.h"
#include "..\Minecraft.Client\Textures.h"
#include "MobCategory.h"

SunSpirit::SunSpirit(Level *level) : Monster( level )
{
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());

	setSize(2.5f, 2.8f);

	getNavigation()->setAvoidWater(true);
	goalSelector.addGoal(0, new FloatGoal(this));
	goalSelector.addGoal(7, new LookAtPlayerGoal(this, typeid(Player), 32));
	goalSelector.addGoal(8, new RandomLookAroundGoal(this));
}

bool SunSpirit::useNewAi()
{
	return true;
}

void SunSpirit::aiStep() 
{
	Monster::aiStep();

	xd = 0;
	yd = 0;
	zd = 0;

	for (int i = 0; i < 2; i++)
	{
		level->addParticle(eParticleType_flame, x + (random->nextDouble() - 0.5) * bbWidth / 2, y - random->nextDouble() * 1.2, z + (random->nextDouble() - 0.5) * bbWidth / 2, 0, 0, 0);
	}
}

void SunSpirit::registerAttributes()
{
	Monster::registerAttributes();

	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(500);
	getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.0f);
	getAttribute(SharedMonsterAttributes::KNOCKBACK_RESISTANCE)->setBaseValue(1.0);
}

void SunSpirit::newServerAiStep()
{
	Monster::newServerAiStep();
}

int SunSpirit::getDeathLoot() 
{
	return 0;
}

void SunSpirit::dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel)
{
	spawnAtLocation(Item::keyGold_Id, 1);
}

bool SunSpirit::removeWhenFarAway()
{
	return false;
}

bool SunSpirit::isPushable()
{
	return false;
}

void SunSpirit::causeFallDamage(float distance) 
{
}

bool SunSpirit::hurt(DamageSource *source, float dmg)
{
	return false;
}