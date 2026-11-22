#pragma once

#include "ModTile.h"
#include "HalfSlabTile.h"
#include "StairTile.h"
#include "FenceTile.h"
#include "FenceGateTile.h"
#include "ItemInstance.h"

class ModSlabTile : public HalfSlabTile, public ModDropSink
{
private:
    ModDrops drops;
    ModIcons icons;
    int halfId;
    int fullId;

public:
    ModSlabTile(int id, bool fullSize, int halfTileId, int fullTileId, const ModTileTraits &traits) : HalfSlabTile(id, fullSize, traits.material)
    {
        halfId = halfTileId;
        fullId = fullTileId;

        drops.load(traits);
        icons.load(traits);

        setIconName(traits.textureIconName.empty() ? traits.placeholderIconName : traits.textureIconName);
        setDestroyTime(traits.hardness);

        if (traits.resistance >= 0.0f)
        {
            setExplodeable(traits.resistance);
        }

        if (traits.sound != nullptr)
        {
            setSoundType(traits.sound);
        }

        setBaseItemTypeAndMaterial(traits.craftingCategory, traits.itemMaterial);
        modApplyLight(id, traits);
    }

    virtual int getAuxName(int auxValue)
    {
        return (int)getDescriptionId();
    }

    virtual bool isHalfSlabTile(int tileId) const
    {
        return tileId == halfId || tileId == fullId;
    }

    virtual int cloneTileId(Level *level, int x, int y, int z)
    {
        return halfId;
    }

    virtual int getResourceCount(Random *random)
    {
        return drops.dropNothing ? 0 : HalfSlabTile::getResourceCount(random);
    }

    virtual int getResource(int data, Random *random, int playerBonusLevel)
    {
        return drops.resource(halfId, random);
    }

    virtual int getSpawnResourcesAuxValue(int data)
    {
        return drops.spawnAux(Tile::getSpawnResourcesAuxValue(data));
    }

    virtual void setDropOverride(int newDropId, int newDropAux)
    {
        drops.dropId = newDropId;
        drops.dropAux = newDropAux;
    }

    virtual void registerIcons(IconRegister *iconRegister)
    {
        Tile::registerIcons(iconRegister);
        icons.registerIcons(iconRegister);
    }

    virtual Icon *getTexture(int face, int data)
    {
        return icons.texture(face, icon);
    }

protected:
    virtual shared_ptr<ItemInstance> getSilkTouchItemInstance(int data)
    {
        return std::make_shared<ItemInstance>(halfId, fullSize ? 2 : 1, data & TYPE_MASK);
    }
};

class ModStairTile : public StairTile, public ModDropSink
{
private:
    ModDrops drops;
    ModIcons icons;

public:
    ModStairTile(int id, const ModTileTraits &traits) : StairTile(id, Tile::wood, 0, traits.material)
    {
        drops.load(traits);
        icons.load(traits);

        setIconName(traits.textureIconName.empty() ? traits.placeholderIconName : traits.textureIconName);
        setDestroyTime(traits.hardness);

        if (traits.resistance >= 0.0f)
        {
            setExplodeable(traits.resistance);
        }

        if (traits.sound != nullptr)
        {
            setSoundType(traits.sound);
        }

        setBaseItemTypeAndMaterial(traits.craftingCategory, traits.itemMaterial);
        modApplyLight(id, traits);
    }

    virtual float getExplosionResistance(shared_ptr<Entity> source)
    {
        return explosionResistance;
    }

    virtual int getResourceCount(Random *random)
    {
        return drops.resourceCount(random);
    }

    virtual int getResource(int data, Random *random, int playerBonusLevel)
    {
        return drops.resource(id, random);
    }

    virtual int getSpawnResourcesAuxValue(int data)
    {
        return drops.spawnAux(Tile::getSpawnResourcesAuxValue(data));
    }

    virtual void setDropOverride(int newDropId, int newDropAux)
    {
        drops.dropId = newDropId;
        drops.dropAux = newDropAux;
    }

    virtual void registerIcons(IconRegister *iconRegister)
    {
        Tile::registerIcons(iconRegister);
        icons.registerIcons(iconRegister);
    }

    virtual Icon *getTexture(int face, int data)
    {
        return icons.texture(face, icon);
    }
};

class ModFenceTile : public FenceTile, public ModDropSink
{
private:
    ModDrops drops;
    ModIcons icons;

public:
    ModFenceTile(int id, const ModTileTraits &traits) : FenceTile(id, Tile::wood, traits.material)
    {
        drops.load(traits);
        icons.load(traits);

        setIconName(traits.textureIconName.empty() ? traits.placeholderIconName : traits.textureIconName);
        setDestroyTime(traits.hardness);

        if (traits.resistance >= 0.0f)
        {
            setExplodeable(traits.resistance);
        }

        if (traits.sound != nullptr)
        {
            setSoundType(traits.sound);
        }

        setBaseItemTypeAndMaterial(traits.craftingCategory, traits.itemMaterial);
        modApplyLight(id, traits);
    }

    virtual int getResourceCount(Random *random)
    {
        return drops.resourceCount(random);
    }

    virtual int getResource(int data, Random *random, int playerBonusLevel)
    {
        return drops.resource(id, random);
    }

    virtual int getSpawnResourcesAuxValue(int data)
    {
        return drops.spawnAux(Tile::getSpawnResourcesAuxValue(data));
    }

    virtual void setDropOverride(int newDropId, int newDropAux)
    {
        drops.dropId = newDropId;
        drops.dropAux = newDropAux;
    }

    virtual void registerIcons(IconRegister *iconRegister)
    {
        Tile::registerIcons(iconRegister);
        icons.registerIcons(iconRegister);
    }

    virtual Icon *getTexture(int face, int data)
    {
        return icons.texture(face, icon);
    }
};

class ModFenceGateTile : public FenceGateTile, public ModDropSink
{
private:
    ModDrops drops;
    ModIcons icons;

public:
    ModFenceGateTile(int id, const ModTileTraits &traits) : FenceGateTile(id, traits.material)
    {
        drops.load(traits);
        icons.load(traits);

        setIconName(traits.textureIconName.empty() ? traits.placeholderIconName : traits.textureIconName);
        setDestroyTime(traits.hardness);

        if (traits.resistance >= 0.0f)
        {
            setExplodeable(traits.resistance);
        }

        if (traits.sound != nullptr)
        {
            setSoundType(traits.sound);
        }

        setBaseItemTypeAndMaterial(traits.craftingCategory, traits.itemMaterial);
        modApplyLight(id, traits);
    }

    virtual int getResourceCount(Random *random)
    {
        return drops.resourceCount(random);
    }

    virtual int getResource(int data, Random *random, int playerBonusLevel)
    {
        return drops.resource(id, random);
    }

    virtual int getSpawnResourcesAuxValue(int data)
    {
        return drops.spawnAux(Tile::getSpawnResourcesAuxValue(data));
    }

    virtual void setDropOverride(int newDropId, int newDropAux)
    {
        drops.dropId = newDropId;
        drops.dropAux = newDropAux;
    }

    virtual void registerIcons(IconRegister *iconRegister)
    {
        Tile::registerIcons(iconRegister);
        icons.registerIcons(iconRegister);
    }

    virtual Icon *getTexture(int face, int data)
    {
        return icons.texture(face, icon);
    }
};
