#pragma once
#include "BiomeDecorator.h"
#include "HolidayTreeFeature.h"

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
	Feature *airFossilFeature;

	Feature *quicksoilShelfFeature;

	HolidayTreeFeature *holidayTreeFeature;

	Feature *largeAerCloudFeature;
	Feature *smallAerCloudFeature;
	Feature *smallGoldAerCloudFeature;
	Feature *smallBlueAerCloudFeature;

	Feature *whiteFlowerFeature;
	Feature *purpleFlowerFeature;

	virtual void decorate();
	void decorateAetherOres();
};
