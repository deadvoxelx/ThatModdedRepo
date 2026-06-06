#include "stdafx.h"
#include "AerwhaleRenderer.h"
#include "AerwhaleModel.h"
#include "..\Minecraft.World\net.minecraft.world.entity.animal.h"

ResourceLocation AerwhaleRenderer::AERWHALE_LOCATION = ResourceLocation(TN_MOB_AERWHALE);

AerwhaleRenderer::AerwhaleRenderer() : MobRenderer(new AerwhaleModel(), 2.5f)
{
}

void AerwhaleRenderer::render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a)
{
	MobRenderer::render(_mob, x, y, z, rot, a);
}

ResourceLocation *AerwhaleRenderer::getTextureLocation(shared_ptr<Entity> mob)
{
    return &AERWHALE_LOCATION;
}

void AerwhaleRenderer::scale(shared_ptr<LivingEntity> mob, float a)
{
	glScalef(5.0f, 5.0f, 5.0f);
}