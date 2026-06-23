#include "stdafx.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.item.h"
#include "ItemInstance.h"
#include "RelicMalletItem.h"

TileArray RelicMalletItem::diggables;

void RelicMalletItem::staticCtor()
{
	RelicMalletItem::diggables = TileArray( PICKAXE_DIGGABLES);
	diggables.data[0] = Tile::cobblestone;
	diggables.data[1] = Tile::stoneSlab;
	diggables.data[2] = Tile::stoneSlabHalf;
	diggables.data[3] = Tile::stone;
	diggables.data[4] = Tile::sandStone;
	diggables.data[5] = Tile::mossyCobblestone;
	diggables.data[6] = Tile::ironOre;
	diggables.data[7] = Tile::ironBlock;
	diggables.data[8] = Tile::coalOre;
	diggables.data[9] = Tile::goldBlock;
	diggables.data[10] = Tile::goldOre;
	diggables.data[11] = Tile::diamondOre;
	diggables.data[12] = Tile::diamondBlock;
	diggables.data[13] = Tile::ice;
	diggables.data[14] = Tile::netherRack;
	diggables.data[15] = Tile::lapisOre;
	diggables.data[16] = Tile::lapisBlock;
	diggables.data[17] = Tile::redStoneOre;
	diggables.data[18] = Tile::redStoneOre_lit;
	diggables.data[19] = Tile::rail;
	diggables.data[20] = Tile::detectorRail;
	diggables.data[21] = Tile::goldenRail;
	diggables.data[22] = Tile::activatorRail;
	diggables.data[23] = Tile::netherGold;
	diggables.data[24] = Tile::netherDiamond;
	diggables.data[25] = Tile::nethaniumOre;
	diggables.data[26] = Tile::nethaniumBlock;
	diggables.data[27] = Tile::zaniteOre;
	diggables.data[28] = Tile::zaniteBlock;
	diggables.data[29] = Tile::gravititeOre;
	diggables.data[30] = Tile::enchantedGravitite;
	diggables.data[31] = Tile::ambrosiumOre;
	diggables.data[32] = Tile::obsidian;
	diggables.data[33] = Tile::endoriumOre;
	diggables.data[34] = Tile::endoriumBlock;
}

RelicMalletItem::RelicMalletItem(int id, const Tier *tier) : DiggerItem(id, 8, tier, &diggables)
{
}

bool RelicMalletItem::canDestroySpecial(Tile *tile)
{
	if (tile == Tile::obsidian) return tier->getLevel() >= 3;
	if (tile == Tile::diamondBlock || tile == Tile::diamondOre) return tier->getLevel() >= 2;
	if (tile == Tile::emeraldBlock || tile == Tile::emeraldOre) return tier->getLevel() >= 2;
	if (tile == Tile::goldBlock || tile == Tile::goldOre) return tier->getLevel() >= 2;
	if (tile == Tile::ironBlock || tile == Tile::ironOre) return tier->getLevel() >= 1;
	if (tile == Tile::lapisBlock || tile == Tile::lapisOre) return tier->getLevel() >= 1;
	if (tile == Tile::redStoneOre || tile == Tile::redStoneOre_lit || tile == Tile::redstoneBlock) return tier->getLevel() >= 2;
	if (tile == Tile::netherGold) return tier->getLevel() >= 1;
	if (tile == Tile::netherDiamond) return tier->getLevel() >= 2;
	if (tile == Tile::nethaniumBlock || tile == Tile::nethaniumOre) return tier->getLevel() >= 3;
	if (tile == Tile::endoriumBlock || tile == Tile::endoriumGrate || tile == Tile::endoriumLamp || tile == Tile::endoriumOre) return tier->getLevel() >= 3;
	if (tile == Tile::enchantedGravitite || tile == Tile::gravititeOre) return tier->getLevel() >= 2;
	if (tile == Tile::zaniteBlock || tile == Tile::zaniteOre) return tier->getLevel() >= 1;
	if (tile->material == Material::stone) return true;
	if (tile->material == Material::metal) return true;
	if (tile->material == Material::heavyMetal) return true;
	if (tile->material == Material::endStone) return true;
	if (tile->material == Material::veloettGrass) return true;
	if (tile->material == Material::obsidian) return true;
	if (tile->material == Material::netherrack) return true;
	return false;
}

float RelicMalletItem::getDestroySpeed(shared_ptr<ItemInstance> itemInstance, Tile *tile)
{
	if (tile != NULL && (tile->material == Material::metal || tile->material == Material::heavyMetal || tile->material == Material::stone || tile->material == Material::glass || tile->material == Material::endStone || tile->material == Material::veloettGrass || tile->material == Material::netherrack || tile->material == Material::obsidian))
	{
		return speed;
	}
	return DiggerItem::getDestroySpeed(itemInstance, tile);
}

bool RelicMalletItem::mineBlock(shared_ptr<ItemInstance> itemInstance, Level *level, int tile, int x, int y, int z, shared_ptr<LivingEntity> owner)
{
	itemInstance->hurtAndBreak(1, owner);
	if (!level->isClientSide) level->explode(nullptr, x + 0.5f, y + 0.5f, z + 0.5f, 3, false, true);
	return true;
}

UseAnim RelicMalletItem::getUseAnimation(shared_ptr<ItemInstance> itemInstance)
{
	return UseAnim_block;
}    

int RelicMalletItem::getUseDuration(shared_ptr<ItemInstance> itemInstance)
{
	return 20 * 60 * 60; // Block for a maximum of one hour!
}

shared_ptr<ItemInstance> RelicMalletItem::use(shared_ptr<ItemInstance> instance, Level *level, shared_ptr<Player> player)
{
	player->startUsingItem(instance, getUseDuration(instance));
	return instance;
}
