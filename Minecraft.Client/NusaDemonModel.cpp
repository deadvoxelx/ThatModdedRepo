#include "stdafx.h"
#include "..\Minecraft.World\Mth.h"
#include "..\Minecraft.World\net.minecraft.world.entity.monster.h"
#include "NusaDemonModel.h"
#include "ModelPart.h"

void NusaDemonModel::_init(float g)
{
	texWidth = 192;
	texHeight = 192;

	body1 = new ModelPart(this, 0, 80);
    body1->addBox(-12, -14, -8, 24, 16, 16);
    body1->setPos(0, -16, 0);

	body2 = new ModelPart(this, 0, 32);
	body2->addBox(-20, -24, -12, 40, 24, 24);
	body2->setPos(0, -14, 0);

    head = new ModelPart(this, 0, 0);
    head->addBox(-8, -16, -8, 16, 16, 16);
    head->setPos(0, -24, 0);

	rightHorn = new ModelPart(this, 64, 0);
	rightHorn->addBox(-2, -8, 0, 4, 8, 4);
	rightHorn->setPos(-8, -10, -4);
	setRotation(rightHorn, 1.2217f, 0.6981f, 0.0f);
	
	leftHorn = new ModelPart(this, 64, 0);
	leftHorn->bMirror = true;
	leftHorn->addBox(-2, -8, 0, 4, 8, 4);
	leftHorn->setPos(8, -10, -4);
	setRotation(leftHorn, 1.2217f, -0.6981f, 0.0f);

	rightHorn2 = new ModelPart(this, 64, 0);
	rightHorn2->addBox(-2, -8, 0, 4, 8, 4);
	rightHorn2->setPos(0, -8, 0);
	setRotation(rightHorn2, 0.0f, 0.0f, 0.61085f);
	
	leftHorn2 = new ModelPart(this, 64, 0);
	leftHorn2->bMirror = true;
	leftHorn2->addBox(-2, -8, 0, 4, 8, 4);
	leftHorn2->setPos(0, -8, 0);
	setRotation(leftHorn2, 0.0f, 0.0f, -0.61085f);
	
	armtopright = new ModelPart(this, 120, 0);
	armtopright->addBox(-16, -10, -10, 16, 20, 20);
	armtopright->setPos(-20, -16, 0);

	armtopleft = new ModelPart(this, 120, 0);
	armtopleft->addBox(0, -10, -10, 16, 20, 20);
	armtopleft->setPos(20, -16, 0);

	armmiddleright = new ModelPart(this, 128, 40);
	armmiddleright->addBox(-13, -12, -6, 10, 12, 12);
	armmiddleright->setPos(0, 22, 0);

	armmiddleleft = new ModelPart(this, 128, 40);
	armmiddleleft->addBox(3, -12, -6, 10, 12, 12);
	armmiddleleft->setPos(0, 22, 0);

	armbottomright = new ModelPart(this, 128, 64);
	armbottomright->addBox(-15, -32, -8, 14, 32, 16);
	armbottomright->setPos(0, 32, 0);

	armbottomleft = new ModelPart(this, 128, 64);
	armbottomleft->addBox(1, -32, -8, 14, 32, 16);
	armbottomleft->setPos(0, 32, 0);

	leg1 = new ModelPart(this, 0, 112);
    leg1->addBox(-12, 0, -8, 16, 40, 16);
    leg1->setPos(-8, -16, 0);

	leg2 = new ModelPart(this, 0, 112);
    leg2->addBox(-4, 0, -8, 16, 40, 16);
    leg2->setPos(8, -16, 0);

	body2->addChild(head);
	body1->addChild(body2);
	head->addChild(rightHorn);
	head->addChild(leftHorn);
	rightHorn->addChild(rightHorn2);
	leftHorn->addChild(leftHorn2);
	body2->addChild(armtopright);
	body2->addChild(armtopleft);
	armtopright->addChild(armmiddleright);
	armtopleft->addChild(armmiddleleft);
	armmiddleright->addChild(armbottomright);
	armmiddleleft->addChild(armbottomleft);

	body1->compile(1.0f/16.0f);
	body2->compile(1.0f/16.0f);
	head->compile(1.0f/16.0f);
	rightHorn->compile(1.0f/16.0f);
	leftHorn->compile(1.0f/16.0f);
	armtopright->compile(1.0f/16.0f);
	armtopleft->compile(1.0f/16.0f);
	armmiddleright->compile(1.0f/16.0f);
	armmiddleleft->compile(1.0f/16.0f);
	armbottomright->compile(1.0f/16.0f);
	armbottomleft->compile(1.0f/16.0f);
	leg1->compile(1.0f/16.0f);
	leg2->compile(1.0f/16.0f);
}

NusaDemonModel::NusaDemonModel() : Model()
{
	_init(0);
}

NusaDemonModel::NusaDemonModel(float g) : Model()
{
	_init(g);
}

int NusaDemonModel::modelVersion()
{
	return 36;
}

void NusaDemonModel::render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled)
{
    setupAnim(time, r, bob, yRot, xRot, scale, entity);

    body1->render(scale, usecompiled);
    //body2->render(scale, usecompiled);
    //head->render(scale, usecompiled);
    //rightHorn->render(scale, usecompiled);
    //leftHorn->render(scale, usecompiled);
    //armtopright->render(scale, usecompiled);
    //armtopleft->render(scale, usecompiled);
    //armmiddleright->render(scale, usecompiled);
    //armmiddleleft->render(scale, usecompiled);
    //armbottomright->render(scale, usecompiled);
    //armbottomleft->render(scale, usecompiled);
	leg1->render(scale, usecompiled);
    leg2->render(scale, usecompiled);
}

void NusaDemonModel::setRotation(ModelPart *model, float x, float y, float z)
{
	model->xRot = x;
	model->yRot = y;
	model->zRot = z;
}

void NusaDemonModel::setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim)
{
    head->yRot = yRot / (float) (180 / PI);
	head->xRot = xRot / (float) (180 / PI);

	leg1->xRot = (Mth::cos(time * 0.333f) * 0.4f) * r;
    leg2->xRot = (Mth::cos(time * 0.333f + PI) * 0.4f) * r;

	armtopleft->xRot = (Mth::cos(time * 0.333f) * 0.4f) * r;
    armtopright->xRot = (Mth::cos(time * 0.333f + PI) * 0.4f) * r;

	body2->zRot = (Mth::cos(time * 0.333f) * 0.2f) * r;
}