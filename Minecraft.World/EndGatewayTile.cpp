#include "stdafx.h"
#include "EndGatewayTile.h"
#include "EndGatewayTileEntity.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.storage.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.entity.player.h"
#include "net.minecraft.world.h"
#include "../Minecraft.Client/MinecraftServer.h"

DWORD EndGatewayTile::tlsIdx = TlsAlloc();

// 4J - allowAnywhere is a static in java, implementing as TLS here to make thread safe
bool EndGatewayTile::allowAnywhere()
{
	return (TlsGetValue(tlsIdx) != nullptr);
}

void EndGatewayTile::allowAnywhere(bool set)
{
	TlsSetValue(tlsIdx,(LPVOID)(set?1:0));
}

EndGatewayTile::EndGatewayTile(int id, Material *material)
    : BaseEntityTile(id, material, false)
{
    this->setLightEmission(1.0f);
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

void EndGatewayTile::entityInside(Level* level, int x, int y, int z, shared_ptr<Entity> entity)
{
    if (entity->GetType() == eTYPE_EXPERIENCEORB) return;

    if (level->getBiome(x, z) == Biome::sky)
	{
		if (entity->riding == nullptr && entity->rider.lock() == nullptr)
    	{
        	if (!level->isClientSide)
        	{
            	MinecraftServer* server = MinecraftServer::getInstance();
            	if (server == nullptr)
            	{
                	printf("MINECRAFT SERVER IS NULL\n");
                	return;
            	}

            	ServerLevel* target = server->getLevel(2);

            	if (target == nullptr)
            	{
                	printf("DIMENSION 2 IS NULL\n");
                	return; // STOP BEFORE CRASH
            	}
 
            	entity->changeDimension(2);

        	}
    	}
	}
	else 
	{
		if (level->getBiome(x, z) == Biome::outerIslands)
		{
			if (entity->riding == nullptr && entity->rider.lock() == nullptr)
    		{
        		if (!level->isClientSide)
        		{
            		MinecraftServer* server = MinecraftServer::getInstance();
            		if (server == nullptr)
            		{
                		printf("MINECRAFT SERVER IS NULL\n");
                		return;
            		}

            		ServerLevel* target = server->getLevel(1);

            		if (target == nullptr)
            		{
                		printf("DIMENSION 1 IS NULL\n");
                		return; // STOP BEFORE CRASH
            		}
 
            		entity->changeDimension(1);

        		}
    		}
		}
	}
	
}

void EndGatewayTile::animateTick(Level *level, int xt, int yt, int zt, Random *random)
{
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
	// don't register null, because of particles
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
