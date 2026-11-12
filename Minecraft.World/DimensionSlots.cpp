#include "stdafx.h"
#include "DimensionSlots.h"
#include <map>
#include <vector>

namespace
{
	std::vector<int> &slots()
	{
		static std::vector<int> ids = { 0, -1, 1, 2, 3, 4 };
		return ids;
	}

	std::map<int, int> &sizes()
	{
		static std::map<int, int> registered;
		return registered;
	}
}

const std::vector<int> &DimensionSlots::ids()
{
	return slots();
}

int DimensionSlots::count()
{
	return static_cast<int>(slots().size());
}

int DimensionSlots::idAt(int index)
{
	if (index < 0 || index >= count()) return 0;
	return slots()[index];
}

int DimensionSlots::indexOf(int id)
{
	const std::vector<int> &list = slots();
	for (size_t i = 0; i < list.size(); ++i)
	{
		if (list[i] == id) return static_cast<int>(i);
	}
	return -1;
}

bool DimensionSlots::add(int id, int xzSize)
{
	if (xzSize > 0) sizes()[id] = xzSize;

	if (indexOf(id) >= 0) return false;

	slots().push_back(id);
	return true;
}

int DimensionSlots::levelXZSize(int id)
{
	std::map<int, int>::const_iterator it = sizes().find(id);
	return it == sizes().end() ? 0 : it->second;
}
