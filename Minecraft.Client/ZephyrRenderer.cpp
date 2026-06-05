#include "stdafx.h"
#include "ZephyrRenderer.h"
#include "ZephyrModel.h"
#include "..\Minecraft.World\net.minecraft.world.entity.monster.h"

ResourceLocation ZephyrRenderer::ZEPHYR_LOCATION = ResourceLocation(TN_MOB_ZEPHYR);

ZephyrRenderer::ZephyrRenderer() : MobRenderer(new ZephyrModel(), 2.0f)
{
}

void ZephyrRenderer::scale(shared_ptr<LivingEntity> mob, float a)
{
	shared_ptr<Zephyr> zephyr = dynamic_pointer_cast<Zephyr>(mob);
        
	float ss = (zephyr->oCharge+(zephyr->charge-zephyr->oCharge)*a)/20.0f;
	if (ss<0) ss = 0;
	ss = 1/(ss*ss*ss*ss*ss*2+1);
	float s = (8+ss)/2;
	float hs = (8+1/ss)/2;
	glScalef(hs, s, hs);
	glColor4f(1, 1, 1, 1);
}

ResourceLocation *ZephyrRenderer::getTextureLocation(shared_ptr<Entity> mob)
{
    return &ZEPHYR_LOCATION;
}