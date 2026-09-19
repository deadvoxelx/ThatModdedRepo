#pragma once

#include "Bush.h"
#include "IconRegister.h"

#include <string>

class Feature;

class ModSaplingTile : public Bush
{
private:
	std::wstring iconName;
	Feature *treeFeature;

public:
	ModSaplingTile(int id, const std::wstring &iconName, Feature *treeFeature);
	virtual ~ModSaplingTile();

	virtual void registerIcons(IconRegister *iconRegister);
	virtual void tick(Level *level, int x, int y, int z, Random *random);
};
