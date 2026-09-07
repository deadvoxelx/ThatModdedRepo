#include "stdafx.h"
#include "CrystalModel.h"
#include "..\Minecraft.World\EntityCrystal.h"
#include "..\Minecraft.World\SharedConstants.h"
#include "CrystalRenderer.h"

ResourceLocation CrystalRenderer::FIRE_CRYSTAL_LOCATION = ResourceLocation(TN_MOB_FIRE_CRYSTAL);
ResourceLocation CrystalRenderer::ICE_CRYSTAL_LOCATION = ResourceLocation(TN_MOB_ICE_CRYSTAL);
ResourceLocation CrystalRenderer::CLOUD_CRYSTAL_LOCATION = ResourceLocation(TN_MOB_CLOUD_CRYSTAL);
ResourceLocation CrystalRenderer::THUNDER_CRYSTAL_LOCATION = ResourceLocation(TN_MOB_THUNDER_CRYSTAL);

CrystalRenderer::CrystalRenderer()
{
	this->shadowRadius = 0.0f;
	model = new CrystalModel();
}

void CrystalRenderer::render(shared_ptr<Entity> _crystal, double x, double y, double z, float rot, float a)
{
	shared_ptr<EntityCrystal> crystal = dynamic_pointer_cast<EntityCrystal>(_crystal);

	float tt = crystal->tickCount + a;
	glPushMatrix();
	glTranslatef(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z));
	bindTexture(getTextureLocation(_crystal));

	glRotatef(tt * 20.0f, 0, 1, 0);
	glRotatef(tt * 10.0f, 1, 0, 0);

	float hh = sin(tt * 0.2f) / 2 + 0.5f;
	hh = hh * hh + hh;

	model->render(crystal, 0, tt * 3, hh * 0.2f, 0, 0, 4.0f / 16.0f, true);

	if (SharedConstants::TEXTURE_LIGHTING)
	{
		int col = 0x00f0;
		int u = col % 65536;
		int v = col / 65536;
		glMultiTexCoord2f(GL_TEXTURE1, u / 1.0f, v / 1.0f);
		glColor4f(1, 1, 1, 1);
	}
	glColor4f(1, 1, 1, 1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	bindTexture(getOverlayLocation(crystal));
	model->render(crystal, 0, tt * 3, hh * 0.2f, 0, 0, 4.0f / 16.0f, true);
	glDisable(GL_BLEND);

	glPopMatrix();
}

ResourceLocation *CrystalRenderer::getOverlayLocation(shared_ptr<EntityCrystal> crystal)
{
	switch (crystal->getCrystalType())
	{
	case eCrystalType_ICE:
		return &ICE_CRYSTAL_LOCATION;
	case eCrystalType_CLOUD:
		return &CLOUD_CRYSTAL_LOCATION;
	case eCrystalType_THUNDER:
		return &THUNDER_CRYSTAL_LOCATION;
	case eCrystalType_FIRE:
	default:
		return &FIRE_CRYSTAL_LOCATION;
	}
}

ResourceLocation *CrystalRenderer::getTextureLocation(shared_ptr<Entity> mob)
{
	shared_ptr<EntityCrystal> crystal = dynamic_pointer_cast<EntityCrystal>(mob);
	if (crystal == nullptr) return &FIRE_CRYSTAL_LOCATION;

	switch (crystal->getCrystalType())
	{
	case eCrystalType_ICE:
		return &ICE_CRYSTAL_LOCATION;
	case eCrystalType_CLOUD:
		return &CLOUD_CRYSTAL_LOCATION;
	case eCrystalType_THUNDER:
		return &THUNDER_CRYSTAL_LOCATION;
	case eCrystalType_FIRE:
	default:
		return &FIRE_CRYSTAL_LOCATION;
	}
}
