#pragma once
#include "Feature.h"

class HolidayTreeFeature : public Feature
{
public:
	HolidayTreeFeature(bool doUpdate);

	virtual bool place(Level *level, Random *random, int x, int y, int z);
};