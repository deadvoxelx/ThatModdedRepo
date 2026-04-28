#include "stdafx.h"
#include "EvupulRenderer.h"
#include "EvupulModel.h"
#include "..\Minecraft.World\net.minecraft.world.entity.monster.h"

ResourceLocation EvupulRenderer::EVUPULDARK_LOCATION = ResourceLocation(TN_MOB_EVUPULDARK);
ResourceLocation EvupulRenderer::EVUPULDARK_OVERLAY_LOCATION = ResourceLocation(TN_MOB_EVUPULDARKOVERLAY);

EvupulRenderer::EvupulRenderer() : MobRenderer(new EvupulModel(), 0.6f)
{
	this->setArmor(new EvupulModel());
}

void EvupulRenderer::render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a)
{
	MobRenderer::render(_mob, x, y, z, rot, a);
}

ResourceLocation *EvupulRenderer::getTextureLocation(shared_ptr<Entity> mob)
{
    return &EVUPULDARK_LOCATION;
}

int EvupulRenderer::prepareArmor(shared_ptr<LivingEntity> _evupulDark, int layer, float a)
{
	shared_ptr<EvupulDark> evupulDark = dynamic_pointer_cast<EvupulDark>(_evupulDark);

	if (layer!=0) return -1;
	MemSect(31);
	bindTexture(&EVUPULDARK_OVERLAY_LOCATION);
	MemSect(0);
	float br = 1.0f;
	glEnable(GL_BLEND);
#ifdef _XBOX
	glDisable(GL_ALPHA_TEST);
#endif
	glBlendFunc(GL_ONE, GL_ONE);
	if (evupulDark->isInvisible())	glDepthMask(false);
	else						glDepthMask(true);

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