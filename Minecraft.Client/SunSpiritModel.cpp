#include "stdafx.h"
#include "..\Minecraft.World\Mth.h"
#include "..\Minecraft.World\FireMinion.h"
#include "SunSpiritModel.h"
#include "ModelPart.h"

void SunSpiritModel::_init(float g)
{
    texWidth = 64;
	texHeight = 64;

    head = new ModelPart(this, 0, 0);
    head->addBox(-4, - 8, -4, 8, 5, 7, g);
	head->texOffs(0, 12)->addBox(-4, -3, -5, 8, 3, 8, g);
    head->setPos(0.0f, -5.75f, 0.5f);

    torso = new ModelPart(this, 34, 0);
    torso->addBox(-5, -5.75, -2.5, 10, 6, 5, g);
	torso->texOffs(34, 11)->addBox(-4.5, 0.25, -2, 9, 5, 4, g);
	torso->texOffs(0, 54)->addBox(-4.5, 5.25, -2.5, 9, 1, 5, g);
    torso->setPos(0.0f, 18.0f, 0.0f);

    rightArm = new ModelPart(this, 0, 23);
    rightArm->addBox(-5.5, -2.5, -2.5, 5, 5, 5, g + 0.5f);
	rightArm->texOffs(0, 33)->addBox(-5.5, 2.5, -2.5, 5, 10, 5, g);
	rightArm->texOffs(0, 48)->addBox(-5.5, 7.5, -2.5, 5, 1, 5, g + 0.25f);
    rightArm->setPos(-5.0f, -3.75f, 0.0f);

    leftArm = new ModelPart(this, 20, 33);
    leftArm->addBox(0.5, 2.5, -2.5, 5, 10, 5, g);
	leftArm->texOffs(20, 23)->addBox(0.5, -2.5, -2.5, 5, 5, 5, g + 0.5f);
	leftArm->texOffs(20, 48)->addBox(0.5, 7.5, -2.5, 5, 1, 5, g + 0.25f);
	leftArm->bMirror = true;
    leftArm->setPos(5.0f, -3.75f, 0.0f);

    torso->addChild(head);
    torso->addChild(rightArm);
    torso->addChild(leftArm);

	head->compile(1.0f/16.0f);
	torso->compile(1.0f/16.0f);
	rightArm->compile(1.0f/16.0f);
	leftArm->compile(1.0f/16.0f);
}

SunSpiritModel::SunSpiritModel() : Model()
{
	_init(0);
}

SunSpiritModel::SunSpiritModel(float g) : Model()
{
	_init(g);
}

void SunSpiritModel::render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled)
{
    setupAnim(time, r, bob, yRot, xRot, scale, entity);

    torso->render(scale, usecompiled);
}

void SunSpiritModel::setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim)
{
    shared_ptr<FireMinion> fireMinion = dynamic_pointer_cast<FireMinion>(entity);

    head->yRot = yRot / (float) (180 / PI);
    head->xRot = xRot / (float) (180 / PI);

    if (fireMinion) torso->y = 6;

    rightArm->xRot = -(Mth::sin(time * 0.067f) * 0.05f);
    rightArm->yRot = 0.0f;
	rightArm->zRot = -(Mth::cos(time * 0.09f) * 0.05f - 0.05f);

    leftArm->xRot = -(rightArm->xRot);
    leftArm->yRot = rightArm->yRot;
	leftArm->zRot = -(rightArm->zRot);
}
