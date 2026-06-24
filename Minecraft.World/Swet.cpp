#include "stdafx.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.storage.h"
#include "net.minecraft.world.level.chunk.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.damagesource.h"
#include "com.mojang.nbt.h"
#include "Swet.h"
#include "..\Minecraft.Client\Textures.h"
#include "SoundTypes.h"

void Swet::_init()
{
	jumpDelay = 0;

	targetSquish = 0;
	squish = 0;
	oSquish = 0;
}

Swet::Swet(Level *level) : Mob( level )
{
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());

	_init();

	int size = 2;
	heightOffset = 0;
	jumpDelay = random->nextInt(20) + 10;
	setSize(size);
}

double Swet::getRideHeight()
{
	return bbHeight * 0.0 - 0.3f;
}

bool Swet::mobInteract(shared_ptr<Player> player) 
{
	if (getSwetType() == TYPE_GOLD)
	{
		if (!level->isClientSide) remove();
	}
	if (getSwetType() == TYPE_BLUE)
	{
		player->ride( rider.lock() == player ? nullptr : shared_from_this() );
		return true;
	}
	return Mob::mobInteract(player);
}

int Swet::getSwetType()
{
	return (int) entityData->getByte(DATA_TYPE_ID);
}

void Swet::setSwetType(int type)
{
	entityData->set(DATA_TYPE_ID, (byte) type);
}

void Swet::defineSynchedData()
{
	Mob::defineSynchedData();

	entityData->define(DATA_TYPE_ID, static_cast<byte>(0));
	entityData->define(ID_SIZE, static_cast<byte>(1));
}

void Swet::setSize(int size)
{
	entityData->set(ID_SIZE, static_cast<byte>(size));
	setSize(0.6f * size, 0.6f * size);
	setPos(x, y, z);
	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(16);
	setHealth(getMaxHealth());
	xpReward = size;
}

int Swet::getSize()
{
	return entityData->getByte(ID_SIZE);
}

void Swet::addAdditonalSaveData(CompoundTag *tag)
{
	Mob::addAdditonalSaveData(tag);
	tag->putByte(L"SwetType", (byte) getSwetType());
	tag->putInt(L"Size", getSize() - 1);
}

void Swet::readAdditionalSaveData(CompoundTag *tag)
{
	Mob::readAdditionalSaveData(tag);
	if (tag->contains(L"SwetType"))
	{
		int value = tag->getByte(L"SwetType");
		setSwetType(value);
	}
	setSize(tag->getInt(L"Size") + 1);
}

ePARTICLE_TYPE Swet::getParticleName()
{
	return eParticleType_slime;
}

int Swet::getSquishSound()
{
	return getSize() > 1 ? eSoundType_MOB_SLIME_BIG : eSoundType_MOB_SLIME_SMALL;
}

void Swet::tick()
{
	if (!level->isClientSide && level->difficulty == Difficulty::PEACEFUL && getSize() > 0)
	{
		removed = true;
	}

	squish = squish + (targetSquish - squish) * .5f;

	oSquish = squish;
	bool wasOnGround = onGround;
	Mob::tick();
	if (onGround && !wasOnGround)
	{
		int size = getSize();
		for (int i = 0; i < size * 8; i++)
		{
			float dir = random->nextFloat() * PI * 2;
			float d = random->nextFloat() * 0.5f + 0.5f;
			float xd = Mth::sin(dir) * size * 0.5f * d;
			float zd = Mth::cos(dir) * size * 0.5f * d;
			level->addParticle(getParticleName(), x + xd, bb->y0, z + zd, 0, 0, 0);
		}

		if (doPlayLandSound()) 
		{
			playSound(getSquishSound(), getSoundVolume(), ((random->nextFloat() - random->nextFloat()) * 0.2f + 1.0f) / 0.8f);
		}
		targetSquish = -0.5f;
	}
	else if (!onGround && wasOnGround)
	{
		targetSquish = 1;
	}
	decreaseSquish();

	if (level->isClientSide)
	{
		int size = getSize();
		setSize(0.6f * size, 0.6f * size);
	}
}

void Swet::serverAiStep() 
{
	checkDespawn();
	shared_ptr<Player> player = level->getNearestAttackablePlayer(shared_from_this(), 16);
	if (player != nullptr)
	{
		lookAt(player, 10, 20);
	}
	if (onGround && jumpDelay-- <= 0) 
	{
		jumpDelay = getJumpDelay();
		if (player != nullptr)
		{
			jumpDelay /= 3;
		}
		jumping = true;
		if (doPlayJumpSound())
		{
			playSound(getSquishSound(), getSoundVolume(), ((random->nextFloat() - random->nextFloat()) * 0.2f + 1.0f) * 0.8f);
		}

		xxa = 1 - random->nextFloat() * 2;
		yya = static_cast<float>(1) * getSize();
	} 
	else
	{
		jumping = false;
		if (onGround)
		{
			xxa = yya = 0;
		}
	}

	if (dimension == 3 && y < 10)
	{
		remove();
	}
}

void Swet::decreaseSquish()
{
	targetSquish = targetSquish * 0.6f;
}

int Swet::getJumpDelay()
{
	return random->nextInt(20) + 10;
}

void Swet::playerTouch(shared_ptr<Player> player)
{
	if (isDealsDamage())
	{
		int size = getSize();
		if (canSee(player) && distanceToSqr(player) < (0.6 * size) * (0.6 * size))
		{
			if (getSwetType() == TYPE_BLUE)
			{
				player->ride( shared_from_this() );
			}
			DamageSource *damageSource = DamageSource::mobAttack( dynamic_pointer_cast<Mob>( shared_from_this() ) );
			if (player->hurt(damageSource, getAttackDamage()))
			{
				playSound(eSoundType_MOB_SLIME_ATTACK, 1, (random->nextFloat() - random->nextFloat()) * 0.2f + 1.0f);
			}
			delete damageSource;
		}
	}
}

bool Swet::isDealsDamage()
{
	return true;
}

int Swet::getAttackDamage()
{
	return 2;
}

int Swet::getHurtSound() 
{
	return eSoundType_MOB_SLIME_BIG;
}

int Swet::getDeathSound()
{
	return eSoundType_MOB_SLIME_BIG;
}

int Swet::getDeathLoot()
{
	return Item::swetBall->id;
}

void Swet::dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel)
{
	int count = 0 + Mth::nextInt(level->random, 1, 2) + random->nextInt(1 + playerBonusLevel);
	for (int i = 0; i < count; i++)
	{
		spawnAtLocation(Item::swetBall_Id, 1);
	}
}

bool Swet::canSpawn()
{
	if (getSize() == 2 && level->difficulty > Difficulty::PEACEFUL)
	{
		return Mob::canSpawn();
	}
	return false;
}

float Swet::getSoundVolume() 
{
	return 0.4f * getSize();
}

int Swet::getMaxHeadXRot()
{
	return 0;
}

bool Swet::doPlayJumpSound()
{
	return getSize() > 0;
}

bool Swet::doPlayLandSound()
{
	return getSize() > 2;
}

MobGroupData *Swet::finalizeMobSpawn(MobGroupData *groupData, int extraData)
{
	groupData = Mob::finalizeMobSpawn(groupData);

	if (getRandom()->nextInt(4) >= 1)
	{
		setSwetType(TYPE_BLUE);
	}
	else
	{
		setSwetType(TYPE_GOLD);
	}
	return groupData;
}
