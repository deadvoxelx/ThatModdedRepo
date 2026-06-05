/*#include "stdafx.h"
#include "..\Minecraft.World\Mth.h"
#include "FlyingCowModel.h"
#include "ModelPart.h"

void FlyingCowModel::_init(float g)
{
    texWidth = 64;
	texHeight = 64;

    head = new ModelPart(this, 0, 0);
    head->addBox(-4, -4, -6, 8, 8, 6, 0); // Head
    head->setPos(0, 12 - 6 - 2, -8);
	head->texOffs(22, 0)->addBox(-5, -5, -4, 1, 3, 1, 0); // Horn1
	head->texOffs(22, 0)->addBox(+4, -5, -4, 1, 3, 1, 0); // Horn1
    head->compile(1.0f/16.0f);

    body = new ModelPart(this, 18, 4);
    body->addBox(-6, -10, -7, 12, 18, 10, 0); // Body
    body->setPos(0, 11 + 6 - 12, 2);
	body->texOffs(52, 0)->addBox(-2, 2, -8, 4, 6, 1);
	body->compile(1.0f/16.0f);

    leg0 = new ModelPart(this, 0, 16);
    leg0->addBox(-2, 0, -2, 4, 12, 4, g);
    leg0->setPos(-4, -12, 7);
    leg0->compile(1.0f/16.0f);

    leg1 = new ModelPart(this, 0, 16);
    leg1->addBox(-2, 0, -2, 4, 12, 4, g);
    leg1->setPos(4, -12, 7);
    leg1->bMirror = true;
    leg1->compile(1.0f/16.0f);

    leg2 = new ModelPart(this, 0, 16);
    leg2->addBox(-2, 0, -1, 4, 12, 4, g);
    leg2->setPos(-4, -12, -6);
    leg2->compile(1.0f/16.0f);

    leg3 = new ModelPart(this, 0, 16);
    leg3->addBox(-2, 0, -1, 4, 12, 4, g);
    leg3->setPos(4, -12, -6);
    leg3->bMirror = true;
    leg3->compile(1.0f/16.0f);

    wingL = new ModelPart(this, 0, 32);
    wingL->addBox(-1, -14, -4, 2, 17, 8, g);
    wingL->setPos(4, -22, 0);
    setRotation(wingL, 0.0f, 0.0f, 0.0873f);
    wingL->addChild(wingL2);
    wingL->compile(1.0f/16.0f);

    wingR = new ModelPart(this, 0, 32);
    wingR->addBox(-1, -14, -4, 2, 17, 8, g);
    wingR->setPos(-4, -22, 0);
    setRotation(wingR, 0.0f, 0.0f, -0.0873f);
    wingR->addChild(wingR2);
    wingR->compile(1.0f/16.0f);

    wingL2 = new ModelPart(this, 20, 33);
    wingL2->addBox(-1, 0, -4.01, 2, 16, 8, g);
    wingL2->setPos(0, 14, 0);
    setRotation(wingL2, 0.0f, 0.0f, -0.2182f);
    wingL2->compile(1.0f/16.0f);

    wingR2 = new ModelPart(this, 20, 33);
    wingR2->addBox(-1, 0, -4.01, 2, 16, 8, g);
    wingR2->setPos(0, -14, 0);
    setRotation(wingR2, 0.0f, 0.0f, 0.2182f);
    wingR2->compile(1.0f/16.0f);
}

FlyingCowModel::FlyingCowModel() : QuadrupedModel(12,0)
{
	_init(0);
}

FlyingCowModel::FlyingCowModel(float g) : QuadrupedModel(12,0)
{
	_init(g);
}

void FlyingCowModel::setRotation(ModelPart *model, float x, float y, float z)
{
	model->xRot = x;
	model->yRot = y;
	model->zRot = z;
}

void FlyingCowModel::render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled)
{
    render(entity, time, r, bob, yRot, xRot, scale, usecompiled);
    setupAnim(time, r, bob, yRot, xRot, scale, entity);

    head->render(scale, usecompiled);
    body->render(scale, usecompiled);
    leg0->render(scale, usecompiled);
	leg1->render(scale, usecompiled);
	leg2->render(scale, usecompiled);
	leg3->render(scale, usecompiled);
    wingL->render(scale, usecompiled);
    wingR->render(scale, usecompiled);
    wingL2->render(scale, usecompiled);
    wingR2->render(scale, usecompiled);
}

void FlyingCowModel::setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim)
{
    float rad = (float) (180 / PI);
    body->xRot = 90 / rad;

    head->yRot = yRot / (float) (180 / PI);
    head->xRot = xRot / (float) (180 / PI);

    leg0->xRot = (Mth::cos(time * 0.6662f) * 1.4f) * r;
    leg1->xRot = (Mth::cos(time * 0.6662f + PI) * 1.4f) * r;
    leg2->xRot = (Mth::cos(time * 0.6662f + PI) * 1.4f) * r;
    leg3->xRot = (Mth::cos(time * 0.6662f) * 1.4f) * r;
}*/

#include "stdafx.h"
#include "FlyingCowModel.h"
#include "ModelPart.h"

FlyingCowModel::FlyingCowModel() : QuadrupedModel(12,0)
{
    texWidth = 64;
	texHeight = 64;

    head = new ModelPart(this, 0, 0);
    head->addBox(-4, -4, -6, 8, 8, 6, 0); // Head
    head->setPos(0, 12 - 6 - 2, -8);
	head->texOffs(22, 0)->addBox(-5, -5, -4, 1, 3, 1, 0); // Horn1
	head->texOffs(22, 0)->addBox(+4, -5, -4, 1, 3, 1, 0); // Horn1

    body = new ModelPart(this, 18, 4);
    body->addBox(-6, -10, -7, 12, 18, 10, 0); // Body
    body->setPos(0, 11 + 6 - 12, 2);
	body->texOffs(52, 0)->addBox(-2, 2, -8, 4, 6, 1);

    leg0 = new ModelPart(this, 0, 16);
    leg0->addBox(-2, 0, -2, 4, 12, 4, 0);
    leg0->setPos(-4, 12, 7);

    leg1 = new ModelPart(this, 0, 16);
    leg1->addBox(-2, 0, -2, 4, 12, 4, 0);
    leg1->setPos(4, 12, 7);
    leg1->bMirror = true;

    leg2 = new ModelPart(this, 0, 16);
    leg2->addBox(-2, 0, -1, 4, 12, 4, 0);
    leg2->setPos(-4, 12, -6);

    leg3 = new ModelPart(this, 0, 16);
    leg3->addBox(-2, 0, -1, 4, 12, 4, 0);
    leg3->setPos(4, 12, -6);
    leg3->bMirror = true;

    wingL = new ModelPart(this, 0, 32);
    wingL->addBox(-1, -14, -4, 2, 17, 8, 0);
    wingL->setPos(4, 0, 0);
    setRotation(wingL, 0.0f, 0.0f, 0.0873f);

    wingR = new ModelPart(this, 0, 32);
    wingR->addBox(-1, -14, -4, 2, 17, 8, 0);
    wingR->setPos(-4, 0, 0);
    setRotation(wingR, 0.0f, 0.0f, -0.0873f);

    wingL2 = new ModelPart(this, 20, 33);
    wingL2->addBox(-1, 0, -4.01, 2, 16, 8, 0);
    wingL2->setPos(5.2, -14, 0);
    setRotation(wingL2, 0.0f, 0.0f, -0.2182f);

    wingR2 = new ModelPart(this, 20, 33);
    wingR2->addBox(-1, 0, -4.01, 2, 16, 8, 0);
    wingR2->setPos(-5.2, -14, 0);
    setRotation(wingR2, 0.0f, 0.0f, 0.2182f);

	this->zHeadOffs += 2;

    //wingL->addChild(wingL2);
    //wingR->addChild(wingR2);

	head->compile(1.0f/16.0f);
	body->compile(1.0f/16.0f);
    leg0->compile(1.0f/16.0f);
    leg1->compile(1.0f/16.0f);
    leg2->compile(1.0f/16.0f);
    leg3->compile(1.0f/16.0f);
    wingL->compile(1.0f/16.0f);
    wingR->compile(1.0f/16.0f);
    wingL2->compile(1.0f/16.0f);
    wingR2->compile(1.0f/16.0f);
}

void FlyingCowModel::setRotation(ModelPart *model, float x, float y, float z)
{
	model->xRot = x;
	model->yRot = y;
	model->zRot = z;
}

void FlyingCowModel::render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled)
{
    QuadrupedModel::render(entity, time, r, bob, yRot, xRot, scale, usecompiled);
    QuadrupedModel::setupAnim(time, r, bob, yRot, xRot, scale, entity);

    head->render(scale, usecompiled);
    body->render(scale, usecompiled);
    leg0->render(scale, usecompiled);
	leg1->render(scale, usecompiled);
	leg2->render(scale, usecompiled);
	leg3->render(scale, usecompiled);
    wingL->render(scale, usecompiled);
    wingR->render(scale, usecompiled);
    wingL2->render(scale, usecompiled);
    wingR2->render(scale, usecompiled);
}