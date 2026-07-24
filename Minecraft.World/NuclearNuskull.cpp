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
#include "NuclearNuskull.h"
#include "GenericStats.h"
#include "JavaMath.h"
#include "SoundTypes.h"
#include "..\Minecraft.Client\Textures.h"

NuclearNuskull::NuclearNuskull(Level *level) : FlyingMonster( level )
{
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());
	setSize(0.7f, 0.7f);

	moveTargetX = 0.0;
    moveTargetY = 0.0;
    moveTargetZ = 0.0;

	targetPosition = nullptr;

	fuse = 0;

	getNavigation()->setAvoidWater(true);
	goalSelector.addGoal(0, new FloatGoal(this));
	goalSelector.addGoal(2, new LeapAtTargetGoal(this, 0.4));
	goalSelector.addGoal(3, new MeleeAttackGoal(this, 1.0, false));
	goalSelector.addGoal(6, new RandomStrollGoal(this, 1.0));

	targetSelector.addGoal(1, new HurtByTargetGoal(this, true));
	targetSelector.addGoal(2, new NearestAttackableTargetGoal(this, typeid(Player), 0, true));
}

int NuclearNuskull::getFuseTick()
{
	return fuse;
}

void NuclearNuskull::registerAttributes()
{
	FlyingMonster::registerAttributes();
	
	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(30);
	getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.05f);
	getAttribute(SharedMonsterAttributes::ATTACK_DAMAGE)->setBaseValue(8);
	getAttribute(SharedMonsterAttributes::KNOCKBACK_RESISTANCE)->setBaseValue(1.0);
}

bool NuclearNuskull::useNewAi()
{
	return true;
}

void NuclearNuskull::aiStep()
{
	FlyingMonster::aiStep();
}

void NuclearNuskull::tick()
{
	FlyingMonster::tick();

	if (fuse > 0)
	{
		--fuse;

		getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.0f);
	}

	if (fuse <= 0 && getHealth() < getMaxHealth())
	{
		if (!level->isClientSide)
		{
			float r = 15.0f;
			level->explode(shared_from_this(), x, y, z, r, true);
			remove();
		}
	}
} 

int NuclearNuskull::getDeathLoot()
{
	return 0;
}

bool NuclearNuskull::makeStepSound()
{
	return false;
}

bool NuclearNuskull::removeWhenFarAway()
{
	return true;
}

void NuclearNuskull::causeFallDamage(float distance)
{
}

void NuclearNuskull::checkFallDamage(double ya, bool onGround)
{
}

inline int signum(double x) { return (x > 0) - (x < 0); }

void NuclearNuskull::newServerAiStep()
{
	FlyingMonster::newServerAiStep();
	{
		shared_ptr<LivingEntity> target = getTarget();
		if (fuse <= 0)
		{
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
				delete targetPosition;
      			targetPosition = new Pos(target->x, target->y, target->z);
			}

			double dx = (targetPosition->x + .3) - x;
			double dy = (targetPosition->y + .3) - y;
			double dz = (targetPosition->z + .3) - z;

			xd = xd + (signum(dx) * .3f - xd) * .1f;
			yd = yd + (signum(dy) * .32f - yd) * .1f;
			zd = zd + (signum(dz) * .3f - zd) * .1f;

			float yRotD = static_cast<float>(atan2(zd, xd) * 180 / PI) - 90;
			float rotDiff = Mth::wrapDegrees(yRotD - yRot);
			yya = .5f;
			yRot += rotDiff;
		}
		else
		{
			delete targetPosition;
			targetPosition = nullptr;
		}
	}
}

bool NuclearNuskull::hurt(DamageSource *source, float dmg)
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

	if (fuse > 0) return false;

	if (!level->isClientSide)
	{
		fuse = 40;
		playSound(eSoundType_RANDOM_FUSE, 1, 1.0f);
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
