#include "stdafx.h"
#include "com.mojang.nbt.h"
#include "net.minecraft.stats.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.item.enchantment.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.entity.ai.goal.h"
#include "net.minecraft.world.entity.ai.navigation.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.global.h"
#include "net.minecraft.network.packet.h"
#include "Pigman.h"
#include "..\Minecraft.Client\Textures.h"
#include "..\Minecraft.Client\ServerPlayer.h"
#include "..\Minecraft.Client\PlayerConnection.h"
#include "ItemInstance.h"
#include "EnchantmentHelper.h"
#include "WeighedTreasure.h"
#include "MobCategory.h"

Pigman::Pigman(Level *level) : Animal( level )
{
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());

	tradeTimer = 0;

	setSize(0.6f, 1.8f);

	getNavigation()->setAvoidWater(true);
	goalSelector.addGoal(0, new FloatGoal(this));
	goalSelector.addGoal(1, new PanicGoal(this, 1.25));
	goalSelector.addGoal(2, new MoveIndoorsGoal(this));
	goalSelector.addGoal(3, new RestrictOpenDoorGoal(this));
	goalSelector.addGoal(4, new OpenDoorGoal(this, true));
	goalSelector.addGoal(5, new MoveTowardsRestrictionGoal(this, 1.0));
	goalSelector.addGoal(6, new RandomStrollGoal(this, 1.0));
	goalSelector.addGoal(7, new LookAtPlayerGoal(this, typeid(Player), 6));
	goalSelector.addGoal(8, new RandomLookAroundGoal(this));
}

bool Pigman::useNewAi()
{
	return true;
}

void Pigman::registerAttributes()
{
	Animal::registerAttributes();

	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(20);
	getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.25f);
}

void Pigman::newServerAiStep()
{
	Animal::newServerAiStep();

	if (!level->isClientSide && tradeTimer > 0)
	{
		if (--tradeTimer <= 0)
		{
			finishTradeOffer();
		}
	}
}

bool Pigman::mobInteract(shared_ptr<Player> player)
{
	shared_ptr<ItemInstance> item = player->inventory->getSelected();

	if (item != nullptr && item->id == Item::goldIngot_Id && tradeTimer <= 0)
	{
		player->swing();

		if (!level->isClientSide)
		{
			if (!player->abilities.instabuild)
			{
				item->count--;
			}

			tradeTimer = GOLD_INGOT_HOLD_TICKS;
			playSound(eSoundType_MOB_PIG_AMBIENT, 1.0f, 1.0f);
			setEquippedSlot(SLOT_WEAPON, std::make_shared<ItemInstance>(Item::goldIngot_Id, 1, 0));
		}

		return true;
	}

	return Animal::mobInteract(player);
}

void Pigman::finishTradeOffer()
{
	setEquippedSlot(SLOT_WEAPON, nullptr);

	WeighedTreasureArray treasureArray(pigmanLoot, TREASURE_ITEMS_COUNT);
	WeighedTreasure *treasure = static_cast<WeighedTreasure *>(WeighedRandom::getRandomItem(random, *((WeighedRandomItemArray *)&treasureArray)));

	if (treasure != nullptr)
	{
		int count = treasure->getMinCount() + random->nextInt(treasure->getMaxCount() - treasure->getMinCount() + 1);
		shared_ptr<ItemInstance> copy = treasure->getItem()->copy();
		copy->count = count;

		if (copy->id == Item::enchantedBook_Id)
		{
			Enchantment *enchantment = Enchantment::validEnchantments[random->nextInt(Enchantment::validEnchantments.size())];
			int level = Mth::nextInt(random, enchantment->getMinLevel(), enchantment->getMaxLevel());
			Item::enchantedBook->addEnchantment(copy, new EnchantmentInstance(enchantment, level));
		}
		else if (copy->id == Item::bow_Id || copy->id == Item::nethaniumChestplate_Id)
		{
			EnchantmentHelper::enchantItem(random, copy, 30);
		}

		playSound(eSoundType_MOB_PIG_AMBIENT, 1.0f, 1.0f);
		spawnAtLocation(copy, 0);
	}
}

void Pigman::readAdditionalSaveData(CompoundTag *tag)
{
	Animal::readAdditionalSaveData(tag);

	shared_ptr<ItemInstance> carried = getCarriedItem();
	if (carried != nullptr && carried->id == Item::goldIngot_Id)
	{
		setEquippedSlot(SLOT_WEAPON, nullptr);
	}
}

int Pigman::getAmbientSound() 
{
	return eSoundType_MOB_PIG_AMBIENT;
}

int Pigman::getHurtSound() 
{
	return eSoundType_MOB_PIG_AMBIENT;
}

int Pigman::getDeathSound() 
{
	return eSoundType_MOB_PIG_DEATH;
}

void Pigman::playStepSound(int xt, int yt, int zt, int t)
{
	playSound(eSoundType_MOB_PIG_STEP, 0.15f, 1);
}

int Pigman::getDeathLoot() 
{
	if (this->isOnFire() ) return Item::porkChop_cooked->id;
	return Item::porkChop_raw_Id;
}

void Pigman::dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel)
{
	int count = random->nextInt(3) + 1 + random->nextInt(1 + playerBonusLevel);

	for (int i = 0; i < count; i++)
	{
		if (isOnFire())
		{
			spawnAtLocation(Item::porkChop_cooked_Id, 1);
		}
		else
		{
			spawnAtLocation(Item::porkChop_raw_Id, 1);
		}
	}
}

void Pigman::thunderHit(const LightningBolt *lightningBolt)
{
	if (level->isClientSide) return;
	shared_ptr<PigZombie> pz = std::make_shared<PigZombie>(level);
	pz->moveTo(x, y, z, yRot, xRot);
	level->addEntity(pz);
	remove();
}

shared_ptr<AgableMob> Pigman::getBreedOffspring(shared_ptr<AgableMob> target)
{
	if( level->canCreateMore( GetType(), Level::eSpawnType_Breed) )
	{
		return std::make_shared<Pigman>(level);
	}
	else
	{
		return nullptr;
	}
}

bool Pigman::removeWhenFarAway()
{
	return false;
}

WeighedTreasure *Pigman::pigmanLoot[Pigman::TREASURE_ITEMS_COUNT] = 
{
	new WeighedTreasure(Item::netherBread_Id, 0, 7, 12, 20),
	new WeighedTreasure(Item::carrotGolden_Id, 0, 7, 12, 20),
	new WeighedTreasure(Item::magmaCream_Id, 0, 7, 11, 20),
	new WeighedTreasure(Item::ghastTear_Id, 0, 3, 7, 20),
	new WeighedTreasure(Item::gunpowder_Id, 0, 6, 10, 20),
	new WeighedTreasure(Tile::goldenclin_Id, 0, 12, 24, 15),
	new WeighedTreasure(Tile::obsidian_Id, 0, 1, 3, 10),
	new WeighedTreasure(Item::enderPearl_Id, 0, 1, 1, 5),
	new WeighedTreasure(Item::hellSphere_Id, 0, 3, 5, 5),
	new WeighedTreasure(Item::fireFossil_Id, 0, 1, 1, 5),
	new WeighedTreasure(Item::bow_Id, 0, 1, 1, 5),
	new WeighedTreasure(Item::enchantedBook_Id, 0, 1, 1, 5),
	new WeighedTreasure(Item::nethaniumChestplate_Id, 0, 1, 1, 5),
};
