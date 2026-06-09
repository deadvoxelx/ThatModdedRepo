#pragma once
#include "Model.h"

class MoaModel : public Model
{
public:
	ModelPart *head, *jaw, *neck, *body, *leg, *leg2, *wing, *wing2, *feather1, *feather2, *feather3;

	void _init(float g);
    MoaModel();
    MoaModel(float g);
    virtual void render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled);
    virtual void setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim=0);
};