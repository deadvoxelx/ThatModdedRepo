#include "stdafx.h"
#include "com.mojang.nbt.h"
#include "net.minecraft.stats.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.entity.ai.goal.h"
#include "net.minecraft.world.entity.ai.navigation.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.phys.h"
#include "MobCategory.h"
#include "CockatriceSpawner.h"
#include "..\Minecraft.Client\Minecraft.h"

CockatriceSpawner::CockatriceSpawner(Level *level) : AmbientCreature(level)
{
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());
	setSize(1.0f, 2.0f);
}

void CockatriceSpawner::defineSynchedData()
{
	AmbientCreature::defineSynchedData();
}

bool CockatriceSpawner::isPushable()
{
	return false;
}

void CockatriceSpawner::doPush(shared_ptr<Entity> e)
{
}

void CockatriceSpawner::pushEntities()
{
}

void CockatriceSpawner::registerAttributes()
{
	AmbientCreature::registerAttributes();

	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(10);
}

bool CockatriceSpawner::useNewAi()
{
	return true;
}

void CockatriceSpawner::tick()
{
	AmbientCreature::tick();

	if (!level->isClientSide)
	{
		if (level->canCreateMore(eTYPE_MONSTER, Level::eSpawnType_Egg) && level->difficulty != Difficulty::PEACEFUL)
		{
			shared_ptr<Cockatrice> pz = std::make_shared<Cockatrice>(level);

			level->addEntity(pz);
			pz->moveTo(x, y, z, yRot, xRot);
			remove();
		}
		else
		{
			remove();
		}
	}
}

void CockatriceSpawner::newServerAiStep()
{
	AmbientCreature::newServerAiStep();
}

bool CockatriceSpawner::makeStepSound()
{
	return false;
}

void CockatriceSpawner::causeFallDamage(float distance)
{
}

void CockatriceSpawner::checkFallDamage(double ya, bool onGround)
{
}

bool CockatriceSpawner::isIgnoringTileTriggers()
{
	return true;
}

bool CockatriceSpawner::hurt(DamageSource *source, float dmg)
{
	return AmbientCreature::hurt(source, dmg);
}