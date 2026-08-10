#pragma once
#include "Feature.h"

class AerCloudFeature : public Feature
{
public:
	AerCloudFeature(int tile, int data, int cloudAmount);

	virtual bool place(Level *level, Random *random, int xIn, int yIn, int zIn);

private:
	int tile;
	int data;
	int cloudAmount;
};
