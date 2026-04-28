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
#include "net.minecraft.world.entity.player.h"
#include "EvupulDark.h"
#include "GenericStats.h"
#include "..\Minecraft.Client\Textures.h"
#include "net.minecraft.world.entity.h"
#include "JavaMath.h"
#include "SoundTypes.h"

//have fun with this one...

EvupulDark::EvupulDark(Level *level) : FlyingMonster( level )
{
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());
	setSize(0.9f, 0.5f);

	moveTargetX = 0.0;
    moveTargetY = 0.0;
    moveTargetZ = 0.0;

	targetPosition = nullptr;

	getNavigation()->setAvoidWater(true);
	goalSelector.addGoal(0, new FloatGoal(this));
	goalSelector.addGoal(2, new LeapAtTargetGoal(this, 0.4));
	goalSelector.addGoal(3, new MeleeAttackGoal(this, eTYPE_PLAYER, 1.0, false));
	goalSelector.addGoal(6, new RandomStrollGoal(this, 1.0));
	goalSelector.addGoal(7, new LookAtPlayerGoal(this, typeid(Player), 8));

	targetSelector.addGoal(1, new HurtByTargetGoal(this, true));
	targetSelector.addGoal(2, new NearestAttackableTargetGoal(this, typeid(Player), 0, true));
}

void EvupulDark::registerAttributes()
{
	FlyingMonster::registerAttributes();
	
	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(30);
	getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.05f);
	getAttribute(SharedMonsterAttributes::ATTACK_DAMAGE)->setBaseValue(6);
}

bool EvupulDark::useNewAi()
{
	return true;
}

void EvupulDark::aiStep()
{
	FlyingMonster::aiStep();
}

bool EvupulDark::hurt(DamageSource *source, float dmg)
{
	if (FlyingMonster::hurt(source, dmg))
	{
		shared_ptr<LivingEntity> target = getTarget();
		if ( (target == nullptr) && getAttackTarget() != nullptr && getAttackTarget()->instanceof(eTYPE_LIVINGENTITY) )	target = dynamic_pointer_cast<LivingEntity>( getAttackTarget() );
		if ( (target == nullptr) && source->getEntity() != nullptr && source->getEntity()->instanceof(eTYPE_LIVINGENTITY) )	target = dynamic_pointer_cast<LivingEntity>( source->getEntity() );

		return true;
	}

	return false;
}

void EvupulDark::tick()
{
	FlyingMonster::tick();
}

bool EvupulDark::doHurtTarget(shared_ptr<Entity> target)
{
	if (FlyingMonster::doHurtTarget(target))
	{
		if ( target->instanceof(eTYPE_LIVINGENTITY) )
		{	//bc yes (:
			dynamic_pointer_cast<LivingEntity>(target)->addEffect(new MobEffectInstance(MobEffect::blindness->id, 200, 0));
		}	//no regrets

		return true;
	}
	return false;
}   

int EvupulDark::getDeathLoot()
{
	return 0;
}

bool EvupulDark::makeStepSound()
{
	return false;
}

bool EvupulDark::removeWhenFarAway()
{
	return true;
}

void EvupulDark::causeFallDamage(float distance)
{
}

void EvupulDark::checkFallDamage(double ya, bool onGround)
{
}

inline int signum(double x) { return (x > 0) - (x < 0); }

void EvupulDark::newServerAiStep() //honestly feel free to improve this or sum, ik its ass
{
	FlyingMonster::newServerAiStep();
	{
		shared_ptr<LivingEntity> target = getTarget();
		if ((target == nullptr) && (getAttackTarget() == nullptr))	//this half determines random empty tiles to move to, kinda chopped but it works
		{															//but only for aimless flying, its ass for targeting
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
		else	//this half forcefully moves it towards its target, also kinda chopped, but its the best i can come up with
		{		//we take whatever wins we can, i have no idea what im doing lmfao
			delete targetPosition;
			targetPosition = new Pos(target->x, target->y, target->z);		//idk why this was so difficult for me to figure out lol
		}		//i got laid after making this LMAOO
		
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