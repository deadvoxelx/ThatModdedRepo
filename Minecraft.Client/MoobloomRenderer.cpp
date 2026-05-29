#include "stdafx.h"
#include "ModelPart.h"
#include "MoobloomRenderer.h"
#include "TextureAtlas.h"
#include "QuadrupedModel.h"
#include "..\Minecraft.World\net.minecraft.world.entity.animal.h"
#include "..\Minecraft.World\net.minecraft.world.level.tile.h"

ResourceLocation MoobloomRenderer::MOOBLOOM_LOCATION = ResourceLocation(TN_MOB_MOOBLOOM);

MoobloomRenderer::MoobloomRenderer(Model *model, float shadow) : MobRenderer(model, shadow)
{
}

void MoobloomRenderer::render(shared_ptr<Entity> _mob, double x, double y, double z, float rot, float a)
{
	MobRenderer::render(_mob, x, y, z, rot, a);
}

void MoobloomRenderer::additionalRendering(shared_ptr<LivingEntity> _mob, float a)
{
	shared_ptr<Moobloom> mob = dynamic_pointer_cast<Moobloom>(_mob);
	MobRenderer::additionalRendering(mob, a);
	if (mob->isBaby()) return;
	bindTexture(&TextureAtlas::LOCATION_BLOCKS);
	glEnable(GL_CULL_FACE);
	glPushMatrix();
	glScalef(1, -1, 1);
	glTranslatef(0.2f, 0.36f, 0.5f);
	glRotatef(42, 0, 1, 0);
	tileRenderer->renderTile(Tile::flower, 0, 1);
	glTranslatef(0.1f, 0, -0.6f);
	glRotatef(42, 0, 1, 0);
	tileRenderer->renderTile(Tile::flower, 0, 1);
	glPopMatrix();
	glPushMatrix();
	static_cast<QuadrupedModel *>(model)->head->translateTo(1 / 16.0f);
	glScalef(1, -1, 1);
	glTranslatef(0, 0.75f, -0.2f);
	glRotatef(12, 0, 1, 0);
	tileRenderer->renderTile(Tile::flower, 0, 1);
	glPopMatrix();
	glDisable(GL_CULL_FACE);
}

ResourceLocation *MoobloomRenderer::getTextureLocation(shared_ptr<Entity> mob)
{
    return &MOOBLOOM_LOCATION;
}