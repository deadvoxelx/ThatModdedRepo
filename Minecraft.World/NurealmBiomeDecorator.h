#include "BiomeDecorator.h"

class Level;
class Random;
class Feature;

class NurealmBiomeDecorator : public BiomeDecorator
{
public:
	NurealmBiomeDecorator(Biome *biome);

protected:
	Feature *endTowerFeature;
	Feature *nusaShrubFeature;

	virtual void decorate();

protected:
	void decorate(int count, Feature *feature);
	void decorateDepthSpan(int count, Feature *feature, int y0, int y1);
	void decorateDepthAverage(int count, Feature *feature, int yMid, int ySpan);
};
