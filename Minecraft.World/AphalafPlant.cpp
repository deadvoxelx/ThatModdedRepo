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
#include "AphalafPlant.h"
#include "AphalafBoss.h"
#include "..\Minecraft.Client\Textures.h"
#include "MobCategory.h"

AphalafPlant::AphalafPlant(Level *level) : Monster( level )
{
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());
	setSize(1.0f, 1.0f);

	xpReward = Enemy::XP_REWARD_MEDIUM;

	getNavigation()->setAvoidWater(true);
	goalSelector.addGoal(3, new MeleeAttackGoal(this, 1.0, true));
	goalSelector.addGoal(8, new RandomLookAroundGoal(this));

	targetSelector.addGoal(1, new HurtByTargetGoal(this, true));
	targetSelector.addGoal(2, new NearestAttackableTargetGoal(this, typeid(Player), 0, true));
}

int AphalafPlant::getHurtSound()
{
	return eSoundType_MOB_APHALAF_HURT;
}

int AphalafPlant::getDeathSound()
{
	return eSoundType_MOB_APHALAF_HURT;
}

bool AphalafPlant::hurt(DamageSource *source, float dmg)
{
	if (source == DamageSource::cactus) return false;
	if (source == DamageSource::dragonbreath) return false;
	if (source == DamageSource::drown) return false;
	if (source == DamageSource::fall) return false;
	if (source == DamageSource::inWall) return false;
	if (source == DamageSource::magic) return false;
	if (source->isExplosion()) return false;

	if (Monster::hurt(source, dmg))
	{
		shared_ptr<LivingEntity> target = getTarget();
		if ( (target == nullptr) && getAttackTarget() != nullptr && getAttackTarget()->instanceof(eTYPE_LIVINGENTITY) )	target = dynamic_pointer_cast<LivingEntity>( getAttackTarget() );
		if ( (target == nullptr) && source->getEntity() != nullptr && source->getEntity()->instanceof(eTYPE_LIVINGENTITY) )	target = dynamic_pointer_cast<LivingEntity>( source->getEntity() );

		return true;
	}
	return false;
}

bool AphalafPlant::doHurtTarget(shared_ptr<Entity> target)
{
	if (Monster::doHurtTarget(target))
	{
		if ( target->instanceof(eTYPE_LIVINGENTITY) )
		{
			dynamic_pointer_cast<LivingEntity>(target)->addEffect(new MobEffectInstance(MobEffect::weakness->id, 200, 0));
		}
		return true;
	}
	return false;
} 

bool AphalafPlant::useNewAi()
{
	return true;
}

void AphalafPlant::registerAttributes()
{
	Monster::registerAttributes();

	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(40);
	getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.0f);
	getAttribute(SharedMonsterAttributes::ATTACK_DAMAGE)->setBaseValue(5);
	getAttribute(SharedMonsterAttributes::KNOCKBACK_RESISTANCE)->setBaseValue(1.0);
}

void AphalafPlant::newServerAiStep()
{
	Monster::newServerAiStep();
}

int AphalafPlant::getDeathLoot() 
{
	return 0;
}

void AphalafPlant::dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel)
{
	int count = random->nextInt(3) - 1;
	for (int i = 0; i < count; i++)
	{
		spawnAtLocation(Item::aphalafTooth_Id, 1);
	}
}

bool AphalafPlant::removeWhenFarAway()
{
	return true;
}

bool AphalafPlant::isPushable()
{
	return false;
}

void AphalafPlant::doPush(shared_ptr<Entity> e)
{
}

void AphalafPlant::pushEntities()
{
}

int AphalafPlant::decreaseAirSupply(int currentSupply)
{
	return currentSupply;
}

void AphalafPlant::tick()
{
	Monster::tick();

	shared_ptr<Player> range1 = level->getNearestAttackablePlayer(shared_from_this(), 8);
	shared_ptr<Player> range2 = level->getNearestAttackablePlayer(shared_from_this(), 16);

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

void AphalafPlant::explode1()
{
	float r = 1.0f;
	level->explode(shared_from_this(), x + 5, y, z, r, false);
	level->explode(shared_from_this(), x, y, z + 5, r, false);
	level->explode(shared_from_this(), x - 5, y, z, r, false);
	level->explode(shared_from_this(), x, y, z - 5, r, false);
	level->explode(shared_from_this(), x + 4, y, z + 4, r, false);
	level->explode(shared_from_this(), x - 4, y, z + 4, r, false);
	level->explode(shared_from_this(), x - 4, y, z - 4, r, false);
	level->explode(shared_from_this(), x + 4, y, z - 4, r, false);
}

void AphalafPlant::explode2()
{
	float r = 1.0f;
	level->explode(shared_from_this(), x + 10, y, z, r, false);
	level->explode(shared_from_this(), x, y, z + 10, r, false);
	level->explode(shared_from_this(), x - 10, y, z, r, false);
	level->explode(shared_from_this(), x, y, z - 10, r, false);
	level->explode(shared_from_this(), x + 8, y, z + 8, r, false);
	level->explode(shared_from_this(), x - 8, y, z + 8, r, false);
	level->explode(shared_from_this(), x - 8, y, z - 8, r, false);
	level->explode(shared_from_this(), x + 8, y, z - 8, r, false);
}

MobGroupData* AphalafPlant::finalizeMobSpawn(MobGroupData* groupData, int extraData)
{
	groupData = Monster::finalizeMobSpawn(groupData);

	//Aphalaf Boss spawns rarely throughout the Outer End
	if (random->nextInt(15) == 0)
	{
		shared_ptr<AphalafBoss> pz = std::make_shared<AphalafBoss>(level);
		pz->moveTo(x, y, z, yRot, xRot);
		level->addEntity(pz);
		remove();
	}

	return groupData;
}
