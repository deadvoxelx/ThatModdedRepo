#include "stdafx.h"
#include "..\Minecraft.World\Mth.h"
#include "..\Minecraft.World\AphalafBoss.h"
#include "AphalafBossModel.h"
#include "ModelPart.h"

//im ngl im surprised this worked first try lol

void AphalafBossModel::_init(float g)
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

    tentacle1 = new ModelPart(this, 55, 1);
    tentacle1->addBox(-0, -16, -2, 1, 16, 1, g);
    tentacle1->setPos(0, 21.8, 0);
    setRotation(tentacle1, 1.0472f, 0.1745f, 0.0f);

    tentacle2 = new ModelPart(this, 55, 1);
    tentacle2->addBox(-0, -16, -2, 1, 16, 1, g);
    tentacle2->setPos(0, 21.8, 0);
    setRotation(tentacle2, 1.0472f, 2.2689f, 0.0f);

    tentacle3 = new ModelPart(this, 55, 1);
    tentacle3->addBox(-0, -16, -2, 1, 16, 1, g);
    tentacle3->setPos(0, 21.8, 0);
    setRotation(tentacle3, 1.0472f, 4.3633f, 0.0f);

    tentacle4 = new ModelPart(this, 61, 1);
    tentacle4->addBox(-0, -12, -17.45, 1, 12, 1, g);
    tentacle4->setPos(0, 7.2, 0);
    setRotation(tentacle4, 0.5236f, 0.1745f, 0.0f);

    tentacle5 = new ModelPart(this, 61, 1);
    tentacle5->addBox(-0, -12, -17.45, 1, 12, 1, g);
    tentacle5->setPos(0, 7.2, 0);
    setRotation(tentacle5, 0.5236f, 2.2689f, 0.0f);

    tentacle6 = new ModelPart(this, 61, 1);
    tentacle6->addBox(-0, -12, -17.45, 1, 12, 1, g);
    tentacle6->setPos(0, 7.2, 0);
    setRotation(tentacle6, 0.5236f, 4.3633f, 0.0f);

    eye1 = new ModelPart(this, 42, 41);
    eye1->addBox(-1.5, -3, -23, 3, 3, 3, g);
    eye1->setPos(0, 4.2, 0);
    setRotation(eye1, 0.1745f, 0.1745f, 0.0f);

    eye2 = new ModelPart(this, 42, 49);
    eye2->addBox(-1.5, -3, -23, 3, 3, 3, g);
    eye2->setPos(0, 4.2, 0);
    setRotation(eye2, 0.1745f, 2.2689f, 0.0f);

    eye3 = new ModelPart(this, 42, 57);
    eye3->addBox(-1.5, -3, -23, 3, 3, 3, g);
    eye3->setPos(0, 4.2, 0);
    setRotation(eye3, 0.1745f, 4.3633f, 0.0f);

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
    tentacle1->compile(1.0f/16.0f);
    tentacle2->compile(1.0f/16.0f);
    tentacle3->compile(1.0f/16.0f);
    tentacle4->compile(1.0f/16.0f);
    tentacle5->compile(1.0f/16.0f);
    tentacle6->compile(1.0f/16.0f);
    eye1->compile(1.0f/16.0f);
    eye2->compile(1.0f/16.0f);
    eye3->compile(1.0f/16.0f);
}

AphalafBossModel::AphalafBossModel() : Model()
{
	_init(0);
}

AphalafBossModel::AphalafBossModel(float g) : Model()
{
	_init(g);
}

int AphalafBossModel::modelVersion()
{
	return 32;
}

void AphalafBossModel::render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled)
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
    tentacle1->render(scale, usecompiled);
    tentacle2->render(scale, usecompiled);
    tentacle3->render(scale, usecompiled);
    tentacle4->render(scale, usecompiled);
    tentacle5->render(scale, usecompiled);
    tentacle6->render(scale, usecompiled);
    eye1->render(scale, usecompiled);
    eye2->render(scale, usecompiled);
    eye3->render(scale, usecompiled);
}

void AphalafBossModel::setRotation(ModelPart *model, float x, float y, float z)
{
	model->xRot = x;
	model->yRot = y;
	model->zRot = z;
}

void AphalafBossModel::setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim)
{
    //head->yRot = yRot / (float) (180 / PI);
}