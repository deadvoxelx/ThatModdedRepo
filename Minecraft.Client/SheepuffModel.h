#pragma once
#include "QuadrupedModel.h"

class SheepuffModel : public QuadrupedModel
{
private:
	float headXRot;
public:
	SheepuffModel();

	virtual void prepareMobModel(shared_ptr<LivingEntity> mob, float time, float r, float a);
	virtual void setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim=0);
};