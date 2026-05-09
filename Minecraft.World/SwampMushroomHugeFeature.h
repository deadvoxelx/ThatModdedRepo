#pragma once
#include "Feature.h"

class SwampMushroomHugeFeature : public Feature
{
private:
	int forcedType;

public:
	SwampMushroomHugeFeature(int forcedType);
	SwampMushroomHugeFeature();
	virtual bool place(Level *level, Random *random, int x, int y, int z);
};
