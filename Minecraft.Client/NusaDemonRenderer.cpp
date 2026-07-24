#include "stdafx.h"
#include "NusaDemonRenderer.h"
#include "NusaDemonModel.h"
#include "..\Minecraft.World\net.minecraft.world.entity.monster.h"
#include "..\Minecraft.World\net.minecraft.world.entity.animal.h"

ResourceLocation NusaDemonRenderer::NUSA_DEMON_LOCATION = ResourceLocation(TN_MOB_NUSA_DEMON);
ResourceLocation NusaDemonRenderer::NUSA_DEMON_OVERLAY_LOCATION = ResourceLocation(TN_MOB_NUSA_DEMON_OVERLAY);

NusaDemonRenderer::NusaDemonRenderer() : MobRenderer(new NusaDemonModel(), 2.5f)
{
	this->setArmor(new NusaDemonModel());
	modelVersion = static_cast<NusaDemonModel *>(model)->modelVersion();
}

void NusaDemonRenderer::render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a)
{
	int modelVersion = (dynamic_cast<NusaDemonModel*>(model))->modelVersion();
	if (modelVersion != this->modelVersion) {
		this->modelVersion = modelVersion;
		model = new NusaDemonModel();
	}
	MobRenderer::render(_mob, x, y, z, rot, a);
}

ResourceLocation *NusaDemonRenderer::getTextureLocation(shared_ptr<Entity> entity)
{
	return &NUSA_DEMON_LOCATION;
}

void NusaDemonRenderer::setupPosition(shared_ptr<LivingEntity> mob, double x, double y, double z)
{
	MobRenderer::setupPosition(mob, x, y, z);
}

int NusaDemonRenderer::prepareArmor(shared_ptr<LivingEntity> _mob, int layer, float a)
{
	shared_ptr<NusaDemon> nusaDemon = dynamic_pointer_cast<NusaDemon>(_mob);

	if (layer!=0) return -1;
	MemSect(31);
	if (shared_ptr<LivingEntity>(nusaDemon))
	{
        bindTexture(&NUSA_DEMON_OVERLAY_LOCATION);
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