#include "stdafx.h"
#include "SunSpiritRenderer.h"
#include "SunSpiritModel.h"
#include "..\Minecraft.World\net.minecraft.world.entity.animal.h"

ResourceLocation SunSpiritRenderer::SUN_SPIRIT_LOCATION = ResourceLocation(TN_MOB_SUN_SPIRIT);

SunSpiritRenderer::SunSpiritRenderer() : MobRenderer(new SunSpiritModel(), 2.5f)
{
}

void SunSpiritRenderer::render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a)
{
	MobRenderer::render(_mob, x, y, z, rot, a);
}

ResourceLocation *SunSpiritRenderer::getTextureLocation(shared_ptr<Entity> mob)
{
    return &SUN_SPIRIT_LOCATION;
}

void SunSpiritRenderer::scale(shared_ptr<LivingEntity> mob, float a)
{
	glScalef(2.5f, 2.5f, 2.5f);
}