#pragma once
#include "Tile.h"

class Level;
class Entity;

class NusaPortalTile : public Tile
{
public:
    NusaPortalTile(int id, Material* material);
    
    virtual void tick(Level *level, int x, int y, int z, Random *random);
    virtual void entityInside(Level* level, int x, int y, int z, shared_ptr<Entity> entity) override;
    virtual void addAABBs(Level *level, int x, int y, int z, AABB *box, AABBList *boxes, shared_ptr<Entity> source) override;
    virtual int getResourceCount(Random *random) override;
    virtual void animateTick(Level *level, int xt, int yt, int zt, Random *random) override;
    virtual int cloneTileId(Level *level, int x, int y, int z) override;
    virtual void registerIcons(IconRegister *iconRegister) override;
};