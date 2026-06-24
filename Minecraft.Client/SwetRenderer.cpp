#include "stdafx.h"
#include "SwetRenderer.h"	
#include "..\Minecraft.World\net.minecraft.world.entity.monster.h"

ResourceLocation SwetRenderer::SWET_LOCATION = ResourceLocation(TN_MOB_SWETBLUE);
ResourceLocation SwetRenderer::SWET_GOLD_LOCATION = ResourceLocation(TN_MOB_SWETGOLD);

SwetRenderer::SwetRenderer(Model *model, Model *armor, float shadow) : MobRenderer(model, shadow)
{
	this->armor = armor;
}

int SwetRenderer::prepareArmor(shared_ptr<LivingEntity> _swet, int layer, float a)
{
	shared_ptr<Swet> swet = dynamic_pointer_cast<Swet>(_swet);

	if (swet->isInvisible()) 
	{
		return 0;
	}
    if (layer == 0)
	{
        setArmor(armor);

        glEnable(GL_NORMALIZE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return 1;
    }
    if (layer == 1)
	{
        glDisable(GL_BLEND);
        glColor4f(1, 1, 1, 1);
    }
    return -1;
}

void SwetRenderer::scale(shared_ptr<LivingEntity> _swet, float a)
{
	shared_ptr<Swet> swet = dynamic_pointer_cast<Swet>(_swet);

    float size = static_cast<float>(swet->getSize());
    float ss = (swet->oSquish + (swet->squish - swet->oSquish) * a) / (size * 0.5f + 1);
    float w = 1 / (ss + 1);
    glScalef(w * size, 1 / w * size, w * size);
}

ResourceLocation *SwetRenderer::getTextureLocation(shared_ptr<Entity> mob)
{
    shared_ptr<Swet> swet = dynamic_pointer_cast<Swet>(mob);

    if (swet->getSwetType() == Swet::TYPE_BLUE)
	{
        return &SWET_LOCATION;
    }
    if (swet->getSwetType() == Swet::TYPE_GOLD)
	{
        return &SWET_GOLD_LOCATION;
    }
}