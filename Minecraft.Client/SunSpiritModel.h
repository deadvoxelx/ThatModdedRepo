#pragma once
#include "Model.h"

class SunSpiritModel : public Model
{
public:
	ModelPart *head;
	ModelPart *torso;
	ModelPart *rightArm;
	ModelPart *leftArm;

	void _init(float g);
    SunSpiritModel();
    SunSpiritModel(float g);
    virtual void render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled);
    virtual void setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim=0);
};