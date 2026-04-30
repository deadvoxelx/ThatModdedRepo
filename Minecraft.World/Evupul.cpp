#include "stdafx.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.damagesource.h"
#include "net.minecraft.world.effect.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.entity.ai.goal.h"
#include "net.minecraft.world.entity.ai.goal.target.h"
#include "net.minecraft.world.entity.ai.navigation.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.entity.animal.h"
#include "net.minecraft.world.entity.player.h"
#include "Evupul.h"
#include "GenericStats.h"
#include "..\Minecraft.Client\Textures.h"
#include "net.minecraft.world.entity.h"
#include "JavaMath.h"
#include "SoundTypes.h"

//have fun with this one...

Evupul::Evupul(Level *level) : FlyingMob( level )
{
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());
	setSize(0.9f, 0.5f);

	xpReward = Enemy::XP_REWARD_MEDIUM;

	moveTargetX = 0.0;
    moveTargetY = 0.0;
    moveTargetZ = 0.0;

	targetPosition = nullptr;

	getNavigation()->setAvoidWater(true);
	goalSelector.addGoal(0, new FloatGoal(this));
	//goalSelector.addGoal(6, new RandomStrollGoal(this, 1.0));
	goalSelector.addGoal(7, new LookAtPlayerGoal(this, typeid(Player), 8));
}

void Evupul::defineSynchedData()
{
	FlyingMob::defineSynchedData();

	entityData->define(DATA_TYPE_ID, (byte) TYPE_DEFAULT);
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
	/*if (getEvupulType() == TYPE_GOLD)
	{
		return Item::evupulWingGold_Id;
	}
	else if (getEvupulType() == TYPE_DEFAULT)
	{
		return Item::evupulWing_Id;
	}*/
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

inline int signum(double x) { return (x > 0) - (x < 0); }

void Evupul::newServerAiStep() //honestly feel free to improve this or sum, ik its ass
{
	FlyingMob::newServerAiStep(); //commented out the Dark Evupul-specific flight code, as these'll never use it
	{
		//shared_ptr<LivingEntity> target = getTarget();
		//if ((target == nullptr) && (getAttackTarget() == nullptr))	//this half determines random empty tiles to move to, kinda chopped but it works
		//{															//but only for aimless flying, its ass for targeting
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
		//}
		//else	//this half forcefully moves it towards its target, also kinda chopped, but its the best i can come up with
		//{		//we take whatever wins we can, i have no idea what im doing lmfao
		//	delete targetPosition;
      	//	targetPosition = new Pos(target->x, target->y, target->z);		//idk why this was so difficult for me to figure out lol
		//}		//i got laid after making this LMAOO						//it seems to crash the game when targeting the Wither wtf
		
		double dx = (targetPosition->x + .3) - x;
		double dy = (targetPosition->y + .1) - y;
		double dz = (targetPosition->z + .3) - z;

		xd = xd + (signum(dx) * .5f - xd) * .1f;
		yd = yd + (signum(dy) * .7f - yd) * .1f;
		zd = zd + (signum(dz) * .5f - zd) * .1f;

		float yRotD = static_cast<float>(atan2(zd, xd) * 180 / PI) - 90;
		float rotDiff = Mth::wrapDegrees(yRotD - yRot);
		yya = .5f;
		yRot += rotDiff;
	}
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
		xpReward = Enemy::XP_REWARD_LARGE;
	}
	else
	{
		setEvupulType(TYPE_DEFAULT);
	}

	return groupData;
}