#include "stdafx.h"
#include "com.mojang.nbt.h"
#include "net.minecraft.stats.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.ambient.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.entity.ai.goal.h"
#include "net.minecraft.world.entity.ai.navigation.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.damagesource.h"
#include "net.minecraft.world.phys.h"
#include "MobCategory.h"
#include "NusaSpike.h"
#include "..\Minecraft.Client\Minecraft.h"

NusaSpike::NusaSpike(Level *level) : AmbientCreature(level)
{
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());
	setSize(0.5f, 1.8f);

	fireImmune = true;
	spikeLifetime = 60;
}

void NusaSpike::defineSynchedData()
{
	AmbientCreature::defineSynchedData();
}

bool NusaSpike::isPushable()
{
	return false;
}

void NusaSpike::doPush(shared_ptr<Entity> e)
{
}

void NusaSpike::pushEntities()
{
}

void NusaSpike::registerAttributes()
{
	AmbientCreature::registerAttributes();

	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(10);
}

bool NusaSpike::useNewAi()
{
	return true;
}

void NusaSpike::tick()
{
	AmbientCreature::tick();

	xd = 0;
	yd = 0;
	zd = 0;

	if (spikeLifetime > 0 && --spikeLifetime <= 0)
	{
		remove();
	}
}

bool NusaSpike::hurt(DamageSource *source, float dmg)
{
	return false;
}

void NusaSpike::newServerAiStep()
{
	AmbientCreature::newServerAiStep();
}

bool NusaSpike::makeStepSound()
{
	return false;
}

void NusaSpike::causeFallDamage(float distance)
{
}

void NusaSpike::checkFallDamage(double ya, bool onGround)
{
}

bool NusaSpike::isIgnoringTileTriggers()
{
	return true;
}

void NusaSpike::playerTouch(shared_ptr<Player> player)
{	// Damages on contact
	if (isDealsDamage())
	{
		if (distanceToSqr(player) < (0.8) * (0.8))
		{
			DamageSource *damageSource = DamageSource::mobAttack( dynamic_pointer_cast<Mob>( shared_from_this() ) );
			if (player->hurt(damageSource, getAttackDamage()))
			{
			}
			delete damageSource;
		}
	}
}

bool NusaSpike::isDealsDamage()
{
	return true;
}

int NusaSpike::getAttackDamage()
{
	return 8;
}