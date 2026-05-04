#include "stdafx.h"
#include "AphalafPlantRenderer.h"
#include "AphalafPlantModel.h"
#include "..\Minecraft.World\net.minecraft.world.entity.animal.h"

ResourceLocation AphalafPlantRenderer::APHALAF_PLANT_LOCATION = ResourceLocation(TN_MOB_APHALAF_PLANT);

AphalafPlantRenderer::AphalafPlantRenderer() : MobRenderer(new AphalafPlantModel(), 1.0f)
{
}

void AphalafPlantRenderer::scale(shared_ptr<LivingEntity> _mob, float a)
{
	glScalef(1.2, 1.2, 1.2);
}

void AphalafPlantRenderer::render(shared_ptr<Entity> entity, double x, double y, double z, float rot, float a)
{
	MobRenderer::render(entity, x, y, z, rot, a);
}

ResourceLocation *AphalafPlantRenderer::getTextureLocation(shared_ptr<Entity> mob)
{
    return &APHALAF_PLANT_LOCATION;
}