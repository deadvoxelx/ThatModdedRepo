#include "stdafx.h"
#include "com.mojang.nbt.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.entity.ai.goal.target.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.entity.npc.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.effect.h"
#include "net.minecraft.world.item.enchantment.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.damagesource.h"
#include "Zombice.h"
#include "SharedConstants.h"
#include "..\Minecraft.Client\Textures.h"
#include "SoundTypes.h"

void Zombice::_init()
{
	registerAttributes();
}

Zombice::Zombice(Level *level) : Zombie( level )
{
	_init();

	targetSelector.addGoal(1, new HurtByTargetGoal(this, true));
	targetSelector.addGoal(2, new NearestAttackableTargetGoal(this, typeid(Player), 0, true));
	targetSelector.addGoal(2, new NearestAttackableTargetGoal(this, typeid(Villager), 0, false));

	fireImmune = false;
}

void Zombice::registerAttributes()
{
	Zombie::registerAttributes();

	getAttribute(SPAWN_REINFORCEMENTS_CHANCE)->setBaseValue(0);
	getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.5f);
	getAttribute(SharedMonsterAttributes::ATTACK_DAMAGE)->setBaseValue(3);
}

bool Zombice::useNewAi()
{
	return false;
}

void Zombice::tick()
{
	Zombie::tick();
}

bool Zombice::canSpawn()
{
	return level->difficulty > Difficulty::PEACEFUL && level->isUnobstructed(bb) && level->getCubes(shared_from_this(), bb)->empty() && !level->containsAnyLiquid(bb);
}

shared_ptr<Entity> Zombice::findAttackTarget()
{
#ifndef _FINAL_BUILD
#ifdef _DEBUG_MENUS_ENABLED
	if(app.GetMobsDontAttackEnabled())
	{
		return shared_ptr<Player>();
	}
#endif
#endif

	return Zombie::findAttackTarget();
}

bool Zombice::hurt(DamageSource *source, float dmg)
{
	return Zombie::hurt(source, dmg);
}

int Zombice::getAmbientSound()
{
	return eSoundType_MOB_ZOMBIE_AMBIENT;
}

int Zombice::getHurtSound()
{
	return eSoundType_MOB_ZOMBIE_HURT;
}

int Zombice::getDeathSound()
{
	return eSoundType_MOB_ZOMBIE_DEATH;
}

void Zombice::dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel)
{
	int count = random->nextInt(2 + playerBonusLevel);
	for (int i = 0; i < count; i++)
	{
		spawnAtLocation(Item::rotten_flesh_Id, 2);
	}
}

bool Zombice::mobInteract(shared_ptr<Player> player)
{
	return false;
}

void Zombice::dropRareDeathLoot(int rareLootLevel)
{
	switch (random->nextInt(3))
	{
	case 0:
		spawnAtLocation(Item::ironIngot_Id, 1);
		break;
	case 1:
		spawnAtLocation(Item::carrots_Id, 1);
		break;
	case 2:
		spawnAtLocation(Item::potato_Id, 1);
		break;
	}
}

int Zombice::getDeathLoot()
{
	return Item::rotten_flesh_Id;
}

MobGroupData *Zombice::finalizeMobSpawn(MobGroupData *groupData, int extraData /*= 0*/) // 4J Added extraData param
{
	Zombie::finalizeMobSpawn(groupData);
	setVillager(false);
	return groupData;
}

bool Zombice::doHurtTarget(shared_ptr<Entity> target)
{
	if (Zombie::doHurtTarget(target))
	{
		if ( target->instanceof(eTYPE_LIVINGENTITY) )
		{
			dynamic_pointer_cast<LivingEntity>(target)->addEffect(new MobEffectInstance(MobEffect::movementSlowdown->id, 300, 0));
		}

		return true;
	}
	return false;
}

void Zombice::populateDefaultEquipmentSlots()
{
	Monster::populateDefaultEquipmentSlots();

	if (random->nextFloat() < (level->difficulty == Difficulty::HARD ? 0.05f : 0.01f))
	{
		int rand = random->nextInt(3);
		if (rand == 0)
		{
			setEquippedSlot(SLOT_WEAPON, std::make_shared<ItemInstance>(Item::sword_iron));
		}
		else
		{
			setEquippedSlot(SLOT_WEAPON, std::make_shared<ItemInstance>(Item::sword_stone));
		}
	}
}

void Zombice::aiStep()
{
	if (level->isDay() && !level->isClientSide)
	{
		float br = getBrightness(1);
		if (br > 0.5f && random->nextFloat() * 30 < (br - 0.4f) * 2 && level->canSeeSky(Mth::floor(x), static_cast<int>(floor(y + 0.5)), Mth::floor(z)))
		{
			bool burn = false;

			if (burn)
			{
				setOnFire(8);
			}
		}
	}
	Monster::aiStep();
}

int Zombice::decreaseAirSupply(int currentSupply)
{
	// infinite air supply
	return currentSupply;
}
