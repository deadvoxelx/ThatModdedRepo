#pragma once
#include "Model.h"

class ZephyrModel : public Model
{
public:
	ModelPart *body;

	void _init(float g);
    ZephyrModel();
    ZephyrModel(float g);
    virtual void render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled);
    virtual void setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim=0);
};