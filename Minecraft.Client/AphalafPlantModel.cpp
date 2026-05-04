#include "stdafx.h"
#include "..\Minecraft.World\Mth.h"
#include "AphalafPlantModel.h"
#include "ModelPart.h"

void AphalafPlantModel::_init(float g)
{
    texWidth = 64;
	texHeight = 64;

    base = new ModelPart(this, 0, 38);
    base->addBox(-5, -1, -5, 10, 1, 10, g);
    base->setPos(0, 24, 0);
    setRotation(base, 0.0f, 0.7854f, 0.0f);

    leaf1 = new ModelPart(this, 0, 51);
    leaf1->addBox(-3.5, -1, 0, 7, 1, 10, g);
    leaf1->setPos(0, 23, 5);
    setRotation(leaf1, 0.2618f, 0.0f, 0.0f);

    leaf2 = new ModelPart(this, 0, 51);
    leaf2->addBox(0, -1, 0, 7, 1, 10, g);
    leaf2->setPos(5, 23, 3.5);
    setRotation(leaf2, 0.2618f, 1.5708f, 0.0f);

    leaf3 = new ModelPart(this, 0, 51);
    leaf3->addBox(0, -1, 0, 7, 1, 10, g);
    leaf3->setPos(3.5, 23, -5);
    setRotation(leaf3, 0.2618f, 3.1416f, 0.0f);

    leaf4 = new ModelPart(this, 0, 51);
    leaf4->addBox(0, -1, 0, 7, 1, 10, g);
    leaf4->setPos(-5, 23, -3.5);
    setRotation(leaf4, 0.2618f, -1.5708f, 0.0f);

    mouth1 = new ModelPart(this, 0, 0);
    mouth1->addBox(0, -2, 0, 10, 2, 16, g);
    mouth1->setPos(0, 23, 7);
    setRotation(mouth1, 0.8727f, 0.7854f, 0.0f);

    mouth2 = new ModelPart(this, 0, 0);
    mouth2->addBox(0, -2, 0, 10, 2, 16, g);
    mouth2->setPos(7, 23, 0);
    setRotation(mouth2, 0.8727f, 2.3562f, 0.0f);

    mouth3 = new ModelPart(this, 0, 0);
    mouth3->addBox(0, -2, 0, 10, 2, 16, g);
    mouth3->setPos(0, 23, -7);
    setRotation(mouth3, 0.8727f, -2.3562f, 0.0f);

    mouth4 = new ModelPart(this, 0, 0);
    mouth4->addBox(0, -2, 0, 10, 2, 16, g);
    mouth4->setPos(-7, 23, 0);
    setRotation(mouth4, 0.8727f, -0.7854f, 0.0f);

    teeth1 = new ModelPart(this, 0, 18);
    teeth1->addBox(0, -4, 0, 10, 4, 16, g);
    teeth1->setPos(0, 23, 7);
    setRotation(teeth1, 0.8727f, 0.7854f, 0.0f);

    teeth2 = new ModelPart(this, 0, 18);
    teeth2->addBox(0, -4, 0, 10, 4, 16, g);
    teeth2->setPos(7, 23, 0);
    setRotation(teeth2, 0.8727f, 2.3562f, 0.0f);

    teeth3 = new ModelPart(this, 0, 18);
    teeth3->addBox(0, -4, 0, 10, 4, 16, g);
    teeth3->setPos(0, 23, -7);
    setRotation(teeth3, 0.8727f, -2.3562f, 0.0f);

    teeth4 = new ModelPart(this, 0, 18);
    teeth4->addBox(0, -4, 0, 10, 4, 16, g);
    teeth4->setPos(-7, 23, 0);
    setRotation(teeth4, 0.8727f, -0.7854f, 0.0f);

	base->compile(1.0f/16.0f);
    leaf1->compile(1.0f/16.0f);
    leaf2->compile(1.0f/16.0f);
    leaf3->compile(1.0f/16.0f);
    leaf4->compile(1.0f/16.0f);
    mouth1->compile(1.0f/16.0f);
    mouth2->compile(1.0f/16.0f);
    mouth3->compile(1.0f/16.0f);
    mouth4->compile(1.0f/16.0f);
    teeth1->compile(1.0f/16.0f);
    teeth2->compile(1.0f/16.0f);
    teeth3->compile(1.0f/16.0f);
    teeth4->compile(1.0f/16.0f);
}

AphalafPlantModel::AphalafPlantModel() : Model()
{
	_init(0);
}

AphalafPlantModel::AphalafPlantModel(float g) : Model()
{
	_init(g);
}

void AphalafPlantModel::render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled)
{
    setupAnim(time, r, bob, yRot, xRot, scale, entity);

    base->render(scale, usecompiled);
    leaf1->render(scale, usecompiled);
    leaf2->render(scale, usecompiled);
    leaf3->render(scale, usecompiled);
    leaf4->render(scale, usecompiled);
    mouth1->render(scale, usecompiled);
    mouth2->render(scale, usecompiled);
    mouth3->render(scale, usecompiled);
    mouth4->render(scale, usecompiled);
    teeth1->render(scale, usecompiled);
    teeth2->render(scale, usecompiled);
    teeth3->render(scale, usecompiled);
    teeth4->render(scale, usecompiled);
}

void AphalafPlantModel::setRotation(ModelPart *model, float x, float y, float z)
{
	model->xRot = x;
	model->yRot = y;
	model->zRot = z;
}

void AphalafPlantModel::setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim)
{
    //head->yRot = yRot / (float) (180 / PI);
}