#pragma once
#include "Model.h"

class NusaSpikeModel : public Model
{
public:
	ModelPart *spike_r1;
	ModelPart *spike_r2;

	void _init(float g);
    NusaSpikeModel();
    NusaSpikeModel(float g);
    virtual void render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled);
    virtual void setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim=0);

private:
    void setRotation(ModelPart *model, float x, float y, float z);
};