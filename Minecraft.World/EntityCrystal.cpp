#include "stdafx.h"
#include "com.mojang.nbt.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.damagesource.h"
#include "net.minecraft.world.effect.h"
#include "EntityCrystal.h"
#include "LightningBolt.h"
#include "Mth.h"
#include "SoundTypes.h"
#include "SunSpirit.h"
#include "FireMinion.h"

void EntityCrystal::_init()
{
	this->defineSynchedData();

	double base = 0.20000000298023224;
	smotionX = (base + random->nextFloat() * 0.15f) * (random->nextInt(2) == 0 ? 1.0 : -1.0);
	smotionY = (base + random->nextFloat() * 0.15f) * (random->nextInt(2) == 0 ? 1.0 : -1.0);
	smotionZ = (base + random->nextFloat() * 0.15f) * (random->nextInt(2) == 0 ? 1.0 : -1.0);

	type = (random->nextInt(2) == 0) ? eCrystalType_FIRE : eCrystalType_ICE;
	_wasHit = false;
	life = 0;
	shootingEntity = nullptr;
	targetEntity = nullptr;

	fireImmune = true;

	lSteps = 0;
	lx = ly = lz = 0.0;
	lyr = lxr = 0.0f;

	for (int i = 0; i < 3; i++)
	{
		sinage[i] = random->nextFloat() * 6.0f;
	}

	setSize(1.2f, 1.2f);
}

EntityCrystal::EntityCrystal(Level *level) : Entity( level )
{
	_init();
}

EntityCrystal::EntityCrystal(Level *level, double x, double y, double z, eCrystalType crystalType) : Entity( level )
{
	_init();

	type = crystalType;
	if (type == eCrystalType_ICE)
	{
		smotionX /= 3.0;
		smotionY = 0.0;
		smotionZ /= 3.0;
	}

	setPos(x, y, z);
}

EntityCrystal::EntityCrystal(Level *level, double x, double y, double z, shared_ptr<LivingEntity> target) : EntityCrystal( level, x, y, z, eCrystalType_THUNDER )
{
	targetEntity = target;
}

void EntityCrystal::defineSynchedData()
{
}

bool EntityCrystal::makeStepSound()
{
	return false;
}

float EntityCrystal::getShadowHeightOffs()
{
	return 0;
}

void EntityCrystal::addAdditonalSaveData(CompoundTag *tag)
{
	tag->putByte(L"crystalType", (byte)type);
	tag->putBoolean(L"wasHit", _wasHit);
}

void EntityCrystal::readAdditionalSaveData(CompoundTag *tag)
{
	if (tag->contains(L"crystalType"))
	{
		type = (eCrystalType)(tag->getByte(L"crystalType") & 0xff);
	}
	if (tag->contains(L"wasHit"))
	{
		_wasHit = tag->getBoolean(L"wasHit");
	}
}

bool EntityCrystal::isPickable()
{
	return true;
}

bool EntityCrystal::isAttackable()
{
	return true;
}

bool EntityCrystal::isShootable()
{
	return true;
}

void EntityCrystal::playerTouch(shared_ptr<Player> player)
{
}

eCrystalType EntityCrystal::getCrystalType()
{
	return type;
}

bool EntityCrystal::wasHit()
{
	return _wasHit;
}

void EntityCrystal::setShootingEntity(shared_ptr<Entity> owner)
{
	shootingEntity = owner;
}

void EntityCrystal::setSMotion(double x, double y, double z)
{
	smotionX = x;
	smotionY = y;
	smotionZ = z;
}

void EntityCrystal::lerpMotion(double xd, double yd, double zd)
{
	smotionX = xd;
	smotionY = yd;
	smotionZ = zd;
	this->xd = xd;
	this->yd = yd;
	this->zd = zd;
}

void EntityCrystal::lerpTo(double x, double y, double z, float yRot, float xRot, int steps)
{
	lx = x;
	ly = y;
	lz = z;
	lyr = yRot;
	lxr = xRot;
	lSteps = steps;
}

int EntityCrystal::maxTicksAlive()
{
	return (type == eCrystalType_THUNDER) ? 200 : 300;
}

void EntityCrystal::moveTowardsTarget(shared_ptr<Entity> target, double speed)
{
	if (target == nullptr || level->isClientSide) return;

	double angle1 = yRot / 57.29577951308232;
	xd -= Mth::sin(angle1) * speed;
	zd += Mth::cos(angle1) * speed;

	double a = target->y - 0.75;
	if (a < bb->y0 - 0.5)
	{
		yd -= speed / 2.0;
	}
	else if (a > bb->y0 + 0.5)
	{
		yd += speed / 2.0;
	}
	else
	{
		yd += (a - bb->y0) * speed / 2.0;
	}

	if (onGround)
	{
		onGround = false;
		yd = 0.10000000149011612;
	}
}

void EntityCrystal::tick()
{
	Entity::tick();

	life++;

	if (level->isClientSide)
	{
		if (lSteps > 0)
		{
			double xt = x + (lx - x) / lSteps;
			double yt = y + (ly - y) / lSteps;
			double zt = z + (lz - z) / lSteps;

			double yrd = lyr - yRot;
			while (yrd < -180) yrd += 360;
			while (yrd >= 180) yrd -= 360;

			yRot += static_cast<float>((yrd) / lSteps);
			xRot += static_cast<float>((lxr - xRot) / lSteps);

			lSteps--;
			setPos(xt, yt, zt);
			setRot(yRot, xRot);
		}

		if (life >= maxTicksAlive())
		{
			remove();
		}

		for (int i = 0; i < 3; i++)
		{
			sinage[i] += 0.3f + i * 0.13f;
			if (sinage[i] > 6.2831855f) sinage[i] -= 6.2831855f;
		}
		return;
	}

	xd = smotionX;
	yd = smotionY;
	zd = smotionZ;

	if (type == eCrystalType_THUNDER)
	{
		if (targetEntity == nullptr || !targetEntity->isAlive())
		{
			remove();
			return;
		}

		float yRotTo = (float)(atan2(targetEntity->z - z, targetEntity->x - x) * 180 / PI) - 90;
		yRot += Mth::wrapDegrees(yRotTo - yRot) * 0.2f;
		setRot(yRot, xRot);

		moveTowardsTarget(targetEntity, 0.02);
	}
	else if (horizontalCollision || onGround)
	{
		if (_wasHit)
		{
			explode();
			expire();
			remove();
			return;
		}

		int i = Mth::floor(x);
		int j = Mth::floor(bb->y0);
		int k = Mth::floor(z);

		xd = smotionX = -smotionX;
		if (smotionX < 0 && !level->isEmptyTile(i - 1, j, k)) xd = smotionX = -smotionX;

		yd = smotionY = -smotionY;
		if (smotionY < 0 && !level->isEmptyTile(i, j - 1, k)) yd = smotionY = -smotionY;

		zd = smotionZ = -smotionZ;
		if (smotionZ < 0 && !level->isEmptyTile(i, j, k - 1)) zd = smotionZ = -smotionZ;
	}

	move(xd, yd, zd);

	if (life >= maxTicksAlive())
	{
		if (type == eCrystalType_THUNDER)
		{
			level->addGlobalEntity(make_shared<LightningBolt>(level, x, y, z));
		}
		expire();
		remove();
		return;
	}

	vector<shared_ptr<Entity> > objects = *level->getEntities(shared_from_this(), bb->grow(0.1, 0.1, 0.1));
	for (auto& e : objects)
	{
		if (e == shared_from_this()) continue;
		onHitEntity(e);
		if (removed) break;
	}

	for (int i = 0; i < 3; i++)
	{
		sinage[i] += 0.3f + i * 0.13f;
		if (sinage[i] > 6.2831855f) sinage[i] -= 6.2831855f;
	}
}

void EntityCrystal::onHitEntity(shared_ptr<Entity> entity)
{
	if (type == eCrystalType_THUNDER)
	{
		if (entity == targetEntity)
		{
			DamageSource *damageSource = DamageSource::thrown(shared_from_this(), shootingEntity);
			bool flag = entity->hurt(damageSource, 5);
			delete damageSource;
			if (flag) moveTowardsTarget(entity, -0.3);
		}
		return;
	}

	if (entity->instanceof(eTYPE_SUN_SPIRIT) || entity->instanceof(eTYPE_APHALAF_BOSS) || entity->instanceof(eTYPE_FIRE_MINION))
	{
		if (type == eCrystalType_ICE && _wasHit && entity->instanceof(eTYPE_SUN_SPIRIT))
		{
			DamageSource *damageSource = DamageSource::thrown(shared_from_this(), shootingEntity);
			entity->hurt(damageSource, 15);
			delete damageSource;
			explode();
			expire();
			remove();
		}
		return;
	}

	if (type == eCrystalType_FIRE)
	{
		DamageSource *damageSource = DamageSource::thrown(shared_from_this(), shootingEntity);
		bool flag = entity->hurt(damageSource, 5);
		delete damageSource;
		if (flag)
		{
			explode();
			expire();
			remove();
			if (entity->instanceof(eTYPE_LIVINGENTITY))
			{
				entity->setOnFire(5);
			}
		}
	}
	else if ((type == eCrystalType_ICE || type == eCrystalType_CLOUD) && _wasHit)
	{
		DamageSource *damageSource = DamageSource::thrown(shared_from_this(), shootingEntity);
		bool flag = entity->hurt(damageSource, 5);
		delete damageSource;
		if (flag)
		{
			explode();
			expire();
			remove();
			if (entity->instanceof(eTYPE_LIVINGENTITY))
			{
				dynamic_pointer_cast<LivingEntity>(entity)->addEffect(new MobEffectInstance(MobEffect::movementSlowdown->id, 10, 0));
			}
		}
	}
}

bool EntityCrystal::hurt(DamageSource *source, float damage)
{
	shared_ptr<Entity> attacker = source->getEntity();
	if (attacker != nullptr)
	{
		if (type == eCrystalType_THUNDER)
		{
			moveTowardsTarget(attacker, -0.15 - damage / 8.0);
			return Entity::hurt(source, damage);
		}

		Vec3 *look = attacker->getLookAngle();
		if (look != nullptr)
		{
			smotionX = look->x;
			smotionZ = look->z;
			if (type != eCrystalType_ICE)
			{
				smotionY = look->y;
			}
		}

		shootingEntity = source->getDirectEntity();
		_wasHit = true;
		return true;
	}
	return false;
}

void EntityCrystal::explode()
{
	playSound(eSoundType_RANDOM_EXPLODE, 2.0f, random->nextFloat() - random->nextFloat() * 0.2f + 1.2f);
	if (level->isClientSide) return;

	double motionMultiplier = 0.5;
	if (type == eCrystalType_ICE || type == eCrystalType_CLOUD) motionMultiplier *= 0.5;

	ePARTICLE_TYPE particle = eParticleType_largeexplode;
	switch (type)
	{
	case eCrystalType_ICE:
	case eCrystalType_CLOUD:
		particle = eParticleType_snowshovel;
		break;
	case eCrystalType_THUNDER:
		particle = eParticleType_hugeexplosion;
		break;
	default:
		break;
	}

	level->addParticle(particle, x, y, z, 0.0, 0.0, 0.0);
	for (int i = 0; i < 40; i++)
	{
		level->addParticle(particle, x, y, z, (random->nextFloat() - 0.5) * motionMultiplier, (random->nextFloat() - 0.5) * motionMultiplier, (random->nextFloat() - 0.5) * motionMultiplier);
	}
}

void EntityCrystal::expire()
{
	eSOUND_TYPE sound = eSoundType_RANDOM_POP;
	ePARTICLE_TYPE particle = eParticleType_flame;
	switch (type)
	{
	case eCrystalType_FIRE:
		sound = eSoundType_FIRE_FIRE;
		particle = eParticleType_flame;
		break;
	case eCrystalType_ICE:
	case eCrystalType_CLOUD:
		sound = eSoundType_RANDOM_GLASS;
		particle = eParticleType_snowballpoof;
		break;
	case eCrystalType_THUNDER:
		sound = eSoundType_AMBIENT_WEATHER_THUNDER;
		particle = eParticleType_hugeexplosion;
		break;
	default:
		break;
	}

	playSound(sound, 2.0f, random->nextFloat() - random->nextFloat() * 0.2f + 1.2f);
	if (level->isClientSide) return;

	level->addParticle(particle, x, bb->y0 + bbHeight * 0.8, z, 0.0, 0.0, 0.0);
	for (int i = 0; i < 16; i++)
	{
		level->addParticle(particle, x, bb->y0 + bbHeight * 0.8, z, (random->nextFloat() - 0.5) * 0.25, (random->nextFloat() - 0.5) * 0.25, (random->nextFloat() - 0.5) * 0.25);
	}
}
