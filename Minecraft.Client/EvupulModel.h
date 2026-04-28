#pragma once
#include "Model.h"

class EvupulModel : public Model
{
public:
	ModelPart *head, *body, *tail, *wing1, *wing2;

	void _init(float g);
    EvupulModel();
    EvupulModel(float g);
    virtual void render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled);
    virtual void setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim=0);
};