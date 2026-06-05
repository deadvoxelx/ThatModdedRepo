#include "stdafx.h"
#include "..\Minecraft.World\Mth.h"
#include "ZephyrModel.h"
#include "ModelPart.h"

void ZephyrModel::_init(float g)
{
    //texWidth = 128;
	//texHeight = 64;

    body = new ModelPart(this, 0, 0);
    body->addBox(-8, -4, -8, 10, 7, 12, g);
    body->setPos(0, 24, 0);

	body->compile(1.0f/16.0f);
}

ZephyrModel::ZephyrModel() : Model()
{
	_init(0);
}

ZephyrModel::ZephyrModel(float g) : Model()
{
	_init(g);
}

void ZephyrModel::render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled)
{
    setupAnim(time, r, bob, yRot, xRot, scale, entity);

    body->render(scale, usecompiled);
}

void ZephyrModel::setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim)
{
}