#include "stdafx.h"
#include "com.mojang.nbt.h"
#include "net.minecraft.stats.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.damagesource.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.entity.ai.goal.h"
#include "net.minecraft.world.entity.ai.goal.target.h"
#include "net.minecraft.world.entity.ai.navigation.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.global.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.h"
#include "FireMinion.h"
#include "..\Minecraft.Client\Textures.h"
#include "MobCategory.h"

FireMinion::FireMinion(Level *level) : Monster( level )
{
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());

	setSize(1.1f, 1.95f);

	fireImmune = true;
	xpReward = Enemy::XP_REWARD_MEDIUM;

	getNavigation()->setAvoidWater(true);
	goalSelector.addGoal(0, new FloatGoal(this));
	goalSelector.addGoal(2, new MeleeAttackGoal(this, eTYPE_PLAYER, 1.0, false));
	goalSelector.addGoal(4, new MoveTowardsRestrictionGoal(this, 1.0));
	goalSelector.addGoal(6, new RandomStrollGoal(this, 1.0));
	goalSelector.addGoal(7, new LookAtPlayerGoal(this, typeid(Player), 8));
	goalSelector.addGoal(8, new RandomLookAroundGoal(this));

	targetSelector.addGoal(1, new HurtByTargetGoal(this, true));
	targetSelector.addGoal(2, new NearestAttackableTargetGoal(this, typeid(Player), 0, true));
}

bool FireMinion::useNewAi()
{
	return true;
}

void FireMinion::aiStep() 
{
	Monster::aiStep();

	for (int i = 0; i < 2; i++)
	{
		level->addParticle(eParticleType_flame, x + (random->nextDouble() - 0.5) * bbWidth / 2, y + 0.8 - random->nextDouble() * 1.2, z + (random->nextDouble() - 0.5) * bbWidth / 2, 0, 0, 0);
	}

	if (!onGround && yd < 0) 
	{
		yd *= 0.6;
	}
}

void FireMinion::registerAttributes()
{
	Monster::registerAttributes();

	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(40);
	getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.25f);
	getAttribute(SharedMonsterAttributes::ATTACK_DAMAGE)->setBaseValue(15);
	getAttribute(SharedMonsterAttributes::FOLLOW_RANGE)->setBaseValue(40);
}

void FireMinion::newServerAiStep()
{
	Monster::newServerAiStep();
}

int FireMinion::getDeathLoot() 
{
	return 0;
}

bool FireMinion::isPushable()
{
	return false;
}

void FireMinion::causeFallDamage(float distance) 
{
}

bool FireMinion::makeStepSound()
{
	return false;
}

bool FireMinion::hurt(DamageSource *source, float dmg)
{
	if (Monster::hurt(source, dmg))
	{
		shared_ptr<LivingEntity> target = getTarget();
		if ( (target == nullptr) && getAttackTarget() != nullptr && getAttackTarget()->instanceof(eTYPE_LIVINGENTITY) )	target = dynamic_pointer_cast<LivingEntity>( getAttackTarget() );
		if ( (target == nullptr) && source->getEntity() != nullptr && source->getEntity()->instanceof(eTYPE_LIVINGENTITY) )	target = dynamic_pointer_cast<LivingEntity>( source->getEntity() );

		return true;
	}
	return false;
}

bool FireMinion::doHurtTarget(shared_ptr<Entity> target)
{
	if (Monster::doHurtTarget(target))
	{
		return true;
	}
	return false;
}