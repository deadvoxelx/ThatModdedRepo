#include "stdafx.h"
#include "NuskullRenderer.h"
#include "WatcherModel.h"
#include "..\Minecraft.World\net.minecraft.world.entity.monster.h"
#include "..\Minecraft.World\net.minecraft.world.entity.animal.h"

ResourceLocation NuskullRenderer::NUSKULL_LOCATION = ResourceLocation(TN_MOB_NUSKULL);
ResourceLocation NuskullRenderer::NUSKULL_OVERLAY_LOCATION = ResourceLocation(TN_MOB_NUSKULL_LAYER);
ResourceLocation NuskullRenderer::NUSKULL2_LOCATION = ResourceLocation(TN_MOB_NUSKULL2);
ResourceLocation NuskullRenderer::NUSKULL2_OVERLAY_LOCATION = ResourceLocation(TN_MOB_NUSKULL2_LAYER);
ResourceLocation NuskullRenderer::NUCLEAR_LOCATION = ResourceLocation(TN_MOB_NUSKULL_NUKE);
ResourceLocation NuskullRenderer::NUCLEAR_OVERLAY_LOCATION = ResourceLocation(TN_MOB_NUSKULL_NUKE_LAYER);

NuskullRenderer::NuskullRenderer() : MobRenderer(new WatcherModel(), 0.4f)
{
	this->setArmor(new WatcherModel());
}

void NuskullRenderer::render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a)
{
	MobRenderer::render(_mob, x, y, z, rot, a);
}

void NuskullRenderer::scale(shared_ptr<LivingEntity> _mob, float a)
{
	glScalef(1.2, 1.2, 1.2);
}

ResourceLocation *NuskullRenderer::getTextureLocation(shared_ptr<Entity> entity)
{
	shared_ptr<NuclearNuskull> nuclear = dynamic_pointer_cast<NuclearNuskull>(entity);
	shared_ptr<Nuskull> nuskull = dynamic_pointer_cast<Nuskull>(entity);

	if (entity->instanceof(eTYPE_NUCLEAR_NUSKULL))
	{
        return &NUCLEAR_LOCATION;
    }

    if (nuskull->getNuskullType() == Nuskull::TYPE_DEFAULT)
	{
        return &NUSKULL_LOCATION;
    }
    if (nuskull->getNuskullType() == Nuskull::TYPE_VARIANT)
	{
        return &NUSKULL2_LOCATION;
    }
}

int NuskullRenderer::prepareArmor(shared_ptr<LivingEntity> _mob, int layer, float a)
{
	shared_ptr<NuclearNuskull> nuclear = dynamic_pointer_cast<NuclearNuskull>(_mob);
	shared_ptr<Nuskull> nuskull = dynamic_pointer_cast<Nuskull>(_mob);

	if (layer!=0) return -1;
	MemSect(31);
	if (shared_ptr<LivingEntity>(nuclear))
	{
        bindTexture(&NUCLEAR_OVERLAY_LOCATION);
    }
	else
	{
		if (nuskull->getNuskullType() == Nuskull::TYPE_DEFAULT)
		{
			bindTexture(&NUSKULL_OVERLAY_LOCATION);
		}
		else if (nuskull->getNuskullType() == Nuskull::TYPE_VARIANT)
		{
			bindTexture(&NUSKULL2_OVERLAY_LOCATION);
		}
    }
	MemSect(0);
	float br = 1.0f;
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	if (SharedConstants::TEXTURE_LIGHTING)
	{
		int col = 0x00f0;			
		int u = col % 65536;
		int v = col / 65536;

		glMultiTexCoord2f(GL_TEXTURE1, u / 1.0f, v / 1.0f);
		glColor4f(1, 1, 1, 1);
	}
	glColor4f(1, 1, 1, br);
	return 1;
}