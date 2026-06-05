#include "stdafx.h"
#include "com.mojang.nbt.h"
#include "net.minecraft.world.phys.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.item.crafting.h"
#include "net.minecraft.world.inventory.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.global.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.entity.ai.goal.h"
#include "net.minecraft.world.entity.ai.navigation.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "MobCategory.h"
#include "GenericStats.h"
#include "Sheepuff.h"
#include "..\Minecraft.Client\Textures.h"

const float Sheepuff::COLOR[Sheepuff::COLOR_LENGTH][3] =
{
	{ 1.0f, 1.0f, 1.0f }, // white
	{ 0.85f, 0.5f, 0.2f }, // orange
	{ 0.7f, 0.3f, 0.85f }, // magenta
	{ 0.4f, 0.6f, 0.85f }, // light blue
	{ 0.9f, 0.9f, 0.2f }, // yellow
	{ 0.5f, 0.8f, 0.1f }, // light green
	{ 0.95f, 0.5f, 0.65f }, // pink
	{ 0.3f, 0.3f, 0.3f }, // gray
	{ 0.6f, 0.6f, 0.6f }, // silver
	{ 0.3f, 0.5f, 0.65f }, // cyan
	{ 0.5f, 0.25f, 0.7f }, // purple
	{ 0.2f, 0.3f, 0.7f }, // blue
	{ 0.4f, 0.3f, 0.2f }, // brown
	{ 0.4f, 0.5f, 0.2f }, // green
	{ 0.6f, 0.2f, 0.2f }, // red
	{ 0.1f, 0.1f, 0.1f }, // black
};

Sheepuff::Sheepuff(Level *level) : Animal( level )
{
	this->defineSynchedData();
	registerAttributes();
	setHealth(getMaxHealth());

	setSize(0.9f, 1.3f);

	eatAnimationTick = 0;

	eatTileGoal = new EatTileGoal(this);

	getNavigation()->setAvoidWater(true);
	goalSelector.addGoal(0, new FloatGoal(this));
	goalSelector.addGoal(1, new PanicGoal(this, 1.25));
	goalSelector.addGoal(2, new BreedGoal(this, 1.0));
	goalSelector.addGoal(3, new TemptGoal(this, 1.1, Item::wheat_Id, false));
	goalSelector.addGoal(4, new FollowParentGoal(this, 1.1));
	goalSelector.addGoal(5, eatTileGoal, false);
	goalSelector.addGoal(6, new RandomStrollGoal(this, 1.0));
	goalSelector.addGoal(7, new LookAtPlayerGoal(this, typeid(Player), 6));
	goalSelector.addGoal(8, new RandomLookAroundGoal(this));

	container = std::make_shared<CraftingContainer>(new SheepuffContainer(), 2, 1);
	container->setItem(0, std::make_shared<ItemInstance>(Item::dye_powder, 1, 0));
	container->setItem(1, std::make_shared<ItemInstance>(Item::dye_powder, 1, 0));
}

bool Sheepuff::useNewAi()
{
	return true;
}

void Sheepuff::newServerAiStep()
{
	eatAnimationTick = eatTileGoal->getEatAnimationTick();
	Animal::newServerAiStep();
}

void Sheepuff::aiStep()
{
	if (level->isClientSide) eatAnimationTick = max(0, eatAnimationTick - 1);
	Animal::aiStep();
}

void Sheepuff::registerAttributes()
{
	Animal::registerAttributes();

	getAttribute(SharedMonsterAttributes::MAX_HEALTH)->setBaseValue(8);
	getAttribute(SharedMonsterAttributes::MOVEMENT_SPEED)->setBaseValue(0.23f);
}

void Sheepuff::defineSynchedData() 
{
	Animal::defineSynchedData();

	entityData->define(DATA_WOOL_ID, static_cast<byte>(0));
}

void Sheepuff::dropDeathLoot(bool wasKilledByPlayer, int playerBonusLevel)
{
	if(!isSheared())
	{
		spawnAtLocation(std::make_shared<ItemInstance>(Tile::wool_Id, 1, getColor()), 0.0f);
	}
}

int Sheepuff::getDeathLoot()
{
	return Tile::wool_Id;
}

void Sheepuff::handleEntityEvent(byte id)
{
	if (id == EntityEvent::EAT_GRASS)
	{
		eatAnimationTick = EAT_ANIMATION_TICKS;
	}
	else
	{
		Animal::handleEntityEvent(id);
	}
}

float Sheepuff::getHeadEatPositionScale(float a)
{
	if (eatAnimationTick <= 0)
	{
		return 0;
	}
	if (eatAnimationTick >= 4 && eatAnimationTick <= (EAT_ANIMATION_TICKS - 4))
	{
		return 1;
	}
	if (eatAnimationTick < 4)
	{
		return (static_cast<float>(eatAnimationTick) - a) / 4.0f;
	}
	return -(static_cast<float>(eatAnimationTick - EAT_ANIMATION_TICKS) - a) / 4.0f;
}

float Sheepuff::getHeadEatAngleScale(float a)
{
	if (eatAnimationTick > 4 && eatAnimationTick <= (EAT_ANIMATION_TICKS - 4))
	{
		float scale = (static_cast<float>(eatAnimationTick - 4) - a) / static_cast<float>(EAT_ANIMATION_TICKS - 8);
		return PI * .20f + PI * .07f * Mth::sin(scale * 28.7f);
	}
	if (eatAnimationTick > 0)
	{
		return PI * .20f;
	}
	return ((xRot / (180.0f / PI)));
}

bool Sheepuff::mobInteract(shared_ptr<Player> player)
{
	shared_ptr<ItemInstance> item = player->inventory->getSelected();

	if (!player->isAllowedToInteract( shared_from_this() ))
		return false;

	if (item != nullptr && item->id == Item::shears->id && !isSheared() && !isBaby())
	{
		if (!level->isClientSide)
		{
			setSheared(true);
			int count = 1 + random->nextInt(3);
			for (int i = 0; i < count; i++)
			{
				shared_ptr<ItemEntity> ie = spawnAtLocation(std::make_shared<ItemInstance>(Tile::wool_Id, 1, getColor()), 1.0f);
				ie->yd += random->nextFloat() * 0.05f;
				ie->xd += (random->nextFloat() - random->nextFloat()) * 0.1f;
				ie->zd += (random->nextFloat() - random->nextFloat()) * 0.1f;
			}
		}
		item->hurtAndBreak(1, player);
		playSound(eSoundType_MOB_SHEEP_SHEAR, 1, 1);
	}

	return Animal::mobInteract(player);
}

void Sheepuff::addAdditonalSaveData(CompoundTag *tag) 
{
	Animal::addAdditonalSaveData(tag);
	tag->putBoolean(L"Sheared", isSheared());
	tag->putByte(L"Color", static_cast<byte>(getColor()));
}

void Sheepuff::readAdditionalSaveData(CompoundTag *tag) 
{
	Animal::readAdditionalSaveData(tag);
	setSheared(tag->getBoolean(L"Sheared"));
	setColor((int) tag->getByte(L"Color"));
}

int Sheepuff::getAmbientSound() 
{
	return eSoundType_MOB_SHEEP_AMBIENT;
}

int Sheepuff::getHurtSound() 
{
	return eSoundType_MOB_SHEEP_AMBIENT;
}

int Sheepuff::getDeathSound() 
{
	return eSoundType_MOB_SHEEP_AMBIENT;
}

void Sheepuff::playStepSound(int xt, int yt, int zt, int t)
{
	playSound(eSoundType_MOB_SHEEP_STEP, 0.15f, 1);
}

int Sheepuff::getColor() 
{
	return (entityData->getByte(DATA_WOOL_ID) & 0x0f);
}

void Sheepuff::setColor(int color) 
{
	byte current = entityData->getByte(DATA_WOOL_ID);
	entityData->set(DATA_WOOL_ID, static_cast<byte>((current & 0xf0) | (color & 0x0f)));
}

bool Sheepuff::isSheared() 
{
	return (entityData->getByte(DATA_WOOL_ID) & 0x10) != 0;
}

void Sheepuff::setSheared(bool value) 
{
	byte current = entityData->getByte(DATA_WOOL_ID);
	if (value) 
	{
		entityData->set(DATA_WOOL_ID, static_cast<byte>(current | 0x10));
	} 
	else 
	{
		entityData->set(DATA_WOOL_ID, static_cast<byte>(current & ~0x10));
	}
}

int Sheepuff::getSheepuffColor(Random *random) 
{
	int nextInt = random->nextInt(100);
	if (nextInt < 5) 
	{
		return 15 - DyePowderItem::BLACK;
	}
	if (nextInt < 10) 
	{
		return 15 - DyePowderItem::GRAY;
	}
	if (nextInt < 15) 
	{
		return 15 - DyePowderItem::SILVER;
	}
	if (nextInt < 18) 
	{
		return 15 - DyePowderItem::BROWN;
	}
	if (random->nextInt(500) == 0) return 15 - DyePowderItem::PINK;
	return 0; // white
}

shared_ptr<AgableMob> Sheepuff::getBreedOffspring(shared_ptr<AgableMob> target)
{
	if( level->canCreateMore( GetType(), Level::eSpawnType_Breed) )
	{
		shared_ptr<Sheepuff> otherSheepuff = dynamic_pointer_cast<Sheepuff>( target );
		shared_ptr<Sheepuff> sheepuff = std::make_shared<Sheepuff>(level);
		int color = getOffspringColor(dynamic_pointer_cast<Animal>(shared_from_this()), otherSheepuff);
		sheepuff->setColor(15 - color);
		return sheepuff;
	}
	else
	{
		return nullptr;
	}
}

void Sheepuff::ate()
{
	setSheared(false);
	if (isBaby())
	{
		ageUp(60);
	}
}

MobGroupData *Sheepuff::finalizeMobSpawn(MobGroupData *groupData, int extraData)
{
	groupData = Animal::finalizeMobSpawn(groupData);

	setColor(getSheepuffColor(level->random));
	return groupData;
}

int Sheepuff::getOffspringColor(shared_ptr<Animal> animal, shared_ptr<Animal> partner)
{
	int parent1DyeColor = getDyeColor(animal);
	int parent2DyeColor = getDyeColor(partner);

	container->getItem(0)->setAuxValue(parent1DyeColor);
	container->getItem(1)->setAuxValue(parent2DyeColor);

	shared_ptr<ItemInstance> instance = Recipes::getInstance()->getItemFor(container, animal->level);

	int color = 0;
	if (instance != nullptr && instance->getItem()->id == Item::dye_powder_Id)
	{
		color = instance->getAuxValue();
	}
	else
	{
		color = level->random->nextBoolean() ? parent1DyeColor : parent2DyeColor;
	}
	return color;
}

int Sheepuff::getDyeColor(shared_ptr<Animal> animal)
{
	return 15 - dynamic_pointer_cast<Sheepuff>(animal)->getColor();
}