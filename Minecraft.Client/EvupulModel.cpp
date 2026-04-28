#include "stdafx.h"
#include "..\Minecraft.World\Mth.h"
#include "EvupulModel.h"
#include "ModelPart.h"

void EvupulModel::_init(float g)
{

	texWidth = 64;
	texHeight = 64;

    head = new ModelPart(this, 0, 0);
    head->addBox(-4.0, -1.5, -4.0, 8.0, 3.0, 4.0, g);
    head->setPos(0, 22.5, -8);
	
	body = new ModelPart(this, 0, 8);
    body->addBox(-3.0, -3.0, -8.0, 6.0, 3.0, 16.0, g);
    body->setPos(0, 24, 0);
	
	tail = new ModelPart(this, 0, 49);
    tail->addBox(-1.0, -0.5, 0.0, 2.0, 2.0, 12.0, g);
    tail->setPos(0.0, 22.5, 8.0);
	
	wing1 = new ModelPart(this, 0, 27);
    wing1->addBox(0.0, 0.0, -6.0, 17.0, 0.0, 22.0, g);
    wing1->setPos(3.0, 22.5, 0.0);
	
	wing2 = new ModelPart(this, 0, 27);
	wing2->bMirror = true;
    wing2->addBox(-17.0, 0.0, -6.0, 17.0, 0.0, 22.0, g);
    wing2->setPos(-3.0, 22.5, 0.0);

	head->compile(1.0f/16.0f);
	body->compile(1.0f/16.0f);
	tail->compile(1.0f/16.0f);
	wing1->compile(1.0f/16.0f);
	wing2->compile(1.0f/16.0f);
}

EvupulModel::EvupulModel() : Model()
{
	_init(0);
}

EvupulModel::EvupulModel(float g) : Model()
{
	_init(g);
}

void EvupulModel::render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled)
{
    setupAnim(time, r, bob, yRot, xRot, scale, entity);

    head->render(scale, usecompiled);
	body->render(scale, usecompiled);
	tail->render(scale, usecompiled);
	wing1->render(scale, usecompiled);
	wing2->render(scale, usecompiled);
}

void EvupulModel::setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim)
{
    head->yRot = yRot / (float) (180 / PI);
	head->xRot = xRot / (float) (180 / PI);

	wing1->zRot = cos(bob * .25f) * PI * .25f;
	wing2->zRot = -wing1->zRot;
}