#pragma once
#include "Feature.h"

class LavenderTreeFeature : public Feature
{
public:
	LavenderTreeFeature(bool doUpdate);

	virtual bool place(Level *level, Random *random, int x, int y, int z);
};