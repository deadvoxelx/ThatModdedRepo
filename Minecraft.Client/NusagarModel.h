#pragma once
#include "Model.h"

class NusagarModel : public Model
{
public:
	ModelPart *body1;
	ModelPart *neck;
	ModelPart *head;
	ModelPart *body2;
	ModelPart *hipbone;
	ModelPart *legright1;
	ModelPart *legright2;
	ModelPart *legleft1;
	ModelPart *legleft2;
	ModelPart *tail1;
	ModelPart *tail2;
	ModelPart *tail3;
	ModelPart *tail4;
	ModelPart *armright1;
	ModelPart *armright2;
	ModelPart *armleft1;
	ModelPart *armleft2;

	void _init(float g);
    NusagarModel();
    NusagarModel(float g);
	int modelVersion();
    virtual void render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled);
    virtual void setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim=0);

private:
    void setRotation(ModelPart *model, float x, float y, float z);
};
