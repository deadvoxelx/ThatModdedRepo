#include "stdafx.h"
#include "NusaPortalTile.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.storage.h"
#include "net.minecraft.world.level.dimension.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.h"
#include "../Minecraft.Client/MinecraftServer.h"

NusaPortalTile::NusaPortalTile(int id, Material *material) : Tile(id, material)
{
    this->setLightEmission(1.0f);
	this->setDestroyTime(-1);
	this->setExplodeable(6000000);
	setTicking(true);
}

void NusaPortalTile::tick(Level *level, int x, int y, int z, Random *random)
{
	Tile::tick(level, x, y, z, random);
}

void NusaPortalTile::addAABBs(Level *level, int x, int y, int z, AABB *box, AABBList *boxes, shared_ptr<Entity> source)
{
}

int NusaPortalTile::getResourceCount(Random *random)
{
	return 0;
}

void NusaPortalTile::entityInside(Level* level, int x, int y, int z, shared_ptr<Entity> entity)
{
    if (entity->GetType() == eTYPE_EXPERIENCEORB) return;

	if (entity->riding == nullptr && entity->rider.lock() == nullptr) entity->handleInsideGateway();
}

void NusaPortalTile::animateTick(Level *level, int xt, int yt, int zt, Random *random)
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

int NusaPortalTile::cloneTileId(Level *level, int x, int y, int z)
{
	return 0;
}

void NusaPortalTile::registerIcons(IconRegister *iconRegister)
{
	icon = iconRegister->registerIcon(L"nusa_portal");
}