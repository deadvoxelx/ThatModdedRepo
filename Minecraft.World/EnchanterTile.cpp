#include "stdafx.h"
#include "EnchanterTile.h"
#include "net.minecraft.world.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.inventory.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.tile.entity.h"
#include "net.minecraft.world.entity.item.h"
#include "net.minecraft.world.entity.player.h"
#include "Facing.h"

bool EnchanterTile::noDrop = false;

EnchanterTile::EnchanterTile(int id) : BaseEntityTile(id, Material::stone)
{
	random = new Random();
	iconTop = nullptr;
}

void EnchanterTile::onPlace(Level *level, int x, int y, int z)
{
	BaseEntityTile::onPlace(level, x, y, z);
}

bool EnchanterTile::TestUse()
{
	return true;
}

bool EnchanterTile::use(Level *level, int x, int y, int z, shared_ptr<Player> player, int clickedFace, float clickX, float clickY, float clickZ, bool soundOnly/*=false*/) // 4J added soundOnly param
{
	if(soundOnly) return false;

	if (level->isClientSide)
	{
		return true;
	}
	shared_ptr<FurnaceTileEntity> enchanter = dynamic_pointer_cast<FurnaceTileEntity>( level->getTileEntity(x, y, z) );
	if (enchanter != nullptr ) player->openFurnace(enchanter);
	return true;
}

shared_ptr<TileEntity> EnchanterTile::newTileEntity(Level *level)
{
	return std::make_shared<FurnaceTileEntity>();
}

void EnchanterTile::setPlacedBy(Level *level, int x, int y, int z, shared_ptr<LivingEntity> by, shared_ptr<ItemInstance> itemInstance)
{
	if (itemInstance->hasCustomHoverName())
	{
		dynamic_pointer_cast<FurnaceTileEntity>( level->getTileEntity(x, y, z))->setCustomName(itemInstance->getHoverName());
	}
}

void EnchanterTile::onRemove(Level *level, int x, int y, int z, int id, int data)
{
	if (!noDrop)
	{
		shared_ptr<Container> container = dynamic_pointer_cast<FurnaceTileEntity>( level->getTileEntity(x, y, z) );
		if( container != nullptr )
		{
			for (unsigned int i = 0; i < container->getContainerSize(); i++)
			{
				shared_ptr<ItemInstance> item = container->getItem(i);
				if (item != nullptr)
				{
					float xo = random->nextFloat() * 0.8f + 0.1f;
					float yo = random->nextFloat() * 0.8f + 0.1f;
					float zo = random->nextFloat() * 0.8f + 0.1f;

					while (item->count > 0)
					{
						int count = random->nextInt(21) + 10;
						if (count > item->count) count = item->count;
						item->count -= count;

#ifndef _CONTENT_PACKAGE
						if(level->isClientSide)
						{
							printf("Client furnace dropping %d of %d/%d\n", count, item->id, item->getAuxValue() );
						}
						else
						{
							printf("Server furnace dropping %d of %d/%d\n", count, item->id, item->getAuxValue() );
						}
#endif
						
						shared_ptr<ItemInstance> newItem = std::make_shared<ItemInstance>(item->id, count, item->getAuxValue());
						newItem->set4JData( item->get4JData() );
						shared_ptr<ItemEntity> itemEntity = std::make_shared<ItemEntity>(level, x + xo, y + yo, z + zo, newItem);
						float pow = 0.05f;
						itemEntity->xd = static_cast<float>(random->nextGaussian()) * pow;
						itemEntity->yd = static_cast<float>(random->nextGaussian()) * pow + 0.2f;
						itemEntity->zd = static_cast<float>(random->nextGaussian()) * pow;
						if (item->hasTag())
						{
							itemEntity->getItem()->setTag(static_cast<CompoundTag *>(item->getTag()->copy()));
						}
						level->addEntity(itemEntity);
					}
					container->setItem(i,nullptr);
				}
			}
			level->updateNeighbourForOutputSignal(x, y, z, id);
		}
	}
	BaseEntityTile::onRemove(level, x, y, z, id, data);
}

bool EnchanterTile::hasAnalogOutputSignal()
{
	return true;
}

int EnchanterTile::getAnalogOutputSignal(Level *level, int x, int y, int z, int dir)
{
	return AbstractContainerMenu::getRedstoneSignalFromContainer(dynamic_pointer_cast<Container>( level->getTileEntity(x, y, z)) );
}

int EnchanterTile::cloneTileId(Level *level, int x, int y, int z)
{
	return Tile::enchanter_Id;
}

Icon *EnchanterTile::getTexture(int face, int data)
{
	if (face == Facing::UP || face == Facing::DOWN) return iconTop;
	return icon;
}

void EnchanterTile::registerIcons(IconRegister *iconRegister)
{
	icon = iconRegister->registerIcon(L"enchanter_side");
	iconTop = iconRegister->registerIcon(L"enchanter_top");
}