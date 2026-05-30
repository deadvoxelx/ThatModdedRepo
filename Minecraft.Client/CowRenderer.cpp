#include "stdafx.h"
#include "CowRenderer.h"
#include "..\Minecraft.World\net.minecraft.world.entity.monster.h"
#include "..\Minecraft.World\net.minecraft.world.entity.animal.h"

ResourceLocation CowRenderer::COW_LOCATION = ResourceLocation(TN_MOB_COW);
ResourceLocation CowRenderer::COW_BROWN_LOCATION = ResourceLocation(TN_MOB_COW_BROWN);
ResourceLocation CowRenderer::COW_BLACK_LOCATION = ResourceLocation(TN_MOB_COW_BLACK);
ResourceLocation CowRenderer::COW_INVERT_LOCATION = ResourceLocation(TN_MOB_COW_INVERT);

CowRenderer::CowRenderer(Model *model, float shadow) : MobRenderer(model, shadow)
{
}

void CowRenderer::render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a)
{
	MobRenderer::render(_mob, x, y, z, rot, a);
}

ResourceLocation *CowRenderer::getTextureLocation(shared_ptr<Entity> mob)
{
    shared_ptr<Cow> cow = dynamic_pointer_cast<Cow>(mob);

    if (cow->getCowType() == Cow::TYPE_DEFAULT)
	{
        return &COW_LOCATION;
    }
    if (cow->getCowType() == Cow::TYPE_BROWN)
	{
        return &COW_BROWN_LOCATION;
    }
    if (cow->getCowType() == Cow::TYPE_BLACK)
	{
        return &COW_BLACK_LOCATION;
    }
    if (cow->getCowType() == Cow::TYPE_INVERT)
	{
        return &COW_INVERT_LOCATION;
    }
}
