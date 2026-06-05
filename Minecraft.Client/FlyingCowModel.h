/*#pragma once
//#include "Model.h"
#include "QuadrupedModel.h"

class FlyingCowModel : public QuadrupedModel
{
public:
	ModelPart *head;
    ModelPart *body;
    ModelPart *leg0;
    ModelPart *leg1;
    ModelPart *leg2;
    ModelPart *leg3;
    ModelPart *wingL;
    ModelPart *wingL2;
    ModelPart *wingR;
    ModelPart *wingR2;

	void _init(float g);
    FlyingCowModel();
    FlyingCowModel(float g);
    virtual void render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled);
    virtual void setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim=0);

private:
	void setRotation(ModelPart *model, float x, float y, float z);
};*/

#pragma once
#include "QuadrupedModel.h"

class FlyingCowModel : public QuadrupedModel 
{
public:
    ModelPart *wingL, *wingR, *wingL2, *wingR2;

    FlyingCowModel();
    virtual void render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled);

private:
	void setRotation(ModelPart *model, float x, float y, float z);
};
