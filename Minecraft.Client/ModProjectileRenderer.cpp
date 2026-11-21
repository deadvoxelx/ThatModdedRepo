#include "stdafx.h"
#include "ModProjectileRenderer.h"
#include "../Minecraft.World/ModProjectile.h"
#include "../Minecraft.World/net.minecraft.world.item.h"
#include "../RubyLauncher/Registry/Projectile/ModProjectileRegistry.h"

ModProjectileRenderer::ModProjectileRenderer() : ItemSpriteRenderer(Item::snowBall)
{
}

void ModProjectileRenderer::render(shared_ptr<Entity> e, double x, double y, double z, float rot, float a)
{
	Item *icon = Item::snowBall;

	shared_ptr<ModProjectile> projectile = dynamic_pointer_cast<ModProjectile>(e);
	const ModProjectileSpec *spec = (projectile != nullptr) ? ModProjectileRegistry::get(projectile->getSpecId()) : nullptr;
	if (spec != nullptr && spec->iconItemId >= 0 && spec->iconItemId < Item::ITEM_NUM_COUNT && Item::items[spec->iconItemId] != nullptr)
	{
		icon = Item::items[spec->iconItemId];
	}

	setSourceItem(icon);
	ItemSpriteRenderer::render(e, x, y, z, rot, a);
}
