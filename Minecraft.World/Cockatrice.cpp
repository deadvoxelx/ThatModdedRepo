#include "stdafx.h"
#include "com.mojang.nbt.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.global.h"
#include "net.minecraft.world.entity.projectile.h"
#include "net.minecraft.world.entity.ai.goal.h"
#include "net.minecraft.world.entity.ai.goal.target.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.entity.ai.navigation.h"
#include "MobCategory.h"
#include "Cockatrice.h"
#include "..\Minecraft.Client\Minecraft.h"
#include "..\Minecraft.Client\Textures.h"

void Cockatrice::_init()
{
	flap = 0;
	flapSpeed = 0;
	flapping = 1;
	oFlapSpeed = oFlap = 0.0f;
}

Cockatrice::Cockatrice(Level *level) : Monster( level )
{
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());

	setSize(1.2f, 2.0f);

	getNavigation()->setAvoidWater(true);
	goalSelector.addGoal(0, new FloatGoal(this));
	goalSelector.addGoal(2, new RangedAttackGoal(this, this, 1.0, SharedConstants::TICKS_PER_SECOND * 2, SharedConstants::TICKS_PER_SECOND * 3, 15));
	goalSelector.addGoal(5, new RandomStrollGoal(this, 1.0));
	goalSelector.addGoal(7, new LookAtPlayerGoal(this, typeid(Player), 6));
	goalSelector.addGoal(8, new RandomLookAroundGoal(this));

	targetSelector.addGoal(1, new HurtByTargetGoal(this, false));
	targetSelector.addGoal(2, new NearestAttackableTargetGoal(this, typeid(Player), 0, true));
}

bool Cockatrice::useNewAi()
{
	return true;
}

void Cockatrice::registerAttributes()
{
	Monster::registerAttributes();

	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(24);
	getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.25f);
}

void Cockatrice::aiStep()
{
	Monster::aiStep();

	oFlap = flap;
	oFlapSpeed = flapSpeed;

	flapSpeed += (onGround ? -1 : 4) * 0.3f;
	if (flapSpeed < 0) flapSpeed = 0;
	if (flapSpeed > 0.75) flapSpeed = 0.75;

	if (!onGround && flapping < 1) flapping = 1;
	flapping *= 0.9;

	if (!onGround && yd < 0) 
	{
		yd *= 0.6;
	}

	flap += flapping * 2;
}

void Cockatrice::newServerAiStep()
{
	Monster::newServerAiStep();
}

void Cockatrice::causeFallDamage(float distance) 
{
}

void Cockatrice::performRangedAttack(shared_ptr<LivingEntity> target, float power)
{
	shared_ptr<DartPoison> dart = shared_ptr<DartPoison>( new DartPoison(level, dynamic_pointer_cast<LivingEntity>(shared_from_this()), target, 1.60f, 14 - (level->difficulty * 4)) );

	dart->setBaseDamage(power * 2.0f + (random->nextGaussian() * 0.25f + (level->difficulty * 0.11f)));

	playSound(eSoundType_RANDOM_BOW, 1.0f, 1 / (getRandom()->nextFloat() * 0.4f + 0.8f));
	level->addEntity(dart);
}

int Cockatrice::getDeathLoot() 
{
	return Item::feather->id;
}

void Cockatrice::dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel)
{
	int count = 1 + Mth::nextInt(level->random, 1, 2) + random->nextInt(1 + playerBonusLevel);
	for (int i = 0; i < count; i++)
	{
		spawnAtLocation(Item::feather_Id, 1);
		spawnAtLocation(Item::dartPoison_Id, 1);
	}

	if (random->nextInt(10) == 0) spawnAtLocation(Item::dartShooterPoison_Id, 1);
}
