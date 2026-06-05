#include "stdafx.h"
#include "SheepuffRenderer.h"
#include "MultiPlayerLocalPlayer.h"
#include "..\Minecraft.World\net.minecraft.world.entity.animal.h"

ResourceLocation SheepuffRenderer::SHEEPUFF_LOCATION = ResourceLocation(TN_MOB_SHEEPUFF);
ResourceLocation SheepuffRenderer::SHEEPUFF_FUR_LOCATION = ResourceLocation(TN_MOB_SHEEPUFF_FUR);

SheepuffRenderer::SheepuffRenderer(Model *model, Model *armor, float shadow) : MobRenderer(model, shadow)
{
	setArmor(armor);
}

int SheepuffRenderer::prepareArmor(shared_ptr<LivingEntity> _sheepuff, int layer, float a)
{
	shared_ptr<Sheepuff> sheepuff = dynamic_pointer_cast<Sheepuff>(_sheepuff);

    if (layer == 0 && !sheepuff->isSheared() &&
		!sheepuff->isInvisibleTo(Minecraft::GetInstance()->player))
	{
		MemSect(31);
		bindTexture(&SHEEPUFF_FUR_LOCATION);
		MemSect(0);

		if (sheepuff->hasCustomName() && sheepuff->getCustomName().compare(L"jeb_") == 0)
		{
			int colorDuration = 25;
			int value = (sheepuff->tickCount / colorDuration) + sheepuff->entityId;
			int c1 = value % Sheepuff::COLOR_LENGTH;
			int c2 = (value + 1) % Sheepuff::COLOR_LENGTH;
			float subStep = ((sheepuff->tickCount % colorDuration) + a) / static_cast<float>(colorDuration);

			glColor3f(
				Sheepuff::COLOR[c1][0] * (1.0f - subStep) + Sheepuff::COLOR[c2][0] * subStep, 
				Sheepuff::COLOR[c1][1] * (1.0f - subStep) + Sheepuff::COLOR[c2][1] * subStep, 
				Sheepuff::COLOR[c1][2] * (1.0f - subStep) + Sheepuff::COLOR[c2][2] * subStep);
		}
		else
		{
        float brightness = SharedConstants::TEXTURE_LIGHTING ? 1.0f : sheepuff->getBrightness(a);
        int color = sheepuff->getColor();
        glColor3f(brightness * Sheepuff::COLOR[color][0], brightness * Sheepuff::COLOR[color][1], brightness * Sheepuff::COLOR[color][2]);

		}
        return 1;
    }
    return -1;
}

void SheepuffRenderer::render(shared_ptr<Entity> mob, double x, double y, double z, float rot, float a)
{
	MobRenderer::render(mob, x, y, z, rot, a);
} 

ResourceLocation *SheepuffRenderer::getTextureLocation(shared_ptr<Entity> mob)
{
    return &SHEEPUFF_LOCATION;
}