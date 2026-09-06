#include "stdafx.h"
#include "..\Minecraft.World\Mth.h"
#include "NusaSpikeModel.h"
#include "ModelPart.h"

void NusaSpikeModel::_init(float g)
{
	texWidth = 32;
	texHeight = 32;

    spike_r1 = new ModelPart(this, 0, 0);
    spike_r1->addBox(-7.5, -28, 0, 15, 28, 0, g);
    spike_r1->setPos(0, 24, 0);
	setRotation(spike_r1, 0.0f, 0.7854f, 0.0f);

    spike_r2 = new ModelPart(this, 0, 0);
    spike_r2->addBox(-7.5, -28, 0, 15, 28, 0, g);
    spike_r2->setPos(0, 24, 0);
	setRotation(spike_r2, 0.0f, -0.7854f, 0.0f);

	spike_r1->compile(1.0f/16.0f);
	spike_r2->compile(1.0f/16.0f);
}

NusaSpikeModel::NusaSpikeModel() : Model()
{
	_init(0);
}

NusaSpikeModel::NusaSpikeModel(float g) : Model()
{
	_init(g);
}

void NusaSpikeModel::render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled)
{
    setupAnim(time, r, bob, yRot, xRot, scale, entity);

    spike_r1->render(scale, usecompiled);
    spike_r2->render(scale, usecompiled);
}

void NusaSpikeModel::setRotation(ModelPart *model, float x, float y, float z)
{
	model->xRot = x;
	model->yRot = y;
	model->zRot = z;
}

void NusaSpikeModel::setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim)
{
}