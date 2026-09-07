#include "stdafx.h"
#include "SunSpiritRenderer.h"
#include "SunSpiritModel.h"
#include "..\Minecraft.World\net.minecraft.world.entity.animal.h"
#include "..\Minecraft.World\SunSpirit.h"
#include "..\Minecraft.World\FireMinion.h"
#include "BossMobGuiInfo.h"

ResourceLocation SunSpiritRenderer::SUN_SPIRIT_LOCATION = ResourceLocation(TN_MOB_SUN_SPIRIT);
ResourceLocation SunSpiritRenderer::SUN_SPIRIT_FROZEN_LOCATION = ResourceLocation(TN_MOB_SUN_SPIRIT_FROZEN);

SunSpiritRenderer::SunSpiritRenderer() : MobRenderer(new SunSpiritModel(), 1.0f)
{
}

void SunSpiritRenderer::render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a)
{
	shared_ptr<SunSpirit> sunSpirit = dynamic_pointer_cast<SunSpirit>(_mob);
	if (sunSpirit != nullptr)
	{
		BossMobGuiInfo::setBossHealth(sunSpirit, true);
	}

	MobRenderer::render(_mob, x, y, z, rot, a);
}

ResourceLocation *SunSpiritRenderer::getTextureLocation(shared_ptr<Entity> mob)
{
	shared_ptr<SunSpirit> sunSpirit = dynamic_pointer_cast<SunSpirit>(mob);
	if (sunSpirit != nullptr && sunSpirit->isFreezing())
	{
		return &SUN_SPIRIT_FROZEN_LOCATION;
	}

	return &SUN_SPIRIT_LOCATION;
}

void SunSpiritRenderer::scale(shared_ptr<LivingEntity> mob, float a)
{
	shared_ptr<FireMinion> fireMinion = dynamic_pointer_cast<FireMinion>(mob);

	if (fireMinion) glScalef(1.0f, 1.0f, 1.0f);
	else glScalef(2.5f, 2.5f, 2.5f);
}
