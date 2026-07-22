#include "stdafx.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.damagesource.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.entity.ai.goal.h"
#include "net.minecraft.world.entity.ai.goal.target.h"
#include "net.minecraft.world.entity.ai.navigation.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.entity.player.h"
#include "Nuskull.h"
#include "GenericStats.h"
#include "JavaMath.h"
#include "SoundTypes.h"
#include "..\Minecraft.Client\Textures.h"

Nuskull::Nuskull(Level *level) : FlyingMonster( level )
{
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());
	setSize(0.7f, 0.7f);

	moveTargetX = 0.0;
    moveTargetY = 0.0;
    moveTargetZ = 0.0;

	targetPosition = nullptr;

	getNavigation()->setAvoidWater(true);
	goalSelector.addGoal(0, new FloatGoal(this));
	goalSelector.addGoal(2, new LeapAtTargetGoal(this, 0.4));
	goalSelector.addGoal(3, new MeleeAttackGoal(this, 1.0, false));
	goalSelector.addGoal(6, new RandomStrollGoal(this, 1.0));

	targetSelector.addGoal(1, new HurtByTargetGoal(this, true));
	targetSelector.addGoal(2, new NearestAttackableTargetGoal(this, typeid(Player), 0, true));
}

void Nuskull::defineSynchedData()
{
	FlyingMonster::defineSynchedData();

	entityData->define(DATA_TYPE_ID, static_cast<byte>(0));
}

void Nuskull::registerAttributes()
{
	FlyingMonster::registerAttributes();
	
	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(30);
	getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.05f);
	getAttribute(SharedMonsterAttributes::ATTACK_DAMAGE)->setBaseValue(6);
}

bool Nuskull::useNewAi()
{
	return true;
}

void Nuskull::aiStep()
{
	FlyingMonster::aiStep();
}

void Nuskull::tick()
{
	FlyingMonster::tick();
} 

int Nuskull::getDeathLoot()
{
	return 0;
}

bool Nuskull::makeStepSound()
{
	return false;
}

bool Nuskull::removeWhenFarAway()
{
	return true;
}

void Nuskull::causeFallDamage(float distance)
{
}

void Nuskull::checkFallDamage(double ya, bool onGround)
{
}

inline int signum(double x) { return (x > 0) - (x < 0); }

void Nuskull::newServerAiStep()
{
	FlyingMonster::newServerAiStep();
	{
		shared_ptr<LivingEntity> target = getTarget();
		if ((target == nullptr))
		{
			if (targetPosition != nullptr && (!level->isEmptyTile(targetPosition->x, targetPosition->y, targetPosition->z) || targetPosition->y))
			{
				delete targetPosition;
				targetPosition = nullptr;
			}
			if (targetPosition == nullptr || random->nextInt(256) == 0 || targetPosition->distSqr(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z)))
			{
				delete targetPosition;
				targetPosition = new Pos(static_cast<int>(x) + random->nextInt(48) - random->nextInt(48), static_cast<int>(y) + random->nextInt(12) - random->nextInt(12), static_cast<int>(z) + random->nextInt(48) - random->nextInt(48));
			}
		}
		else
		{
			if (targetPosition != nullptr)
			{
				delete targetPosition;
				targetPosition = nullptr;
			}
			if (targetPosition == nullptr)
			{
				delete targetPosition;
      			targetPosition = new Pos(target->x, target->y, target->z);
			}
		}
		
		double dx = (targetPosition->x + .3) - x;
		double dy = (targetPosition->y + .1) - y;
		double dz = (targetPosition->z + .3) - z;

		xd = xd + (signum(dx) * .32f - xd) * .1f;
		yd = yd + (signum(dy) * .32f - yd) * .1f;
		zd = zd + (signum(dz) * .32f - zd) * .1f;

		float yRotD = static_cast<float>(atan2(zd, xd) * 180 / PI) - 90;
		float rotDiff = Mth::wrapDegrees(yRotD - yRot);
		yya = .5f;
		yRot += rotDiff;
	}
}

bool Nuskull::hurt(DamageSource *source, float dmg)
{
	if (source == DamageSource::cactus) return false;
	if (source == DamageSource::dragonbreath) return false;
	if (source == DamageSource::drown) return false;
	if (source == DamageSource::inWall) return false;
	if (source == DamageSource::magic) return false;
	if (source == DamageSource::inFire) return false;
	if (source == DamageSource::onFire) return false;
	if (source == DamageSource::lava) return false;
	if (source->isExplosion()) return false;

	if (!level->isClientSide)
	{
		float r = 2.0f;
		level->explode(shared_from_this(), x, y, z, r, true);
		remove();
	}

	if (FlyingMonster::hurt(source, dmg))
	{
		shared_ptr<LivingEntity> target = getTarget();
		if ( (target == nullptr) && getAttackTarget() != nullptr && getAttackTarget()->instanceof(eTYPE_LIVINGENTITY) )	target = dynamic_pointer_cast<LivingEntity>( getAttackTarget() );
		if ( (target == nullptr) && source->getEntity() != nullptr && source->getEntity()->instanceof(eTYPE_LIVINGENTITY) )	target = dynamic_pointer_cast<LivingEntity>( source->getEntity() );

		return true;
	}
	return false;
}

int Nuskull::getNuskullType()
{
	return (int) entityData->getByte(DATA_TYPE_ID);
}

void Nuskull::setNuskullType(int type)
{
	entityData->set(DATA_TYPE_ID, (byte) type);
}

MobGroupData *Nuskull::finalizeMobSpawn(MobGroupData *groupData, int extraData)
{
	groupData = FlyingMonster::finalizeMobSpawn(groupData);

	if (random->nextInt(2) == 0)
	{
		setNuskullType(TYPE_VARIANT);
	}
	else
	{
		setNuskullType(TYPE_DEFAULT);
	}

	return groupData;
}

void Nuskull::readAdditionalSaveData(CompoundTag *tag)
{
	FlyingMonster::readAdditionalSaveData(tag);

	if (tag->contains(L"NuskullType"))
	{
		int value = tag->getByte(L"NuskullType");
		setNuskullType(value);
	}
}

void Nuskull::addAdditonalSaveData(CompoundTag *entityTag)
{
	FlyingMonster::addAdditonalSaveData(entityTag);
	entityTag->putByte(L"NuskullType", (byte) getNuskullType());
}