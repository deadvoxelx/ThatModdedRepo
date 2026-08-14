#include "stdafx.h"
#include "EndGatewayTile.h"
#include "EndGatewayTileEntity.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.storage.h"
#include "net.minecraft.world.level.dimension.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.h"
#include "../Minecraft.Client/MinecraftServer.h"

DWORD EndGatewayTile::tlsIdx = TlsAlloc();

bool EndGatewayTile::allowAnywhere()
{
	return (TlsGetValue(tlsIdx) != nullptr);
}

void EndGatewayTile::allowAnywhere(bool set)
{
	TlsSetValue(tlsIdx,(LPVOID)(set?1:0));
}

EndGatewayTile::EndGatewayTile(int id, Material *material) : BaseEntityTile(id, material, false)
{
    this->setLightEmission(1.0f);
	setTicking(true);
}

void EndGatewayTile::tick(Level *level, int x, int y, int z, Random *random)
{
	BaseEntityTile::tick(level, x, y, z, random);
}

shared_ptr<TileEntity> EndGatewayTile::newTileEntity(Level *level)
{
	return std::make_shared<EndGatewayTileEntity>();
}

void EndGatewayTile::addAABBs(Level *level, int x, int y, int z, AABB *box, AABBList *boxes, shared_ptr<Entity> source)
{
}

int EndGatewayTile::getResourceCount(Random *random)
{
	return 0;
}

int EndGatewayTile::getRenderShape()
{
	return SHAPE_INVISIBLE;
}

void EndGatewayTile::entityInside(Level* level, int x, int y, int z, shared_ptr<Entity> entity)
{
    if (entity->GetType() == eTYPE_EXPERIENCEORB) return;

	if (entity->riding == nullptr && entity->rider.lock() == nullptr) entity->handleInsideGateway();
}

void EndGatewayTile::animateTick(Level *level, int xt, int yt, int zt, Random *random)
{
	if (random->nextInt(100) == 0)
	{
		level->playLocalSound(xt + 0.5, yt + 0.5, zt + 0.5, eSoundType_PORTAL_PORTAL, 0.5f, random->nextFloat() * 0.4f + 0.8f, false);
	}

	double x = xt + random->nextFloat();
	double y = yt + 0.8f;
	double z = zt + random->nextFloat();
	double xa = 0;
	double ya = 0;
	double za = 0;

	level->addParticle(eParticleType_endportal, x, y, z, xa, ya, za);
}

void EndGatewayTile::onPlace(Level *level, int x, int y, int z)
{
	if (allowAnywhere()) return;
}

int EndGatewayTile::cloneTileId(Level *level, int x, int y, int z)
{
	return 0;
}

void EndGatewayTile::registerIcons(IconRegister *iconRegister)
{
	icon = iconRegister->registerIcon(L"portal");
}

int EndGatewayTile::getColor() const
{
	return 0x000000;
}

int EndGatewayTile::getColor(int auxData)
{
	return 0x000000;
}

int EndGatewayTile::getColor(LevelSource *level, int x, int y, int z)
{
	return 0x000000;
}

int EndGatewayTile::getColor(LevelSource *level, int x, int y, int z, int data)
{
	return 0x000000;
}
