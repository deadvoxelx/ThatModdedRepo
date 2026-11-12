#pragma once

#include "Bush.h"
#include "IconRegister.h"
#include <string>
#include <vector>

class ModPlantTile : public Bush
{
private:
    std::wstring iconName;
    std::vector<int> soil;
    int dropId = -1;
    int dropAux = 0;
    bool dropNothing = false;

public:
    ModPlantTile(int id, const std::wstring &iconName);

    virtual void registerIcons(IconRegister *iconRegister);
    virtual void updateDefaultShape();
    virtual int getResourceCount(Random *random);
    virtual int getResource(int data, Random *random, int playerBonusLevel);
    virtual int getSpawnResourcesAuxValue(int data);

    void setSoil(const std::vector<int> &blocks);
    void setDrop(int newDropId, int newDropAux);
    void setDropNothing(bool value);

protected:
    virtual bool mayPlaceOn(int tile);
};
