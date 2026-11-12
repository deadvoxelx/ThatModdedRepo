#pragma once

#include "Dimension.h"
#include "Registry/Dimension/DimensionRegistry.h"

class ChunkSource;

class RubyDimension : public Dimension
{
public:
	explicit RubyDimension(const DimensionDefinition &definition);

	virtual void init();
	virtual ChunkSource *createRandomLevelSource() const;
	virtual bool isValidSpawn(int x, int z) const;
	virtual bool mayRespawn() const;
	virtual Pos *getSpawnPos();
	virtual int getXZSize();
	virtual int getSeaLevelOverride();
	virtual bool isNaturalDimension();
	virtual float getTimeOfDay(int64_t time, float a) const;
	virtual float *getSunriseColor(float td, float a);

private:
	DimensionDefinition m_definition;
};
