#pragma once
#include "Model.h"

class AerwhaleModel : public Model
{
public:
	ModelPart *body, *body2, *body3, *fin1, *fin2, *fin3, *fin4;

	void _init(float g);
    AerwhaleModel();
    AerwhaleModel(float g);
    virtual void render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled);
    virtual void setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim=0);
};