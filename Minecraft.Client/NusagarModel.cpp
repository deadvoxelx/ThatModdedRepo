#include "stdafx.h"
#include "..\Minecraft.World\Mth.h"
#include "..\Minecraft.World\Nusagar.h"
#include "NusagarModel.h"
#include "ModelPart.h"

void NusagarModel::_init(float g)
{
	texWidth = 64;
	texHeight = 64;

	body1 = new ModelPart(this, 0, 33);
	body1->addBox(-5, -8, -2.5f, 10, 8, 5);
	body1->setPos(0, -12, 0);

	neck = new ModelPart(this, 0, 22);
	neck->addBox(-1.5f, -8, -1.5f, 3, 8, 3);
	neck->setPos(0, -8, 0);

	head = new ModelPart(this, 0, 0);
	head->addBox(-3, -6, -6, 6, 6, 16);
	head->setPos(0, -5.5f, 0);
	setRotation(head, 0.5236f, 0.0f, 0.0f);

	body2 = new ModelPart(this, 0, 46);
	body2->addBox(-3.5f, -6, -2, 7, 6, 4);
	body2->setPos(0, 6, 0);

	hipbone = new ModelPart(this, 0, 56);
	hipbone->addBox(-4.5f, -2, -2, 9, 2, 4);
	hipbone->setPos(0, 2, 0);

	legright1 = new ModelPart(this, 30, 37);
	legright1->addBox(-3, 0, -1.5f, 3, 12, 3);
	legright1->setPos(-2, 0, 0);

	legright2 = new ModelPart(this, 42, 37);
	legright2->addBox(-3, 0, -1.5f, 3, 12, 3);
	legright2->setPos(0, 12, 0);

	legleft1 = new ModelPart(this, 30, 37);
	legleft1->addBox(0, 0, -1.5f, 3, 12, 3);
	legleft1->setPos(2, 0, 0);

	legleft2 = new ModelPart(this, 42, 37);
	legleft2->addBox(0, 0, -1.5f, 3, 12, 3);
	legleft2->setPos(0, 12, 0);

	tail1 = new ModelPart(this, 12, 22);
	tail1->addBox(0, -3, 0, 3, 3, 8);
	tail1->setPos(-1.5f, 1, 2);
	setRotation(tail1, 0.1745f, 0.0f, 0.0f);

	tail2 = new ModelPart(this, 12, 22);
	tail2->addBox(0, -3, 0, 3, 3, 8);
	tail2->setPos(0, 0, 8);

	tail3 = new ModelPart(this, 12, 22);
	tail3->addBox(0, -3, 0, 3, 3, 8);
	tail3->setPos(0, 0, 8);

	tail4 = new ModelPart(this, 12, 22);
	tail4->addBox(0, -3, 0, 3, 3, 8);
	tail4->setPos(0, 0, 8);

	armright1 = new ModelPart(this, 46, 22);
	armright1->addBox(-3, 0, -1.5f, 3, 12, 3);
	armright1->setPos(-5, -8, 0);
	setRotation(armright1, 0.0f, 0.0f, 0.0873f);

	armright2 = new ModelPart(this, 34, 22);
	armright2->addBox(-3, 0, -1.5f, 3, 12, 3);
	armright2->setPos(0, 12, 0);

	armleft1 = new ModelPart(this, 46, 22);
	armleft1->addBox(0, 0, -1.5f, 3, 12, 3);
	armleft1->setPos(5, -8, 0);
	setRotation(armleft1, 0.0f, 0.0f, -0.0873f);

	armleft2 = new ModelPart(this, 34, 22);
	armleft2->addBox(0, 0, -1.5f, 3, 12, 3);
	armleft2->setPos(0, 12, 0);

	body1->addChild(neck);
	neck->addChild(head);
	body1->addChild(body2);
	body2->addChild(hipbone);
	hipbone->addChild(legright1);
	legright1->addChild(legright2);
	hipbone->addChild(legleft1);
	legleft1->addChild(legleft2);
	body2->addChild(tail1);
	tail1->addChild(tail2);
	tail2->addChild(tail3);
	tail3->addChild(tail4);
	body1->addChild(armright1);
	armright1->addChild(armright2);
	body1->addChild(armleft1);
	armleft1->addChild(armleft2);

	body1->compile(1.0f/16.0f);
	neck->compile(1.0f/16.0f);
	head->compile(1.0f/16.0f);
	body2->compile(1.0f/16.0f);
	hipbone->compile(1.0f/16.0f);
	legright1->compile(1.0f/16.0f);
	legright2->compile(1.0f/16.0f);
	legleft1->compile(1.0f/16.0f);
	legleft2->compile(1.0f/16.0f);
	tail1->compile(1.0f/16.0f);
	tail2->compile(1.0f/16.0f);
	tail3->compile(1.0f/16.0f);
	tail4->compile(1.0f/16.0f);
	armright1->compile(1.0f/16.0f);
	armright2->compile(1.0f/16.0f);
	armleft1->compile(1.0f/16.0f);
	armleft2->compile(1.0f/16.0f);
}

NusagarModel::NusagarModel() : Model()
{
	_init(0);
}

NusagarModel::NusagarModel(float g) : Model()
{
	_init(g);
}

int NusagarModel::modelVersion()
{
	return 36;
}

void NusagarModel::render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled)
{
    setupAnim(time, r, bob, yRot, xRot, scale, entity);

    body1->render(scale, usecompiled);
}

void NusagarModel::setRotation(ModelPart *model, float x, float y, float z)
{
	model->xRot = x;
	model->yRot = y;
	model->zRot = z;
}

void NusagarModel::setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim)
{
	shared_ptr<Nusagar> nusagar = dynamic_pointer_cast<Nusagar>(entity);

    head->yRot = yRot / (float) (180 / PI);
	head->xRot = 0.5236f + (xRot / (float) (180 / PI));

	if (nusagar != nullptr)
	{
		if (nusagar->isSpinning())
		{
			body1->xRot = HALF_PI / 1.2f;
			body1->yRot = time * 1.5f;

			tail1->xRot = -0.1745f + -(HALF_PI / 1.5f);
			tail2->xRot = 0.0f;
			tail3->xRot = 0.0f;
			tail4->xRot = 0.0f;

			//tail1->yRot = -(HALF_PI / 1.5f);
			tail2->yRot = -(HALF_PI / 2.0f);
			tail3->yRot = -(HALF_PI / 2.5f);
			tail4->yRot = -(HALF_PI / 3.0f);
		}
		else
		{
			body1->xRot = 0.075f;
			body1->yRot = head->yRot;

			tail1->xRot = 0.1745f + cos(bob * .05f) * PI * .025f;
			tail2->xRot = cos(bob * .05f) * PI * .025f;
			tail3->xRot = cos(bob * .05f) * PI * .025f;
			tail4->xRot = cos(bob * .05f) * PI * .025f;

			tail1->yRot = 0.0f;
			tail2->yRot = 0.0f;
			tail3->yRot = 0.0f;
			tail4->yRot = 0.0f;
		}
	}
}
