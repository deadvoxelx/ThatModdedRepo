#include "stdafx.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.damagesource.h"
#include "net.minecraft.world.effect.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.entity.ai.goal.h"
#include "net.minecraft.world.entity.ai.goal.target.h"
#include "net.minecraft.world.entity.ai.navigation.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.entity.animal.h"
#include "net.minecraft.world.entity.player.h"
#include "Evupul.h"
#include "GenericStats.h"
#include "JavaMath.h"
#include "SoundTypes.h"
#include "..\Minecraft.Client\Textures.h"

//have fun with this one...

Evupul::Evupul(Level *level) : FlyingMob( level )
{
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());
	setSize(0.9f, 0.5f);
	footSize = 1;
	xpReward = Enemy::XP_REWARD_MEDIUM;

	targetPosition = nullptr;

	flyX = flyY = flyZ = 0.0f;

	getNavigation()->setAvoidWater(true);
	goalSelector.addGoal(0, new FloatGoal(this));
	goalSelector.addGoal(7, new LookAtPlayerGoal(this, typeid(Player), 8));
}

void Evupul::defineSynchedData()
{
	FlyingMob::defineSynchedData();

	entityData->define(DATA_TYPE_ID, static_cast<byte>(0));
}

void Evupul::registerAttributes()
{
	FlyingMob::registerAttributes();
	
	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(30);
	getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.05f);
}

bool Evupul::useNewAi()
{
	return true;
}

void Evupul::aiStep()
{
	FlyingMob::aiStep();
}

void Evupul::tick()
{
	FlyingMob::tick();
}  

int Evupul::getDeathLoot()
{
	return 0;
}

void Evupul::dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel)
{
	if (getEvupulType() == TYPE_GOLD)
	{
		int count = random->nextInt(3) - 1;
		for (int i = 0; i < count; i++)
		{
			spawnAtLocation(Item::evupulWingGold_Id, 1);
		}
	}
	else if (getEvupulType() == TYPE_DEFAULT)
	{
		int count = random->nextInt(3) - 1;
		for (int i = 0; i < count; i++)
		{
			spawnAtLocation(Item::evupulWing_Id, 1);
		}
	}
}

bool Evupul::makeStepSound()
{
	return false;
}

bool Evupul::removeWhenFarAway()
{
	return true;
}

void Evupul::causeFallDamage(float distance)
{
}

void Evupul::checkFallDamage(double ya, bool onGround)
{
}

void Evupul::calculateFlight(float xa, float ya, float za)
{
	xa = xa * 12.0f;
	ya = 0;
	za = za * 12.0f;

	flyX = smoothFlyX.getNewDeltaValue(xa, .35f * 3.5f);
	flyY = smoothFlyY.getNewDeltaValue(ya, .35f * 3.5f);
	flyZ = smoothFlyZ.getNewDeltaValue(za, .35f * 3.5f);
}

bool Evupul::isPushable()
{
	if (rider.lock() != nullptr)
	{
		return true;
	}
	return false;
}

bool Evupul::hasRider()
{
	if (rider.lock() != nullptr)
	{
		return true;
	}
	return false;
}

inline int signum(double x) { return (x > 0) - (x < 0); }

void Evupul::newServerAiStep()
{
	FlyingMob::newServerAiStep();

	{
		if (!hasRider())
		{
			if (targetPosition != nullptr && !level->isEmptyTile(targetPosition->x, targetPosition->y, targetPosition->z))
			{
				delete targetPosition;
				targetPosition = nullptr;
			}
			if (targetPosition == nullptr || random->nextInt(256) == 0 || targetPosition->distSqr(static_cast<int>(x), static_cast<int>(y), static_cast<int>(z)) <= 4)
			{
				delete targetPosition;
				targetPosition = new Pos(static_cast<int>(x) + random->nextInt(48) - random->nextInt(48), static_cast<int>(y) + random->nextInt(12) - random->nextInt(12), static_cast<int>(z) + random->nextInt(48) - random->nextInt(48));
			}

			float yRotD = static_cast<float>(atan2(zd, xd) * 180 / PI) - 90;
			float rotDiff = Mth::wrapDegrees(yRotD - yRot);
			yya = .5f;
			yRot += rotDiff;
			yHeadRot = yBodyRot = yRot;
			setRot(yRot, xRot);

			double dx = (targetPosition->x + .3) - x;
			double dy = (targetPosition->y + .1) - y;
			double dz = (targetPosition->z + .3) - z;

			xd = xd + (signum(dx) * .4f - xd) * .1f;
			yd = yd + (signum(dy) * .42f - yd) * .1f;
			zd = zd + (signum(dz) * .4f - zd) * .1f;

			onGround = false;
		}
		else
		{	//Why was this so annoying lol
			Vec3* viewVector = getViewVector(1.0f);

			flyX = static_cast<float>(viewVector->x) * 0.35;
			flyY = -rider.lock()->xRot * 0.005;		//This in particular...
			flyZ = static_cast<float>(viewVector->z) * 0.35;

			yRot = rider.lock()->yRot;
			xRot = rider.lock()->xRot;
			yHeadRot = yBodyRot = yRot;
			setRot(yRot, xRot);
			move(flyX, flyY, flyZ);

			onGround = true;
		}
	}
}

bool Evupul::mobInteract(shared_ptr<Player> player) 
{
	if (!level->isClientSide && (getEvupulType() == TYPE_DEFAULT || getEvupulType() == TYPE_GOLD))
	{
		player->ride( rider.lock() == player ? nullptr : shared_from_this() );
		return true;
	}
	return FlyingMob::mobInteract(player);
}

double Evupul::getRideHeight()
{
	return bbHeight * 0.0;
}

int Evupul::getEvupulType()
{
	return (int) entityData->getByte(DATA_TYPE_ID);
}

void Evupul::setEvupulType(int type)
{
	entityData->set(DATA_TYPE_ID, (byte) type);

	fireImmune = type == TYPE_GOLD;
}

MobGroupData *Evupul::finalizeMobSpawn(MobGroupData *groupData, int extraData)
{
	groupData = FlyingMob::finalizeMobSpawn(groupData);

	if (getRandom()->nextInt(7) < 1)
	{
		setEvupulType(TYPE_GOLD);
		getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(40);
		setHealth(40);
		xpReward = Enemy::XP_REWARD_LARGE;
	}
	else
	{
		setEvupulType(TYPE_DEFAULT);
	}

	return groupData;
}

/*int Evupul::getHurtSound()
{
	int type = getEvupulType();
	if (type == TYPE_GOLD)
	{
		return eSoundType_MOB_EVUPULGOLD_HURT;
	}
	return eSoundType_DAMAGE_HURT;
}

int Evupul::getDeathSound()
{
	int type = getEvupulType();
	if (type == TYPE_GOLD)
	{
		return eSoundType_MOB_EVUPULGOLD_DEATH;
	}
	return eSoundType_DAMAGE_HURT;
}

int Evupul::getAmbientSound()
{
	return eSoundType_MOB_EVUPULGOLD_AMBIENT;
}

float Evupul::getSoundVolume()
{
	int type = getEvupulType();
	if (type == TYPE_GOLD)
	{
		return 10;
	}
	return 1;
}*/

void Evupul::readAdditionalSaveData(CompoundTag *tag)
{
	FlyingMob::readAdditionalSaveData(tag);

	if (tag->contains(L"EvupulType"))
	{
		int value = tag->getByte(L"EvupulType");
		setEvupulType(value);
	}
}

void Evupul::addAdditonalSaveData(CompoundTag *entityTag)
{
	FlyingMob::addAdditonalSaveData(entityTag);
	entityTag->putByte(L"EvupulType", (byte) getEvupulType());
}
