#pragma once
#include "Model.h"

class AerwhaleModel : public Model
{
public:
	ModelPart *head, *bottomPartHead, *middleBody, *bottomPartMiddlebody, *frontBody, *backBody, *rightFin, *leftFin, *middleFin, *backfinRight, *backfinLeft;

	void _init(float g);
    AerwhaleModel();
    AerwhaleModel(float g);
    virtual void render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled);
    virtual void setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim=0);

private:
	void setRotation(ModelPart *model, float x, float y, float z);
};
