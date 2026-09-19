#pragma once

#include "Tile.h"
#include "Facing.h"
#include "IconRegister.h"

#include <string>

class Random;

struct ModTileTraits
{
    Material *material = nullptr;
    const Tile::SoundType *sound = nullptr;
    std::wstring textureIconName;
    std::wstring topTextureIconName;
    std::wstring bottomTextureIconName;
    std::wstring placeholderIconName;
    float hardness = 1.5f;
    float resistance = -1.0f;
    int craftingCategory = 0;
    int itemMaterial = 0;
    bool transparent = false;

    int dropId = -1;
    int dropAux = 0;
    int dropMin = 1;
    int dropMax = 1;
    float dropChance = 1.0f;
    bool dropNothing = false;
};

class ModTile : public Tile {
private:
    std::wstring topIconName;
    Icon *topIcon = nullptr;
    std::wstring bottomIconName;
    Icon *bottomIcon = nullptr;
    bool transparent;
    int dropId;
    int dropAux;
    int dropMin;
    int dropMax;
    float dropChance;
    bool dropNothing;

public:
    ModTile(int id, const ModTileTraits& traits) : Tile(id, traits.material, !traits.transparent)
    {
        setIconName(traits.textureIconName.empty() ? traits.placeholderIconName : traits.textureIconName);
        topIconName = traits.topTextureIconName;
        bottomIconName = traits.bottomTextureIconName;
        transparent = traits.transparent;
        setDestroyTime(traits.hardness);

        if (traits.resistance >= 0.0f) {
            setExplodeable(traits.resistance);
        }

        if (traits.sound != nullptr) {
            setSoundType(traits.sound);
        }

        if (transparent)
        {
            setLightBlock(1);
        }

        dropId = traits.dropId;
        dropAux = traits.dropAux;
        dropMin = traits.dropMin;
        dropMax = traits.dropMax;
        dropChance = traits.dropChance;
        dropNothing = traits.dropNothing;

        setBaseItemTypeAndMaterial(traits.craftingCategory, traits.itemMaterial);
    }

    virtual int getResourceCount(Random *random)
    {
        if (dropNothing) return 0;
        if (dropMin >= dropMax) return dropMax;
        return dropMin + random->nextInt(dropMax - dropMin + 1);
    }

    virtual int getResource(int data, Random *random, int playerBonusLevel)
    {
        if (dropNothing) return 0;
        if (dropId <= 0) return id;
        if (dropChance >= 1.0f || random->nextFloat() < dropChance) return dropId;
        return 0;
    }

    virtual int getSpawnResourcesAuxValue(int data)
    {
        return (dropId > 0) ? dropAux : Tile::getSpawnResourcesAuxValue(data);
    }

    void setDropOverride(int newDropId, int newDropAux)
    {
        dropId = newDropId;
        dropAux = newDropAux;
    }

    virtual bool isSolidRender(bool isServerLevel = false)
    {
        return !transparent;
    }

    virtual bool blocksLight()
    {
        return transparent ? false : Tile::blocksLight();
    }

    virtual void registerIcons(IconRegister *iconRegister)
    {
        Tile::registerIcons(iconRegister);

        if (!topIconName.empty())
        {
            topIcon = iconRegister->registerIcon(topIconName);
        }

        if (!bottomIconName.empty())
        {
            bottomIcon = iconRegister->registerIcon(bottomIconName);
        }
    }

    virtual Icon *getTexture(int face, int data)
    {
        if (face == Facing::UP && topIcon != nullptr) return topIcon;
        if (face == Facing::DOWN && bottomIcon != nullptr) return bottomIcon;
        return icon;
    }
};
