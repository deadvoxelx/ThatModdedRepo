#include "stdafx.h"
#include "..\Minecraft.World\Mth.h"
#include "AerwhaleModel.h"
#include "ModelPart.h"

void AerwhaleModel::_init(float g)
{
    body = new ModelPart(this, 20, 0);
    body->addBox(-3.5, -3.5, -12.5, 7, 6, 10, g);
    body->setPos(0, 20, 0);

    body2 = new ModelPart(this, 0, 0);
    body2->addBox(-2.5, -2.5, -2.5, 5, 5, 5, g);
    body2->setPos(0, 20, 0);

    body3 = new ModelPart(this, 0, 10);
    body3->addBox(-1.5, -1.5, 2.5, 3, 3, 4, g);
    body3->setPos(0, 20, 0);

    fin1 = new ModelPart(this, 0, 17);
    fin1->addBox(-7.5, -0.5, 2.5, 8, 1, 4, g);
    fin1->setPos(0, 20, 0);

    fin2 = new ModelPart(this, 0, 17);
    fin2->addBox(-0.5, -0.5, 2.5, 8, 1, 4, g);
    fin2->setPos(0, 20, 0);

    fin3 = new ModelPart(this, 0, 22);
    fin3->addBox(-7.5, 1.5, -6.5, 4, 1, 2, g);
    fin3->setPos(0, 20, 0);

    fin4 = new ModelPart(this, 0, 22);
    fin4->addBox(3.5, 1.5, -6.5, 4, 1, 2, g);
    fin4->setPos(0, 20, 0);

	body->compile(1.0f/16.0f);
    body2->compile(1.0f/16.0f);
    body3->compile(1.0f/16.0f);
    fin1->compile(1.0f/16.0f);
    fin2->compile(1.0f/16.0f);
    fin3->compile(1.0f/16.0f);
    fin4->compile(1.0f/16.0f);
}

AerwhaleModel::AerwhaleModel() : Model()
{
	_init(0);
}

AerwhaleModel::AerwhaleModel(float g) : Model()
{
	_init(g);
}

void AerwhaleModel::render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled)
{
    setupAnim(time, r, bob, yRot, xRot, scale, entity);

    body->render(scale, usecompiled);
    body2->render(scale, usecompiled);
    body3->render(scale, usecompiled);
    fin1->render(scale, usecompiled);
    fin2->render(scale, usecompiled);
    fin3->render(scale, usecompiled);
    fin4->render(scale, usecompiled);
}

void AerwhaleModel::setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim)
{
    //head->yRot = yRot / (float) (180 / PI);
}