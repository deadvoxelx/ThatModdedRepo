#pragma once
#include "HalfTransparentTile.h"

class AercloudTile : public HalfTransparentTile
{
	friend class Tile;
public:
	static const int TYPE_DEFAULT = 0;
	static const int TYPE_BLUE = 1;
	static const int TYPE_GOLD = 2;

	static const int CLOUD_NAMES_LENGTH = 3;

	static const unsigned int CLOUD_NAMES[CLOUD_NAMES_LENGTH];
	static const wstring TEXTURE_NAMES[];

private:
	Icon** icons;

protected:
	AercloudTile(int id, Material *material, bool allowSame);

public:
	virtual AABB *getAABB(Level *level, int x, int y, int z);
	virtual void fallOn(Level *level, int x, int y, int z, shared_ptr<Entity> entity, float fallDistance);
	virtual void entityInside(Level *level, int x, int y, int z, shared_ptr<Entity> entity);
	virtual bool isSolidRender(bool isServerLevel = false);
	virtual int getSpawnResourcesAuxValue(int data);
	virtual int getRenderLayer();
	virtual bool shouldRenderFace(LevelSource *level, int x, int y, int z, int face);

	virtual int getColor() const;
	virtual int getColor(int auxData);
	virtual int getColor(LevelSource *level, int x, int y, int z);
	virtual int getColor(LevelSource *level, int x, int y, int z, int data);

	virtual Icon* getTexture(int face, int data);
	void registerIcons(IconRegister* iconRegister);
};