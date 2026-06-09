#include "stdafx.h"
#include "..\Minecraft.World\Mth.h"
#include "MoaModel.h"
#include "ModelPart.h"

void MoaModel::_init(float g)
{
    head = new ModelPart(this, 0, 13);
    head->addBox(-2, -4, -6, 4, 4, 8, 0);
    head->setPos(0, (-8 + 16), -4);

    jaw = new ModelPart(this, 24, 13);
    jaw->addBox(-2, -1, -6, 4, 1, 8, -0.1);
    jaw->setPos(0, (-8 + 16), -4);

    neck = new ModelPart(this, 44, 0);
    neck->addBox(-1, -6, -1, 2, 6, 2, g);
    neck->setPos(0, (-2 + 16), -4);

    body = new ModelPart(this, 0, 0);
    body->addBox(-3, -3, 0, 6, 8, 5, 0);
    body->setPos(0, (0 + 16), 0);

    leg = new ModelPart(this, 22, 0);
    leg->addBox(-1, -1, -1, 2, 9, 2, g);
    leg->setPos(-2, (0 + 16), 1);

    leg2 = new ModelPart(this, 22, 0);
    leg2->addBox(-1, -1, -1, 2, 9, 2, g);
    leg2->setPos(2, (0 + 16), 1);

    wing = new ModelPart(this, 52, 0);
    wing->addBox(-1, 0, -1, 1, 8, 4, g);
    wing->setPos(-3, (-4 + 16), 0);

    wing2 = new ModelPart(this, 52, 0);
    wing2->addBox(0, 0, -1, 1, 8, 4, g);
    wing2->setPos(3, (-4 + 16), 0);

    feather1 = new ModelPart(this, 30, 0);
    feather1->addBox(-1, -5, 5, 2, 1, 5, -0.3);
    feather1->setPos(0, (1 + 16), 1);

    feather2 = new ModelPart(this, 30, 0);
    feather2->addBox(-1, -5, 5, 2, 1, 5, -0.3);
    feather2->setPos(0, (1 + 16), 1);

    feather3 = new ModelPart(this, 30, 0);
    feather3->addBox(-1, -5, 5, 2, 1, 5, -0.3);
    feather3->setPos(0, (1 + 16), 1);

    feather1->y += 0.5;
    feather2->y += 0.5;
    feather3->y += 0.5;

	head->compile(1.0f/16.0f);
    jaw->compile(1.0f/16.0f);
    neck->compile(1.0f/16.0f);
    body->compile(1.0f/16.0f);
    leg->compile(1.0f/16.0f);
    leg2->compile(1.0f/16.0f);
    wing->compile(1.0f/16.0f);
    wing2->compile(1.0f/16.0f);
    feather1->compile(1.0f/16.0f);
    feather2->compile(1.0f/16.0f);
    feather3->compile(1.0f/16.0f);
}

MoaModel::MoaModel() : Model()
{
	_init(0);
}

MoaModel::MoaModel(float g) : Model()
{
	_init(g);
}

void MoaModel::render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled)
{
    setupAnim(time, r, bob, yRot, xRot, scale, entity);

    head->render(scale, usecompiled);
    jaw->render(scale, usecompiled);
    neck->render(scale, usecompiled);
    body->render(scale, usecompiled);
    leg->render(scale, usecompiled);
    leg2->render(scale, usecompiled);
    wing->render(scale, usecompiled);
    wing2->render(scale, usecompiled);
    feather1->render(scale, usecompiled);
    feather2->render(scale, usecompiled);
    feather3->render(scale, usecompiled);
}

void MoaModel::setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim)
{
    head->xRot = xRot / (float) (180 / PI);
    head->yRot = yRot / (float) (180 / PI);
    jaw->xRot = head->xRot;
    jaw->yRot = head->yRot;
    neck->xRot = 0.0f;
    neck->yRot = head->yRot;
    body->xRot = 1.570796f;
    leg->xRot = (Mth::cos(time * 0.6662f) * 1.4f) * r;
    leg2->xRot = (Mth::cos(time * 0.6662f + PI) * 1.4f) * r;
    feather1->xRot = 0.25f;
    feather2->xRot = 0.25f;
    feather3->xRot = 0.25f;
    feather1->yRot = -0.375f;
    feather2->yRot = 0.0f;
    feather3->yRot = 0.375f;
}