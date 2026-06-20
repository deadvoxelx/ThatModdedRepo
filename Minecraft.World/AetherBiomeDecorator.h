#pragma once
#include "BiomeDecorator.h"

class AerCloudFeature;

class AetherBiomeDecorator : public BiomeDecorator
{
public:
	AetherBiomeDecorator(Biome *biome);

protected:
	Feature *ambrosiumOreFeature;
	Feature *zaniteOreFeature;
	Feature *gravititeOreFeature;
	Feature *icestoneFeature;
	Feature *mossyHolystoneFeature;

	Feature *quicksoilShelfFeature;

	Feature *holidayTreeFeature;

	Feature *largeAerCloudFeature;
	Feature *smallAerCloudFeature;
	Feature *smallGoldAerCloudFeature;
	Feature *smallBlueAerCloudFeature;

	Feature *whiteFlowerFeature;
	Feature *purpleFlowerFeature;

	virtual void decorate();
	void decorateAetherOres();
};
