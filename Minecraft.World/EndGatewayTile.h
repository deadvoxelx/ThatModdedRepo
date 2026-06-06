#pragma once
#include "BaseEntityTile.h"

class Level;
class Entity;

class EndGatewayTile : public BaseEntityTile
{
public:
    EndGatewayTile(int id, Material* material);

    static DWORD tlsIdx;
    static bool allowAnywhere();
    static void allowAnywhere(bool set);
    virtual shared_ptr<TileEntity> newTileEntity(Level *level) override;
    virtual void entityInside(Level* level, int x, int y, int z, shared_ptr<Entity> entity) override;
    virtual void addAABBs(Level *level, int x, int y, int z, AABB *box, AABBList *boxes, shared_ptr<Entity> source) override;
    virtual int getResourceCount(Random *random) override;
    virtual void animateTick(Level *level, int xt, int yt, int zt, Random *random) override;
    virtual void onPlace(Level *level, int x, int y, int z) override;
    virtual int cloneTileId(Level *level, int x, int y, int z) override;
    virtual void registerIcons(IconRegister *iconRegister) override;

    virtual int getColor() const;
	virtual int getColor(int auxData);
	virtual int getColor(LevelSource *level, int x, int y, int z);
	virtual int getColor(LevelSource *level, int x, int y, int z, int data);
};
