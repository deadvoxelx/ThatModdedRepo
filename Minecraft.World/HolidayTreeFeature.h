#pragma once
#include "Feature.h"

class HolidayTreeFeature : public Feature
{
public:
	HolidayTreeFeature(int blockId);

	virtual bool place(Level *level, Random *random, int x, int y, int z);
};
