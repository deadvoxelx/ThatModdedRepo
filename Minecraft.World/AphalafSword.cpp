#include "stdafx.h"
#include "net.minecraft.world.item.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.ai.attributes.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.entity.monster.h"
#include "net.minecraft.world.entity.projectile.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.effect.h"
#include "net.minecraft.world.level.h"
#include "HellSphereItem.h"
#include "AphalafSword.h"

AphalafSwordItem::AphalafSwordItem(int id, const Tier *tier) : Item(id), tier( tier )
{
	maxStackSize = 1;
	setMaxDamage(tier->getUses());

	damage = 4 + tier->getAttackDamageBonus();
}

float AphalafSwordItem::getTierDamage()
{
	return tier->getAttackDamageBonus();
}

float AphalafSwordItem::getDestroySpeed(shared_ptr<ItemInstance> itemInstance, Tile *tile)
{
	if (tile->id == Tile::web_Id)
	{
		return 15;
	}

	if (tile->id == Tile::gravel_Id)
	{
		return 1.5f;
	}

	if (tile->id == Tile::lavaRock_Id)
	{
		return 1.5f;
	}

	Material *material = tile->material;
	if (material == Material::plant || material == Material::replaceable_plant || material == Material::coral || material == Material::leaves || material == Material::vegetable || material == Material::dirt)
	{
		return 1.5f;
	}
	return 1.0f;
}

bool AphalafSwordItem::hurtEnemy(shared_ptr<ItemInstance> itemInstance, shared_ptr<LivingEntity> mob, shared_ptr<LivingEntity> attacker) 
{
	itemInstance->hurtAndBreak(1, attacker);
	if ( mob->instanceof(eTYPE_LIVINGENTITY) )
	{
		dynamic_pointer_cast<LivingEntity>(mob)->addEffect(new MobEffectInstance(MobEffect::weakness->id, 200, 0));
	}
	return true;
}

bool AphalafSwordItem::mineBlock(shared_ptr<ItemInstance> itemInstance, Level *level, int tile, int x, int y, int z, shared_ptr<LivingEntity> owner)
{
	if (Tile::tiles[tile]->getDestroySpeed(level, x, y, z) != 0.0) itemInstance->hurtAndBreak(2, owner);
	return true;
}

bool AphalafSwordItem::isHandEquipped()
{
	return true;
}

UseAnim AphalafSwordItem::getUseAnimation(shared_ptr<ItemInstance> itemInstance)
{
	return UseAnim_block;
}    

int AphalafSwordItem::getUseDuration(shared_ptr<ItemInstance> itemInstance)
{
	return 20 * 60 * 60;
}

shared_ptr<ItemInstance> AphalafSwordItem::use(shared_ptr<ItemInstance> instance, Level *level, shared_ptr<Player> player)
{
	player->startUsingItem(instance, getUseDuration(instance));

	level->playEntitySound((shared_ptr<Entity> ) player, eSoundType_RANDOM_BOW, 0.5f, 0.4f / (random->nextFloat() * 0.4f + 0.8f));
	if (!level->isClientSide) level->addEntity( shared_ptr<HellSphere>( new HellSphere(level, player) ) );

	return instance;
}

bool AphalafSwordItem::canDestroySpecial(Tile *tile)
{
	return tile->id == Tile::web_Id;
}

int AphalafSwordItem::getEnchantmentValue()
{
	return tier->getEnchantmentValue();
}

const Item::Tier *AphalafSwordItem::getTier()
{
	return tier;
}

bool AphalafSwordItem::isValidRepairItem(shared_ptr<ItemInstance> source, shared_ptr<ItemInstance> repairItem)
{
	if (tier->getTierItemId() == repairItem->id)
	{
		return true;
	}
	return Item::isValidRepairItem(source, repairItem);
}

attrAttrModMap *AphalafSwordItem::getDefaultAttributeModifiers()
{
	attrAttrModMap *result = Item::getDefaultAttributeModifiers();

	result->insert(attrAttrModMap::value_type( SharedMonsterAttributes::ATTACK_DAMAGE->getId(), new AttributeModifier(eModifierId_ITEM_BASEDAMAGE, damage, AttributeModifier::OPERATION_ADDITION) ) );

	return result;
}
