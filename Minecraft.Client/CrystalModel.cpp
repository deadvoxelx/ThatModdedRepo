#include "stdafx.h"
#include "..\Minecraft.World\Mth.h"
#include "CrystalModel.h"
#include "ModelPart.h"

void CrystalModel::_init(float g)
{
	texWidth = 64;
	texHeight = 32;

	crystal = new ModelPart(this, 0, 0);
	crystal->addBox(-3, -3, -3, 6, 6, 6, g);
	crystal->texOffs(24, 0)->addBox(-3, -3, -3, 6, 6, 6, g + 0.5f);
	crystal->setPos(0.0f, 0.0f, 0.0f);
	crystal->compile(1.0f / 16.0f);
}

CrystalModel::CrystalModel() : Model()
{
	_init(0);
}

CrystalModel::CrystalModel(float g) : Model()
{
	_init(g);
}

void CrystalModel::render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled)
{
	setupAnim(time, r, bob, yRot, xRot, scale, entity);
	crystal->render(scale, usecompiled);
}

void CrystalModel::setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim)
{
	crystal->xRot = Mth::sin(time * 0.1f) * 0.05f;
	crystal->yRot = time * 0.5f;
	crystal->zRot = 0.0f;
}
