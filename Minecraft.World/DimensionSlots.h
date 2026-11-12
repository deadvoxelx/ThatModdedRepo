#pragma once

#include <vector>

namespace DimensionSlots
{
	const std::vector<int> &ids();

	int count();
	int idAt(int index);
	int indexOf(int id);

	bool add(int id, int xzSize);

	int levelXZSize(int id);
}
