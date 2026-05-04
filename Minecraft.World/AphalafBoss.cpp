#include "stdafx.h"
#include "com.mojang.nbt.h"
#include "net.minecraft.stats.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.dimension.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.effect.h"
#include "net.minecraft.world.damagesource.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.entity.ai.goal.h"
#include "net.minecraft.world.entity.ai.navigation.h"
#include "net.minecraft.world.entity.ai.goal.target.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.global.h"
#include "AphalafBoss.h"
#include "..\Minecraft.Client\Textures.h"
#include "MobCategory.h"

AphalafBoss::AphalafBoss(Level *level) : Monster( level )
{
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());
	setSize(2.2f, 3.0f);

	xpReward = Enemy::XP_REWARD_BOSS;
	fireImmune = true;

	getNavigation()->setAvoidWater(true);
	goalSelector.addGoal(3, new MeleeAttackGoal(this, eTYPE_PLAYER, 1.0, true));
	goalSelector.addGoal(8, new RandomLookAroundGoal(this));

	targetSelector.addGoal(1, new HurtByTargetGoal(this, true));
	targetSelector.addGoal(2, new NearestAttackableTargetGoal(this, typeid(Player), 0, true));
}

int AphalafBoss::getHurtSound()
{
	return eSoundType_MOB_APHALAF_HURT;
}

int AphalafBoss::getDeathSound()
{
	return eSoundType_MOB_APHALAF_HURT;
}

bool AphalafBoss::hurt(DamageSource *source, float dmg)
{
	if (source == DamageSource::cactus) return false;
	if (source == DamageSource::dragonbreath) return false;
	if (source == DamageSource::drown) return false;
	if (source == DamageSource::fall) return false;
	if (source == DamageSource::inWall) return false;
	if (source == DamageSource::lava) return false;
	if (source == DamageSource::magic) return false;
	if (source == DamageSource::wither) return false;
	if (source->isExplosion()) return false;

	shared_ptr<Entity> directEntity = source->getDirectEntity();
	if (directEntity != nullptr && directEntity->GetType() == eTYPE_ARROW)
	{
		return false;
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

bool AphalafBoss::doHurtTarget(shared_ptr<Entity> target)
{
	if (Monster::doHurtTarget(target))
	{
		if ( target->instanceof(eTYPE_LIVINGENTITY) )
		{
			dynamic_pointer_cast<LivingEntity>(target)->addEffect(new MobEffectInstance(MobEffect::weakness->id, 600, 1));
		}
		return true;
	}
	return false;
} 

bool AphalafBoss::useNewAi()
{
	return true;
}

void AphalafBoss::registerAttributes()
{
	Monster::registerAttributes();

	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(250);
	getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.0f);
	getAttribute(SharedMonsterAttributes::ATTACK_DAMAGE)->setBaseValue(12);
	getAttribute(SharedMonsterAttributes::KNOCKBACK_RESISTANCE)->setBaseValue(1.0);
}

void AphalafBoss::newServerAiStep()
{
	Monster::newServerAiStep();
}

int AphalafBoss::getDeathLoot()
{
	return 0;
}

void AphalafBoss::dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel)
{
	spawnAtLocation(Item::aphalafSword_Id, 1);
}

bool AphalafBoss::removeWhenFarAway()
{
	return true;
}

bool AphalafBoss::isPushable()
{
	return false;
}

void AphalafBoss::doPush(shared_ptr<Entity> e)
{
}

void AphalafBoss::pushEntities()
{
}

int AphalafBoss::decreaseAirSupply(int currentSupply)
{
	return currentSupply;
}

void AphalafBoss::tick()
{
	Monster::tick();

	shared_ptr<Player> range1 = level->getNearestAttackablePlayer(shared_from_this(), 12);
	shared_ptr<Player> range2 = level->getNearestAttackablePlayer(shared_from_this(), 24);

	if ((range1 != nullptr) && (range2 != nullptr))
	{
		if (getRandom()->nextInt(25) < 1)
		{
			explode1();
		}
	}
	else if ((range1 == nullptr) && (range2 != nullptr))
	{
		if (getRandom()->nextInt(25) < 1)
		{
			explode2();
		}
	}

	if (dimension == 2 && y < 10)
	{
		remove();
	}
}

void AphalafBoss::explode1()
{
	float r = 3.0f;
	level->explode(shared_from_this(), x + 8, y, z, r, false);
	level->explode(shared_from_this(), x, y, z + 8, r, false);
	level->explode(shared_from_this(), x - 8, y, z, r, false);
	level->explode(shared_from_this(), x, y, z - 8, r, false);
	level->explode(shared_from_this(), x + 7, y, z + 7, r, false);
	level->explode(shared_from_this(), x - 7, y, z + 7, r, false);
	level->explode(shared_from_this(), x - 7, y, z - 7, r, false);
	level->explode(shared_from_this(), x + 7, y, z - 7, r, false);
}

void AphalafBoss::explode2()
{
	float r = 3.0f;
	level->explode(shared_from_this(), x + 16, y, z, r, false);
	level->explode(shared_from_this(), x, y, z + 16, r, false);
	level->explode(shared_from_this(), x - 16, y, z, r, false);
	level->explode(shared_from_this(), x, y, z - 16, r, false);
	level->explode(shared_from_this(), x + 14, y, z + 14, r, false);
	level->explode(shared_from_this(), x - 14, y, z + 14, r, false);
	level->explode(shared_from_this(), x - 14, y, z - 14, r, false);
	level->explode(shared_from_this(), x + 14, y, z - 14, r, false);
}