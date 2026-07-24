#pragma once
#include "Model.h"

class NusaDemonModel : public Model
{
public:
	ModelPart *head;
	ModelPart *rightHorn;
	ModelPart *rightHorn2;
	ModelPart *leftHorn;
	ModelPart *leftHorn2;
	ModelPart *body1;
	ModelPart *body2;
	ModelPart *armtopright;
	ModelPart *armtopleft;
	ModelPart *armmiddleright;
	ModelPart *armmiddleleft;
	ModelPart *armbottomright;
	ModelPart *armbottomleft;
	ModelPart *leg1;
	ModelPart *leg2;

	void _init(float g);
    NusaDemonModel();
    NusaDemonModel(float g);
	int modelVersion();
    virtual void render(shared_ptr<Entity> entity, float time, float r, float bob, float yRot, float xRot, float scale, bool usecompiled);
    virtual void setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim=0);

private:
    void setRotation(ModelPart *model, float x, float y, float z);
};