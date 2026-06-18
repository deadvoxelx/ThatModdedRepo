#include "BiomeDecorator.h"
#include "EndGatewayFeature.h"
#include "OuterEndPlatformFeature.h"

class Level;
class Random;
class Feature;

class TheOuterEndBiomeDecorator : public BiomeDecorator
{
public:
	TheOuterEndBiomeDecorator(Biome *biome);

protected:
	EndGatewayFeature *endGatewayFeature;
	OuterEndPlatformFeature *outerEndPlatformFeature;
	Feature *endesertBushFeature;
	Feature *veloettShrubFeature;
	Feature *veloettFlowerFeature;
	Feature *endTowerFeature;

	virtual void decorate();

protected:
	void decorate(int count, Feature *feature);
	void decorateDepthSpan(int count, Feature *feature, int y0, int y1);
	void decorateDepthAverage(int count, Feature *feature, int yMid, int ySpan);
};
