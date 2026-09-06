#include "stdafx.h"
#include "NusaSpikeRenderer.h"
#include "NusaSpikeModel.h"
#include "..\Minecraft.World\net.minecraft.world.entity.animal.h"
#include "..\Minecraft.World\net.minecraft.world.entity.monster.h"

ResourceLocation NusaSpikeRenderer::NUSA_SPIKE_LOCATION = ResourceLocation(TN_MOB_NUSA_SPIKE);

NusaSpikeRenderer::NusaSpikeRenderer() : MobRenderer(new NusaSpikeModel(), 0.3f)
{
}

void NusaSpikeRenderer::render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a)
{
	MobRenderer::render(_mob, x, y, z, rot, a);
}

ResourceLocation *NusaSpikeRenderer::getTextureLocation(shared_ptr<Entity> mob)
{
    return &NUSA_SPIKE_LOCATION;
}