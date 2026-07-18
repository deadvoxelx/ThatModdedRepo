#pragma once
#include "Feature.h"

class NusaTreeFeature : public Feature
{
public:
	NusaTreeFeature(bool doUpdate);

	virtual bool place(Level *level, Random *random, int x, int y, int z);
	virtual bool foliage(Level *level, Random *random, int x, int y, int z);
};