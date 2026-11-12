#include "stdafx.h"
#include "net.minecraft.world.level.newbiome.layer.h"
#include "net.minecraft.world.level.biome.h"
#include "IntCache.h"
#include "../RubyLauncher/Registry/Biome/BiomeRegistry.h"
#include <vector>

namespace
{
	const int BLOCKS_PER_CELL = 4;

	const int kCoverageThresholds[] =
	{
		96, 100, 104, 108, 112, 116, 120, 124, 128, 132,
		136, 140, 144, 148, 152, 156, 160, 164, 168, 172,
		176, 180, 184, 188, 192, 196, 200, 204, 208, 212,
		216, 220, 224, 228, 232, 236, 240, 244, 248, 252
	};

	const float kCoveragePercents[] =
	{	// These might seem random, but theyre actually what MC's noise params produce; its complicated, but theres logic to it
		71.80f, 69.14f, 66.46f, 63.87f, 61.01f, 58.12f, 55.35f, 52.38f, 49.47f, 46.85f,
		43.91f, 40.98f, 38.15f, 35.38f, 32.58f, 30.11f, 27.55f, 25.06f, 22.60f, 20.38f,
		18.17f, 16.17f, 14.18f, 12.30f, 10.74f, 9.08f, 7.62f, 6.36f, 5.21f, 4.13f,
		3.27f, 2.49f, 1.81f, 1.23f, 0.84f, 0.50f, 0.29f, 0.12f, 0.04f, 0.00f
	};

	const int kCoverageSteps = static_cast<int>(sizeof(kCoverageThresholds) / sizeof(kCoverageThresholds[0]));

	int floorDiv(int value, int divisor)
	{
		return value >= 0 ? value / divisor : -((-value + divisor - 1) / divisor);
	}

	bool isRubyBiomeTarget(int biomeId)
	{
		if (biomeId == Biome::ocean->id || biomeId == Biome::frozenOcean->id) return false;
		if (biomeId == Biome::river->id || biomeId == Biome::frozenRiver->id) return false;
		if (biomeId == Biome::beaches->id) return false;
		if (biomeId == Biome::mushroomIsland->id || biomeId == Biome::mushroomIslandShore->id) return false;
		if (biomeId == Biome::hell->id || biomeId == Biome::sky->id) return false;
		return true;
	}

	int thresholdForFrequency(float percent)
	{
		const int last = kCoverageSteps - 1;
		if (percent >= kCoveragePercents[0]) return kCoverageThresholds[0];
		if (percent <= kCoveragePercents[last]) return kCoverageThresholds[last];

		for (int i = 1; i < kCoverageSteps; i++)
		{
			if (percent > kCoveragePercents[i]) continue;

			const float upperPercent = kCoveragePercents[i - 1];
			const float lowerPercent = kCoveragePercents[i];
			const float span = upperPercent - lowerPercent;
			const float t = span > 0.0f ? (upperPercent - percent) / span : 0.0f;

			const int lowerThreshold = kCoverageThresholds[i - 1];
			const int upperThreshold = kCoverageThresholds[i];
			return lowerThreshold + static_cast<int>(t * (upperThreshold - lowerThreshold) + 0.5f);
		}
		return kCoverageThresholds[last];
	}
}

RubyBiomeLayer::RubyBiomeLayer(int64_t seed, shared_ptr<Layer> parent) : Layer(seed)
{
	this->parent = parent;
}

int RubyBiomeLayer::gridValue(int gx, int gz)
{
	initRandom(gx * 341873128712L, gz * 132897987541L);
	return nextRandom(256);
}

float RubyBiomeLayer::sample(int x, int z, int cells)
{
	const int gx = floorDiv(x, cells);
	const int gz = floorDiv(z, cells);
	const float fx = (x - gx * cells) / (float)cells;
	const float fz = (z - gz * cells) / (float)cells;

	const int n00 = gridValue(gx, gz);
	const int n10 = gridValue(gx + 1, gz);
	const int n01 = gridValue(gx, gz + 1);
	const int n11 = gridValue(gx + 1, gz + 1);

	const float north = n00 + (n10 - n00) * fx;
	const float south = n01 + (n11 - n01) * fx;
	return north + (south - north) * fz;
}

intArray RubyBiomeLayer::getArea(int xo, int yo, int w, int h)
{
	intArray parentArea = parent->getArea(xo, yo, w, h);

	const std::vector<BiomePlacement> &placements = BiomeRegistry::customBiomePlacements();
	if (placements.empty()) return parentArea;

	const int placementCount = static_cast<int>(placements.size());
	std::vector<int> cells(placementCount);
	std::vector<int> thresholds(placementCount);

	for (int i = 0; i < placementCount; i++)
	{
		cells[i] = placements[i].size / BLOCKS_PER_CELL;
		if (cells[i] < 1) cells[i] = 1;
		thresholds[i] = thresholdForFrequency(placements[i].frequency);
	}

	intArray result = IntCache::allocate(w * h);

	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			const int index = x + y * w;
			const int old = parentArea[index];
			result[index] = old;

			if (!isRubyBiomeTarget(old)) continue;

			for (int i = 0; i < placementCount; i++)
			{
				if (sample(x + xo, y + yo, cells[i]) >= thresholds[i])
				{
					result[index] = placements[i].biomeId;
					break;
				}
			}
		}
	}
	return result;
}
