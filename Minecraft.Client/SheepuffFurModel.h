#pragma once
#include "QuadrupedModel.h"

class LivingEntity;

class SheepuffFurModel : public QuadrupedModel
{
private:
	float headXRot;
public:
	SheepuffFurModel();

	virtual void prepareMobModel(shared_ptr<LivingEntity> mob, float time, float r, float a);
	virtual void setupAnim(float time, float r, float bob, float yRot, float xRot, float scale, shared_ptr<Entity> entity, unsigned int uiBitmaskOverrideAnim=0);
};