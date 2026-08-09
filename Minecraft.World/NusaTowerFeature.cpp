#include "NusaTowerFeature.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.material.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.tile.entity.h"
#include "net.minecraft.world.item.h"
#include "WeighedTreasure.h"

NusaTowerFeature::NusaTowerFeature(int blockId) : Feature(blockId)
{
}

bool NusaTowerFeature::place(Level *level, Random *random, int x, int y, int z)
{
    int belowTile = level->getTile(x, y - 1, z);
    int thisTile = level->getTile(x, y, z);
	{
		if ((belowTile == Tile::nustone_Id || belowTile == Tile::nugrass_Id) && (thisTile != Tile::nustone_Id && thisTile != Tile::nugrass_Id && thisTile != Tile::nusaLog_Id && thisTile != Tile::netherLeaves_Id))
		{
		//Fix for End Tower being filled with End Stone sometimes; Clear space up to roof height
			for (int dx = -3; dx <= 3; dx++)
			{
				for (int dy = 0; dy <= 28; dy++)
				{
					for (int dz = -3; dz <= 3; dz++)
					{
						placeBlock(level, x + dx, y + dy, z + dz, 0, 0);
					}
				}
			}

		//UnderLayer
			for (int fx = -4; fx <= 4; fx++)
			{
				for (int fz = -2; fz <= 2; fz++)
				{
					placeBlock(level, x + fx, y - 1, z + fz, Tile::nustone_Id, 0);
				}
			}
			for (int gx = -2; gx <= 2; gx++)
			{
				for (int gz = -4; gz <= 4; gz++)
				{
					placeBlock(level, x + gx, y - 1, z + gz, Tile::nustone_Id, 0);
				}
			}

			placeBlock(level, x + 3, y - 1, z + 3, Tile::nustone_Id, 0);
			placeBlock(level, x - 3, y - 1, z - 3, Tile::nustone_Id, 0);
			placeBlock(level, x + 3, y - 1, z - 3, Tile::nustone_Id, 0);
			placeBlock(level, x - 3, y - 1, z + 3, Tile::nustone_Id, 0);
	
		//Floor 1
			for (int hx = -3; hx <= 3; hx++)
			{
				for (int hz = -2; hz <= 2; hz++)
				{
					placeBlock(level, x + hx, y, z + hz, Tile::netherPlanks_Id, 3);
				}
			}
			for (int ix = -2; ix <= 2; ix++)
			{
				for (int iz = -3; iz <= 3; iz++)
				{
					placeBlock(level, x + ix, y, z + iz, Tile::netherPlanks_Id, 3);
				}
			}
		
			placeBlock(level, x + 3, y, z + 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x - 3, y, z - 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x + 3, y, z - 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x - 3, y, z + 3, Tile::nusaLog_Id, 0);

			placeBlock(level, x + 4, y, z + 2, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y, z - 2, Tile::nustone_Id, 1);
			placeBlock(level, x + 4, y, z - 2, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y, z + 2, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 4, y, z + 1, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y, z - 1, Tile::nustone_Id, 1);
			placeBlock(level, x + 4, y, z - 1, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y, z + 1, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 2, y, z + 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 2, y, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x + 2, y, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 2, y, z + 4, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 1, y, z + 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 1, y, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x + 1, y, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 1, y, z + 4, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 4, y, z, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y, z, Tile::nustone_Id, 1);
			placeBlock(level, x, y, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x, y, z + 4, Tile::nustone_Id, 1);
		
		//Floor 2
			for (int jx = -3; jx <= 3; jx++)
			{
				for (int jz = -2; jz <= 2; jz++)
				{
					placeBlock(level, x + jx, y + 4, z + jz, Tile::netherPlanks_Id, 3);
				}
			}
			for (int kx = -2; kx <= 2; kx++)
			{
				for (int kz = -3; kz <= 3; kz++)
				{
					placeBlock(level, x + kx, y + 4, z + kz, Tile::netherPlanks_Id, 3);
				}
			}
			for (int lx = -1; lx <= 1; lx++)
			{
				for (int lz = -1; lz <= 1; lz++)
				{
					placeBlock(level, x + lx, y + 4, z + lz, 0, 0);
				}
			}
		
			placeBlock(level, x + 3, y + 4, z + 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x - 3, y + 4, z - 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x + 3, y + 4, z - 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x - 3, y + 4, z + 3, Tile::nusaLog_Id, 0);

			placeBlock(level, x + 4, y + 4, z + 2, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 4, z - 2, Tile::nustone_Id, 1);
			placeBlock(level, x + 4, y + 4, z - 2, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 4, z + 2, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 4, y + 4, z + 1, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 4, z - 1, Tile::nustone_Id, 1);
			placeBlock(level, x + 4, y + 4, z - 1, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 4, z + 1, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 2, y + 4, z + 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 2, y + 4, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x + 2, y + 4, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 2, y + 4, z + 4, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 1, y + 4, z + 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 1, y + 4, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x + 1, y + 4, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 1, y + 4, z + 4, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 4, y + 4, z, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 4, z, Tile::nustone_Id, 1);
			placeBlock(level, x, y + 4, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x, y + 4, z + 4, Tile::nustone_Id, 1);
		
		
		
			placeBlock(level, x + 5, y + 4, z + 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 4, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 4, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 4, z + 2, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 4, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 4, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 4, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 4, z + 1, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 2, y + 4, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 4, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 4, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 4, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 1, y + 4, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 4, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 1, y + 4, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 4, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 4, z, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 4, z, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 4, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 4, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 4, y + 4, z + 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 4, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x + 4, y + 4, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 4, z + 3, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 3, y + 4, z + 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 4, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x + 3, y + 4, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 4, z + 4, Tile::nustone_Id, 2);
		
		//Floor 3
			for (int mx = -3; mx <= 3; mx++)
			{
				for (int mz = -2; mz <= 2; mz++)
				{
					placeBlock(level, x + mx, y + 8, z + mz, Tile::netherPlanks_Id, 3);
				}
			}
			for (int nx = -2; nx <= 2; nx++)
			{
				for (int nz = -3; nz <= 3; nz++)
				{
					placeBlock(level, x + nx, y + 8, z + nz, Tile::netherPlanks_Id, 3);
				}
			}
			for (int ox = -1; ox <= 1; ox++)
			{
				for (int oz = -1; oz <= 1; oz++)
				{
					placeBlock(level, x + ox, y + 8, z + oz, 0, 0);
				}
			}
		
			placeBlock(level, x + 3, y + 8, z + 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x - 3, y + 8, z - 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x + 3, y + 8, z - 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x - 3, y + 8, z + 3, Tile::nusaLog_Id, 0);

			placeBlock(level, x + 4, y + 8, z + 2, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 8, z - 2, Tile::nustone_Id, 1);
			placeBlock(level, x + 4, y + 8, z - 2, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 8, z + 2, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 4, y + 8, z + 1, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 8, z - 1, Tile::nustone_Id, 1);
			placeBlock(level, x + 4, y + 8, z - 1, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 8, z + 1, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 2, y + 8, z + 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 2, y + 8, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x + 2, y + 8, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 2, y + 8, z + 4, Tile::nustone_Id, 1);
			
			placeBlock(level, x + 1, y + 8, z + 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 1, y + 8, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x + 1, y + 8, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 1, y + 8, z + 4, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 4, y + 8, z, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 8, z, Tile::nustone_Id, 1);
			placeBlock(level, x, y + 8, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x, y + 8, z + 4, Tile::nustone_Id, 1);
		
		
		
			placeBlock(level, x + 5, y + 8, z + 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 8, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 8, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 8, z + 2, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 8, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 8, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 8, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 8, z + 1, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 2, y + 8, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 8, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 8, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 8, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 1, y + 8, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 8, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 1, y + 8, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 8, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 8, z, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 8, z, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 8, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 8, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 4, y + 8, z + 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 8, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x + 4, y + 8, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 8, z + 3, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 3, y + 8, z + 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 8, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x + 3, y + 8, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 8, z + 4, Tile::nustone_Id, 2);
		
		//Floor 4
			for (int px = -3; px <= 3; px++)
			{
				for (int pz = -2; pz <= 2; pz++)
				{
					placeBlock(level, x + px, y + 12, z + pz, Tile::netherPlanks_Id, 3);
				}
			}
			for (int qx = -2; qx <= 2; qx++)
			{
				for (int qz = -3; qz <= 3; qz++)
				{
					placeBlock(level, x + qx, y + 12, z + qz, Tile::netherPlanks_Id, 3);
				}
			}
			for (int rx = -1; rx <= 1; rx++)
			{
				for (int rz = -1; rz <= 1; rz++)
				{
					placeBlock(level, x + rx, y + 12, z + rz, 0, 0);
				}
			}
		
			placeBlock(level, x + 3, y + 12, z + 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x - 3, y + 12, z - 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x + 3, y + 12, z - 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x - 3, y + 12, z + 3, Tile::nusaLog_Id, 0);

			placeBlock(level, x + 4, y + 12, z + 2, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 12, z - 2, Tile::nustone_Id, 1);
			placeBlock(level, x + 4, y + 12, z - 2, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 12, z + 2, Tile::nustone_Id, 1);
			
			placeBlock(level, x + 4, y + 12, z + 1, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 12, z - 1, Tile::nustone_Id, 1);
			placeBlock(level, x + 4, y + 12, z - 1, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 12, z + 1, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 2, y + 12, z + 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 2, y + 12, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x + 2, y + 12, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 2, y + 12, z + 4, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 1, y + 12, z + 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 1, y + 12, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x + 1, y + 12, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 1, y + 12, z + 4, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 4, y + 12, z, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 12, z, Tile::nustone_Id, 1);
			placeBlock(level, x, y + 12, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x, y + 12, z + 4, Tile::nustone_Id, 1);
		
		
		
			placeBlock(level, x + 5, y + 12, z + 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 12, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 12, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 12, z + 2, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 12, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 12, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 12, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 12, z + 1, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 2, y + 12, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 12, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 12, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 12, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 1, y + 12, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 12, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 1, y + 12, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 12, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 12, z, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 12, z, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 12, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 12, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 4, y + 12, z + 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 12, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x + 4, y + 12, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 12, z + 3, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 3, y + 12, z + 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 12, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x + 3, y + 12, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 12, z + 4, Tile::nustone_Id, 2);
		
		//Floor 5
			for (int sx = -3; sx <= 3; sx++)
			{
				for (int sz = -2; sz <= 2; sz++)
				{
					placeBlock(level, x + sx, y + 16, z + sz, Tile::netherPlanks_Id, 3);
				}
			}
			for (int tx = -2; tx <= 2; tx++)
			{
				for (int tz = -3; tz <= 3; tz++)
				{
					placeBlock(level, x + tx, y + 16, z + tz, Tile::netherPlanks_Id, 3);
				}
			}
			for (int ux = -1; ux <= 1; ux++)
			{
				for (int uz = -1; uz <= 1; uz++)
				{
					placeBlock(level, x + ux, y + 16, z + uz, 0, 0);
				}
			}
		
			placeBlock(level, x + 3, y + 16, z + 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x - 3, y + 16, z - 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x + 3, y + 16, z - 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x - 3, y + 16, z + 3, Tile::nusaLog_Id, 0);

			placeBlock(level, x + 4, y + 16, z + 2, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 16, z - 2, Tile::nustone_Id, 1);
			placeBlock(level, x + 4, y + 16, z - 2, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 16, z + 2, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 4, y + 16, z + 1, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 16, z - 1, Tile::nustone_Id, 1);
			placeBlock(level, x + 4, y + 16, z - 1, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 16, z + 1, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 2, y + 16, z + 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 2, y + 16, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x + 2, y + 16, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 2, y + 16, z + 4, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 1, y + 16, z + 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 1, y + 16, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x + 1, y + 16, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 1, y + 16, z + 4, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 4, y + 16, z, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 16, z, Tile::nustone_Id, 1);
			placeBlock(level, x, y + 16, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x, y + 16, z + 4, Tile::nustone_Id, 1);
		
		
		
			placeBlock(level, x + 5, y + 16, z + 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 16, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 16, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 16, z + 2, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 16, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 16, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 16, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 16, z + 1, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 2, y + 16, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 16, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 16, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 16, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 1, y + 16, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 16, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 1, y + 16, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 16, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 16, z, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 16, z, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 16, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 16, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 4, y + 16, z + 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 16, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x + 4, y + 16, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 16, z + 3, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 3, y + 16, z + 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 16, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x + 3, y + 16, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 16, z + 4, Tile::nustone_Id, 2);
		
		//Floor 6
			for (int vx = -3; vx <= 3; vx++)
			{
				for (int vz = -2; vz <= 2; vz++)
				{
					placeBlock(level, x + vx, y + 20, z + vz, Tile::netherPlanks_Id, 3);
				}
			}
			for (int wx = -2; wx <= 2; wx++)
			{
				for (int wz = -3; wz <= 3; wz++)
				{
					placeBlock(level, x + wx, y + 20, z + wz, Tile::netherPlanks_Id, 3);
				}
			}
			for (int xx = -1; xx <= 1; xx++)
			{
				for (int xz = -1; xz <= 1; xz++)
				{
					placeBlock(level, x + xx, y + 20, z + xz, 0, 0);
				}
			}
		
			placeBlock(level, x + 3, y + 20, z + 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x - 3, y + 20, z - 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x + 3, y + 20, z - 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x - 3, y + 20, z + 3, Tile::nusaLog_Id, 0);

			placeBlock(level, x + 4, y + 20, z + 2, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 20, z - 2, Tile::nustone_Id, 1);
			placeBlock(level, x + 4, y + 20, z - 2, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 20, z + 2, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 4, y + 20, z + 1, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 20, z - 1, Tile::nustone_Id, 1);
			placeBlock(level, x + 4, y + 20, z - 1, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 20, z + 1, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 2, y + 20, z + 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 2, y + 20, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x + 2, y + 20, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 2, y + 20, z + 4, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 1, y + 20, z + 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 1, y + 20, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x + 1, y + 20, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x - 1, y + 20, z + 4, Tile::nustone_Id, 1);
		
			placeBlock(level, x + 4, y + 20, z, Tile::nustone_Id, 1);
			placeBlock(level, x - 4, y + 20, z, Tile::nustone_Id, 1);
			placeBlock(level, x, y + 20, z - 4, Tile::nustone_Id, 1);
			placeBlock(level, x, y + 20, z + 4, Tile::nustone_Id, 1);
		
		
		
			placeBlock(level, x + 5, y + 20, z + 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 20, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 20, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 20, z + 2, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 20, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 20, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 20, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 20, z + 1, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 2, y + 20, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 20, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 20, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 20, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 1, y + 20, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 20, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 1, y + 20, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 20, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 20, z, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 20, z, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 20, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 20, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 4, y + 20, z + 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 20, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x + 4, y + 20, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 20, z + 3, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 3, y + 20, z + 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 20, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x + 3, y + 20, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 20, z + 4, Tile::nustone_Id, 2);
		
		//Floor 7
			for (int yx = -3; yx <= 3; yx++)
			{
				for (int yz = -2; yz <= 2; yz++)
				{
					placeBlock(level, x + yx, y + 24, z + yz, Tile::netherPlanks_Id, 3);
				}
			}
			for (int zx = -2; zx <= 2; zx++)
			{
				for (int zz = -3; zz <= 3; zz++)
				{
					placeBlock(level, x + zx, y + 24, z + zz, Tile::netherPlanks_Id, 3);
				}
			}
		
			placeBlock(level, x + 3, y + 24, z + 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x - 3, y + 24, z - 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x + 3, y + 24, z - 3, Tile::nusaLog_Id, 0);
			placeBlock(level, x - 3, y + 24, z + 3, Tile::nusaLog_Id, 0);

			placeBlock(level, x + 4, y + 24, z + 2, Tile::netherPlanks_Id, 3);
			placeBlock(level, x - 4, y + 24, z - 2, Tile::netherPlanks_Id, 3);
			placeBlock(level, x + 4, y + 24, z - 2, Tile::netherPlanks_Id, 3);
			placeBlock(level, x - 4, y + 24, z + 2, Tile::netherPlanks_Id, 3);
		
			placeBlock(level, x + 4, y + 24, z + 1, Tile::netherPlanks_Id, 3);
			placeBlock(level, x - 4, y + 24, z - 1, Tile::netherPlanks_Id, 3);
			placeBlock(level, x + 4, y + 24, z - 1, Tile::netherPlanks_Id, 3);
			placeBlock(level, x - 4, y + 24, z + 1, Tile::netherPlanks_Id, 3);
		
			placeBlock(level, x + 2, y + 24, z + 4, Tile::netherPlanks_Id, 3);
			placeBlock(level, x - 2, y + 24, z - 4, Tile::netherPlanks_Id, 3);
			placeBlock(level, x + 2, y + 24, z - 4, Tile::netherPlanks_Id, 3);
			placeBlock(level, x - 2, y + 24, z + 4, Tile::netherPlanks_Id, 3);
		
			placeBlock(level, x + 1, y + 24, z + 4, Tile::netherPlanks_Id, 3);
			placeBlock(level, x - 1, y + 24, z - 4, Tile::netherPlanks_Id, 3);
			placeBlock(level, x + 1, y + 24, z - 4, Tile::netherPlanks_Id, 3);
			placeBlock(level, x - 1, y + 24, z + 4, Tile::netherPlanks_Id, 3);
		
			placeBlock(level, x + 4, y + 24, z, Tile::netherPlanks_Id, 3);
			placeBlock(level, x - 4, y + 24, z, Tile::netherPlanks_Id, 3);
			placeBlock(level, x, y + 24, z - 4, Tile::netherPlanks_Id, 3);
			placeBlock(level, x, y + 24, z + 4, Tile::netherPlanks_Id, 3);
		
		
		
			placeBlock(level, x + 5, y + 24, z + 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 24, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 24, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 24, z + 2, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 24, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 24, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 24, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 24, z + 1, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 2, y + 24, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 24, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 24, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 24, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 1, y + 24, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 24, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 1, y + 24, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 24, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 24, z, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 24, z, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 24, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 24, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 4, y + 24, z + 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 24, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x + 4, y + 24, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 4, y + 24, z + 3, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 3, y + 24, z + 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 24, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x + 3, y + 24, z - 4, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 24, z + 4, Tile::nustone_Id, 2);

		//Roof
			for (int ex = -4; ex <= 4; ex++)
			{
				for (int ez = -4; ez <= 4; ez++)
				{
					placeBlock(level, x + ex, y + 28, z + ez, Tile::nustone_Id, 2);
				}
			}
		
			placeBlock(level, x + 5, y + 28, z + 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 28, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 28, z - 3, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 28, z + 3, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 28, z + 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 28, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 28, z - 2, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 28, z + 2, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 28, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 28, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 5, y + 28, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 28, z + 1, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 3, y + 28, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 28, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 3, y + 28, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 3, y + 28, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 2, y + 28, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 28, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 2, y + 28, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 2, y + 28, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 1, y + 28, z + 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 28, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x + 1, y + 28, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 28, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 5, y + 28, z, Tile::nustone_Id, 2);
			placeBlock(level, x - 5, y + 28, z, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 28, z - 5, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 28, z + 5, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 6, y + 28, z + 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 6, y + 28, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x + 6, y + 28, z - 1, Tile::nustone_Id, 2);
			placeBlock(level, x - 6, y + 28, z + 1, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 1, y + 28, z + 6, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 28, z - 6, Tile::nustone_Id, 2);
			placeBlock(level, x + 1, y + 28, z - 6, Tile::nustone_Id, 2);
			placeBlock(level, x - 1, y + 28, z + 6, Tile::nustone_Id, 2);
		
			placeBlock(level, x + 6, y + 28, z, Tile::nustone_Id, 2);
			placeBlock(level, x - 6, y + 28, z, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 28, z - 6, Tile::nustone_Id, 2);
			placeBlock(level, x, y + 28, z + 6, Tile::nustone_Id, 2);
		
		
		
		//Walls
		placeBlock(level, x + 3, y + 1, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 1, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 1, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 1, z + 3, Tile::nusaLog_Id, 0);

		placeBlock(level, x + 4, y + 1, z + 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 1, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 1, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 1, z + 2, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 1, z + 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 1, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 1, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 1, z + 1, Tile::nustone_Id, 1);

		placeBlock(level, x + 2, y + 1, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 1, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 2, y + 1, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 1, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 1, y + 1, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 1, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 1, y + 1, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 1, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 1, z, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 1, z, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 1, z + 4, Tile::nustone_Id, 1);
		
		
		
		placeBlock(level, x + 3, y + 2, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 2, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 2, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 2, z + 3, Tile::nusaLog_Id, 0);

		placeBlock(level, x + 4, y + 2, z + 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 2, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 2, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 2, z + 2, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 2, z + 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 2, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 2, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 2, z + 1, Tile::nustone_Id, 1);

		placeBlock(level, x + 2, y + 2, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 2, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 2, y + 2, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 2, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 1, y + 2, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 2, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 1, y + 2, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 2, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 2, z, Tile::stained_glass_Id, 3);
	    placeBlock(level, x - 4, y + 2, z, Tile::stained_glass_Id, 3);
	    placeBlock(level, x, y + 2, z + 4, Tile::stained_glass_Id, 3);
		
		
		
		placeBlock(level, x + 3, y + 3, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 3, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 3, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 3, z + 3, Tile::nusaLog_Id, 0);
		
		placeBlock(level, x + 4, y + 3, z + 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 3, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 3, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 3, z + 2, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 3, z + 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 3, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 3, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 3, z + 1, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 2, y + 3, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 3, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 2, y + 3, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 3, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 1, y + 3, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 3, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 1, y + 3, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 3, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 3, z, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 3, z, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 3, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 3, z + 4, Tile::nustone_Id, 1);
		
		
		
		placeBlock(level, x + 3, y + 5, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 5, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 5, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 5, z + 3, Tile::nusaLog_Id, 0);
		
		placeBlock(level, x + 4, y + 5, z + 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 5, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 5, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 5, z + 2, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 5, z + 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 5, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 5, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 5, z + 1, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 2, y + 5, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 5, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 2, y + 5, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 5, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 1, y + 5, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 5, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 1, y + 5, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 5, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 5, z, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 5, z, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 5, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 5, z + 4, Tile::nustone_Id, 1);
		
		
		
		placeBlock(level, x + 3, y + 6, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 6, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 6, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 6, z + 3, Tile::nusaLog_Id, 0);

		placeBlock(level, x + 4, y + 6, z + 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 6, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 6, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 6, z + 2, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 6, z + 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 6, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 6, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 6, z + 1, Tile::nustone_Id, 1);

		placeBlock(level, x + 2, y + 6, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 6, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 2, y + 6, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 6, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 1, y + 6, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 6, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 1, y + 6, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 6, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 6, z, Tile::stained_glass_Id, 3);
	    placeBlock(level, x - 4, y + 6, z, Tile::stained_glass_Id, 3);
	    placeBlock(level, x, y + 6, z + 4, Tile::stained_glass_Id, 3);
		placeBlock(level, x, y + 6, z - 4, Tile::stained_glass_Id, 3);
		
		
		
		placeBlock(level, x + 3, y + 7, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 7, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 7, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 7, z + 3, Tile::nusaLog_Id, 0);
		
		placeBlock(level, x + 4, y + 7, z + 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 7, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 7, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 7, z + 2, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 7, z + 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 7, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 7, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 7, z + 1, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 2, y + 7, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 7, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 2, y + 7, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 7, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 1, y + 7, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 7, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 1, y + 7, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 7, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 7, z, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 7, z, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 7, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 7, z + 4, Tile::nustone_Id, 1);
		
		
		
		placeBlock(level, x + 3, y + 9, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 9, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 9, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 9, z + 3, Tile::nusaLog_Id, 0);
		
		placeBlock(level, x + 4, y + 9, z + 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 9, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 9, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 9, z + 2, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 9, z + 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 9, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 9, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 9, z + 1, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 2, y + 9, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 9, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 2, y + 9, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 9, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 1, y + 9, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 9, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 1, y + 9, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 9, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 9, z, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 9, z, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 9, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 9, z + 4, Tile::nustone_Id, 1);
		
		
		
		placeBlock(level, x + 3, y + 10, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 10, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 10, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 10, z + 3, Tile::nusaLog_Id, 0);

		placeBlock(level, x + 4, y + 10, z + 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 10, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 10, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 10, z + 2, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 10, z + 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 10, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 10, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 10, z + 1, Tile::nustone_Id, 1);

		placeBlock(level, x + 2, y + 10, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 10, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 2, y + 10, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 10, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 1, y + 10, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 10, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 1, y + 10, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 10, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 10, z, Tile::stained_glass_Id, 3);
	    placeBlock(level, x - 4, y + 10, z, Tile::stained_glass_Id, 3);
	    placeBlock(level, x, y + 10, z + 4, Tile::stained_glass_Id, 3);
		placeBlock(level, x, y + 10, z - 4, Tile::stained_glass_Id, 3);
		
		
		
		placeBlock(level, x + 3, y + 11, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 11, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 11, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 11, z + 3, Tile::nusaLog_Id, 0);
		
		placeBlock(level, x + 4, y + 11, z + 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 11, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 11, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 11, z + 2, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 11, z + 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 11, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 11, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 11, z + 1, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 2, y + 11, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 11, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 2, y + 11, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 11, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 1, y + 11, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 11, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 1, y + 11, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 11, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 11, z, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 11, z, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 11, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 11, z + 4, Tile::nustone_Id, 1);
		
		
		
		placeBlock(level, x + 3, y + 13, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 13, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 13, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 13, z + 3, Tile::nusaLog_Id, 0);
		
		placeBlock(level, x + 4, y + 13, z + 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 13, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 13, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 13, z + 2, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 13, z + 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 13, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 13, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 13, z + 1, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 2, y + 13, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 13, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 2, y + 13, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 13, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 1, y + 13, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 13, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 1, y + 13, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 13, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 13, z, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 13, z, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 13, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 13, z + 4, Tile::nustone_Id, 1);
		
		
		
		placeBlock(level, x + 3, y + 14, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 14, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 14, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 14, z + 3, Tile::nusaLog_Id, 0);

		placeBlock(level, x + 4, y + 14, z + 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 14, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 14, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 14, z + 2, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 14, z + 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 14, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 14, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 14, z + 1, Tile::nustone_Id, 1);

		placeBlock(level, x + 2, y + 14, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 14, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 2, y + 14, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 14, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 1, y + 14, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 14, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 1, y + 14, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 14, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 14, z, Tile::stained_glass_Id, 3);
	    placeBlock(level, x - 4, y + 14, z, Tile::stained_glass_Id, 3);
	    placeBlock(level, x, y + 14, z + 4, Tile::stained_glass_Id, 3);
		placeBlock(level, x, y + 14, z - 4, Tile::stained_glass_Id, 3);
		
		
		
		placeBlock(level, x + 3, y + 15, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 15, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 15, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 15, z + 3, Tile::nusaLog_Id, 0);
		
		placeBlock(level, x + 4, y + 15, z + 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 15, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 15, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 15, z + 2, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 15, z + 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 15, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 15, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 15, z + 1, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 2, y + 15, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 15, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 2, y + 15, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 15, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 1, y + 15, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 15, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 1, y + 15, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 15, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 15, z, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 15, z, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 15, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 15, z + 4, Tile::nustone_Id, 1);
		
		
		
		placeBlock(level, x + 3, y + 17, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 17, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 17, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 17, z + 3, Tile::nusaLog_Id, 0);
		
		placeBlock(level, x + 4, y + 17, z + 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 17, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 17, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 17, z + 2, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 17, z + 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 17, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 17, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 17, z + 1, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 2, y + 17, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 17, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 2, y + 17, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 17, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 1, y + 17, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 17, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 1, y + 17, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 17, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 17, z, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 17, z, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 17, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 17, z + 4, Tile::nustone_Id, 1);
		
		
		
		placeBlock(level, x + 3, y + 18, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 18, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 18, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 18, z + 3, Tile::nusaLog_Id, 0);

		placeBlock(level, x + 4, y + 18, z + 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 18, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 18, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 18, z + 2, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 18, z + 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 18, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 18, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 18, z + 1, Tile::nustone_Id, 1);

		placeBlock(level, x + 2, y + 18, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 18, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 2, y + 18, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 18, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 1, y + 18, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 18, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 1, y + 18, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 18, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 18, z, Tile::stained_glass_Id, 3);
	    placeBlock(level, x - 4, y + 18, z, Tile::stained_glass_Id, 3);
	    placeBlock(level, x, y + 18, z + 4, Tile::stained_glass_Id, 3);
		placeBlock(level, x, y + 18, z - 4, Tile::stained_glass_Id, 3);
		
		
		
		placeBlock(level, x + 3, y + 19, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 19, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 19, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 19, z + 3, Tile::nusaLog_Id, 0);
		
		placeBlock(level, x + 4, y + 19, z + 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 19, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 19, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 19, z + 2, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 19, z + 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 19, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 19, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 19, z + 1, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 2, y + 19, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 19, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 2, y + 19, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 19, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 1, y + 19, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 19, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 1, y + 19, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 19, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 19, z, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 19, z, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 19, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 19, z + 4, Tile::nustone_Id, 1);
		
		
		
		placeBlock(level, x + 3, y + 21, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 21, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 21, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 21, z + 3, Tile::nusaLog_Id, 0);
		
		placeBlock(level, x + 4, y + 21, z + 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 21, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 21, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 21, z + 2, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 21, z + 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 21, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 21, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 21, z + 1, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 2, y + 21, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 21, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 2, y + 21, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 21, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 1, y + 21, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 21, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 1, y + 21, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 21, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 21, z, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 21, z, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 21, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 21, z + 4, Tile::nustone_Id, 1);
		
		
		
		placeBlock(level, x + 3, y + 22, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 22, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 22, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 22, z + 3, Tile::nusaLog_Id, 0);

		placeBlock(level, x + 4, y + 22, z + 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 22, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 22, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 22, z + 2, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 22, z + 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 22, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 22, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 22, z + 1, Tile::nustone_Id, 1);

		placeBlock(level, x + 2, y + 22, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 22, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 2, y + 22, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 22, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 1, y + 22, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 22, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 1, y + 22, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 22, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 22, z, Tile::stained_glass_Id, 3);
	    placeBlock(level, x - 4, y + 22, z, Tile::stained_glass_Id, 3);
	    placeBlock(level, x, y + 22, z + 4, Tile::stained_glass_Id, 3);
		placeBlock(level, x, y + 22, z - 4, Tile::stained_glass_Id, 3);
		
		
		
		placeBlock(level, x + 3, y + 23, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 23, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 23, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 23, z + 3, Tile::nusaLog_Id, 0);
		
		placeBlock(level, x + 4, y + 23, z + 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 23, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 23, z - 2, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 23, z + 2, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 23, z + 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 23, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x + 4, y + 23, z - 1, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 23, z + 1, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 2, y + 23, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 23, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 2, y + 23, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 2, y + 23, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 1, y + 23, z + 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 23, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x + 1, y + 23, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x - 1, y + 23, z + 4, Tile::nustone_Id, 1);
		
		placeBlock(level, x + 4, y + 23, z, Tile::nustone_Id, 1);
	    placeBlock(level, x - 4, y + 23, z, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 23, z - 4, Tile::nustone_Id, 1);
	    placeBlock(level, x, y + 23, z + 4, Tile::nustone_Id, 1);
		
		
		
		placeBlock(level, x + 3, y + 25, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 25, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 25, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 25, z + 3, Tile::nusaLog_Id, 0);
		
		placeBlock(level, x + 3, y + 26, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 26, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 26, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 26, z + 3, Tile::nusaLog_Id, 0);

		placeBlock(level, x + 3, y + 27, z + 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 27, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x + 3, y + 27, z - 3, Tile::nusaLog_Id, 0);
	    placeBlock(level, x - 3, y + 27, z + 3, Tile::nusaLog_Id, 0);
		
		
		
		//Ladder
			for (int posty = 0; posty <= 24; posty++)
			{
				if (level->getTile(x, y + posty, z + 2) == 0)
				{
					placeBlock(level, x, y + posty, z + 2, Tile::nusaLog_Id, 0);
				}
			}
			for (int laddery = 0; laddery <= 23; laddery++)
			{
				placeBlock(level, x, y + laddery + 1, z + 1, Tile::ladder_Id, 2);
			}

		//Chest/Core
			if (random->nextInt(2) == 0)
			{
				level->setTileAndData(x, y + 25, z, Tile::chest_Id, 0, Tile::UPDATE_CLIENTS);
				WeighedTreasureArray wrapperArray(nusaTowerTreasure, TREASURE_ITEMS_COUNT);
				WeighedTreasureArray treasure = WeighedTreasure::addToTreasure(wrapperArray, Item::enchantedBook->createForRandomTreasure(random));
				shared_ptr<ChestTileEntity> chest = dynamic_pointer_cast<ChestTileEntity>( level->getTileEntity(x, y + 25, z) );
				if (chest != NULL)
				{
					WeighedTreasure::addChestItems(random, treasure, chest, 7 + random->nextInt(3));
				}
			}
			else
			{
				placeBlock(level, x, y + 25, z, Tile::nustone_Id, 1);
				placeBlock(level, x, y + 26, z, Tile::nusaCore_Id, 0);
			}
		}
	}
    return true;
}

WeighedTreasure *NusaTowerFeature::nusaTowerTreasure[NusaTowerFeature::TREASURE_ITEMS_COUNT] = 
{
	new WeighedTreasure(Item::coal_Id, 0, 3, 7, 6),
	new WeighedTreasure(Item::ironIngot_Id, 0, 3, 7, 6),
	new WeighedTreasure(Item::nethanium_Id, 0, 3, 7, 6),
	new WeighedTreasure(Tile::enchantedGravitite_Id, 0, 3, 7, 6),
	new WeighedTreasure(Item::endorium_Id, 0, 3, 7, 6),
	new WeighedTreasure(Item::endothaniumIngot_Id, 0, 1, 4, 3),
	new WeighedTreasure(Item::nethaniumBread_Id, 0, 3, 7, 5),
	new WeighedTreasure(Item::goldBread_Id, 0, 4, 9, 6),
	new WeighedTreasure(Item::apple_gold_Id, 1, 1, 1, 2),
	new WeighedTreasure(Item::endoriumHelmet_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::endoriumChestplate_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::endoriumLeggings_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::endoriumBoots_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::nusaShard_Id, 0, 1, 5, 5),
	new WeighedTreasure(Item::lifeShard_Id, 0, 1, 1, 3),
	new WeighedTreasure(Item::vampireBlade_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::valkyrieLance_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::valkyrieAxe_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::agilityCape_Id, 0, 1, 1, 2),
	new WeighedTreasure(Item::ironBubble_Id, 0, 1, 1, 2),
};
