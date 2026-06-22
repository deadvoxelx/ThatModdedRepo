#include "stdafx.h"
#include "..\Minecraft.World\Mth.h"
#include "AerwhaleModel.h"
#include "ModelPart.h"

void AerwhaleModel::_init(float g)
{
    texWidth = 512;
	texHeight = 64;

    head = new ModelPart(this, 408, 18);
    head->addBox(-12, -9, -14, 24, 18, 28);
    head->setPos(0, 0, -20);
    setRotation(head, 0.0f, 0.0f, 0.0f);

    bottomPartHead = new ModelPart(this, 116, 28);
    bottomPartHead->addBox(-13, 4, -15, 26, 6, 30);
    bottomPartHead->setPos(0, 0, -20);
    setRotation(bottomPartHead, 0.0f, 0.0f, 0.0f);

    middleBody = new ModelPart(this, 314, 25);
    middleBody->addBox(-11, -5, -1, 22, 14, 25);
    middleBody->setPos(0, -1, -6);
    setRotation(middleBody, -0.0698132f, 0.0f, 0.0f);

    bottomPartMiddlebody = new ModelPart(this, 16, 32);
    bottomPartMiddlebody->addBox(-12, 5, -1, 24, 6, 26);
    bottomPartMiddlebody->setPos(0, -1, -6);
    setRotation(bottomPartMiddlebody, 0.0f, 0.0f, 0.0f);

    frontBody = new ModelPart(this, 0, 0);
    frontBody->addBox(-11.5, -1, -0.5, 19, 5, 21, g);
    frontBody->setPos(2, 6, 18);
    setRotation(frontBody, -0.1047198f, 0.0f, 0.0f);

    backBody = new ModelPart(this, 228, 32);
    backBody->addBox(-10.5, -9, -2, 17, 10, 22);
    backBody->setPos(2, 5, 18);
    setRotation(backBody, -0.1047198f, 0.0f, 0.0f);

    rightFin = new ModelPart(this, 446, 1);
    rightFin->addBox(-20, -2, -6, 19, 3, 14);
    rightFin->setPos(-10, 4, -10);
    setRotation(rightFin, -0.148353f, 0.2094395f, 0.0f);

    leftFin = new ModelPart(this, 446, 1);
    leftFin->addBox(1, -2, -6, 19, 3, 14);
    leftFin->setPos(10, 4, -10);
    setRotation(leftFin, -0.148353f, -0.2094395f, 0.0f);

    middleFin = new ModelPart(this, 318, 35);
    middleFin->addBox(-1, -11, 7, 2, 7, 8);
    middleFin->setPos(0, -1, -6);
    setRotation(middleFin, -0.1441704f, 0.0f, 0.0f);

    backfinRight = new ModelPart(this, 261, 5);
    backfinRight->addBox(-11, 0, -6, 15, 3, 24);
    backfinRight->setPos(-4, 5, 39);
    setRotation(backfinRight, -0.1047198f, -0.7330383f, 0.0f);

    backfinLeft = new ModelPart(this, 261, 5);
    backfinLeft->addBox(-4, 0, -6, 13, 3, 24);
    backfinLeft->setPos(5, 5, 39);
    setRotation(backfinLeft, -0.1047198f, 0.7330383f, 0.0f);



	head->compile(1.0f/16.0f);
    bottomPartHead->compile(1.0f/16.0f);
    middleBody->compile(1.0f/16.0f);
    bottomPartMiddlebody->compile(1.0f/16.0f);
    frontBody->compile(1.0f/16.0f);
    backBody->compile(1.0f/16.0f);
    rightFin->compile(1.0f/16.0f);
    leftFin->compile(1.0f/16.0f);
    middleFin->compile(1.0f/16.0f);
    backfinRight->compile(1.0f/16.0f);
    backfinLeft->compile(1.0f/16.0f);
}

void AerwhaleModel::setRotation(ModelPart *model, float x, float y, float z)
{
	model->xRot = x;
	model->yRot = y;
	model->zRot = z;
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

    head->render(scale, usecompiled);
    bottomPartHead->render(scale, usecompiled);
    middleBody->render(scale, usecompiled);
    bottomPartMiddlebody->render(scale, usecompiled);
    frontBody->render(scale, usecompiled);
    backBody->render(scale, usecompiled);
    rightFin->render(scale, usecompiled);
    leftFin->render(scale, usecompiled);
    middleFin->render(scale, usecompiled);
    backfinRight->render(scale, usecompiled);
    backfinLeft->render(scale, usecompiled);
}

void AerwhaleModel::setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim)
{
    //head->yRot = yRot / (float) (180 / PI);
}
