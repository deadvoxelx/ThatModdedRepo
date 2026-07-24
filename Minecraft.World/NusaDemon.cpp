#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.entity.ai.goal.h"
#include "net.minecraft.world.entity.ai.navigation.h"
#include "net.minecraft.world.entity.ai.goal.target.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.damagesource.h"
#include "NusaDemon.h"
#include "MobCategory.h"
#include "..\Minecraft.Client\Textures.h"

NusaDemon::NusaDemon(Level *level) : Monster( level )
{
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());
	setSize(2.7f, 6.0f);

	xpReward = Enemy::XP_REWARD_LARGE;

	getNavigation()->setAvoidWater(true);
	goalSelector.addGoal(3, new MeleeAttackGoal(this, 1.0, true));
	goalSelector.addGoal(6, new RandomStrollGoal(this, 1.0));
	goalSelector.addGoal(7, new LookAtPlayerGoal(this, typeid(Player), 8));
	goalSelector.addGoal(8, new RandomLookAroundGoal(this));

	targetSelector.addGoal(1, new HurtByTargetGoal(this, true));
	targetSelector.addGoal(2, new NearestAttackableTargetGoal(this, typeid(Player), 0, true));
}

void NusaDemon::registerAttributes()
{
	Monster::registerAttributes();

	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(180);
	getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.32f);
	getAttribute(SharedMonsterAttributes::FOLLOW_RANGE)->setBaseValue(44);
	getAttribute(SharedMonsterAttributes::ATTACK_DAMAGE)->setBaseValue(14);
	getAttribute(SharedMonsterAttributes::KNOCKBACK_RESISTANCE)->setBaseValue(1.0);
}

int NusaDemon::getHurtSound()
{
	return eSoundType_MOB_WITHER_HURT;
}

int NusaDemon::getDeathSound()
{
	return eSoundType_MOB_WITHER_DEATH;
}

bool NusaDemon::hurt(DamageSource *source, float dmg)
{
	if (source == DamageSource::cactus) return false;
	if (source == DamageSource::dragonbreath) return false;
	if (source == DamageSource::drown) return false;
	if (source == DamageSource::fall) return false;
	if (source == DamageSource::inWall) return false;
	if (source == DamageSource::magic) return false;
	if (source == DamageSource::inFire) return false;
	if (source == DamageSource::onFire) return false;
	if (source == DamageSource::lava) return false;
	if (source->isExplosion()) return false;

	if (!level->isClientSide)
	{
		if (random->nextInt(3) == 0)
		{
			float r = 2.0f;
			level->explode(shared_from_this(), x, y + 2, z, r, true);
		}
	}

	if (Monster::hurt(source, dmg))
	{
		shared_ptr<LivingEntity> target = getTarget();
		if ( (target == nullptr) && getAttackTarget() != nullptr && getAttackTarget()->instanceof(eTYPE_LIVINGENTITY) )	target = dynamic_pointer_cast<LivingEntity>( getAttackTarget() );
		if ( (target == nullptr) && source->getEntity() != nullptr && source->getEntity()->instanceof(eTYPE_LIVINGENTITY) )	target = dynamic_pointer_cast<LivingEntity>( source->getEntity() );

		return true;
	}
	return false;
}

void NusaDemon::die(DamageSource *source)
{
	Monster::die(source);

	if (!level->isClientSide)
	{
		float r = 5.0f;
		level->explode(shared_from_this(), x, y, z, r, true);
	}
}

bool NusaDemon::useNewAi()
{
	return true;
}

void NusaDemon::newServerAiStep()
{
	Monster::newServerAiStep();
}

int NusaDemon::getDeathLoot() 
{
	return 0;
}

bool NusaDemon::removeWhenFarAway()
{
	return true;
}

bool NusaDemon::isPushable()
{
	return false;
}

void NusaDemon::doPush(shared_ptr<Entity> e)
{
}

void NusaDemon::pushEntities()
{
}

int NusaDemon::decreaseAirSupply(int currentSupply)
{
	return currentSupply;
}