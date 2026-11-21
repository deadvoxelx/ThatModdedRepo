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
	std::string featureName;

	void grow(Level *level, int x, int y, int z, Random *random);

public:
	ModSaplingTile(int id, const std::wstring &iconName, Feature *treeFeature, const std::string &featureName = std::string());
	virtual ~ModSaplingTile();

	virtual void registerIcons(IconRegister *iconRegister);
	virtual void tick(Level *level, int x, int y, int z, Random *random);
	virtual bool canBonemeal(Level *level, int x, int y, int z);
	virtual bool bonemeal(Level *level, int x, int y, int z, Random *random);
};
