#pragma once
#include "Model.h"

class AphalafPlantModel : public Model
{
public:
	ModelPart *base, *leaf1, *leaf2, *leaf3, *leaf4, *mouth1, *mouth2, *mouth3, *mouth4, *teeth1, *teeth2, *teeth3, *teeth4;

	void _init(float g);
    AphalafPlantModel();
    AphalafPlantModel(float g);
    virtual void render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled);
    virtual void setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim=0);

private:
	void setRotation(ModelPart *model, float x, float y, float z);
};