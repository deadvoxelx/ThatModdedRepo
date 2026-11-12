#pragma once

#include "Layer.h"

class RubyBiomeLayer : public Layer
{
public:
	RubyBiomeLayer(int64_t seed, shared_ptr<Layer> parent);

	virtual intArray getArea(int xo, int yo, int w, int h);

private:
	int gridValue(int gx, int gz);
	float sample(int x, int z, int cells);
};
