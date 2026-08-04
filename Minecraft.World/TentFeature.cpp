#include "TentFeature.h"
#include "net.minecraft.world.entity.h"
#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.levelgen.feature.h"
#include "net.minecraft.world.level.material.h"
#include "net.minecraft.world.level.biome.h"
#include "net.minecraft.world.level.dimension.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.item.h"

TentFeature::TentFeature(int blockId) : Feature(blockId)
{
}

bool TentFeature::place(Level *level, Random *random, int x, int y, int z)
{
  while (y > 0 && !level->getMaterial(x, y - 1, z)->blocksMotion()) y--;

  int r = 16 * 3; //This extra stuff up here prevents it from generating too close to the world border

  int xc = Mth::floor(x * 1.0);
  int yc = Mth::floor(y * 1.0);
  int zc = Mth::floor(z * 1.0);

  int XZSIZE = level->dimension->getXZSize() * 16;
  int XZOFFSET = (XZSIZE / 2) - 16;

  if( (xc - r) < -XZOFFSET )
  {
	return false;
  }
  else if ( (xc + r) >= XZOFFSET )
  {
	return false;
  }
  if( (zc - r) < -XZOFFSET )
  {
	return false;
  }
  else if ( (zc + r) >= XZOFFSET )
  {
	return false;
  } //End extra stuff

  if (random->nextInt(7) == 0)
  {
	for (int groundx = -3; groundx <= 3; groundx++)
	{
		for (int groundz = -3; groundz <= 3; groundz++)
		{	//check 7x7 space for tent
			int belowTile = level->getTile(x + groundx, y - 1, z + groundz);
    		Material *thisTile = level->getMaterial(x + groundx, y, z + groundz);

			if (belowTile == Tile::grass_Id && !thisTile->blocksMotion())
			{	//Place on grass only, avoid placing in solid spaces
				//Clear space up to roof height and create solid ground
				for (int dx = -3; dx <= 3; dx++)
				{
					for (int dy = 0; dy <= 3; dy++)
					{
						for (int dz = -3; dz <= 3; dz++)
						{
							placeBlock(level, x + dx, y + dy, z + dz, 0, 0);
							placeBlock(level, x + dx, y - 1, z + dz, Tile::grass_Id, 0);
							placeBlock(level, x + dx, y - 2, z + dz, Tile::dirt_Id, 0);
							placeBlock(level, x + dx, y - 3, z + dz, Tile::dirt_Id, 0);
						}
					}
				}

				//Biome-specific tree-placing
				if (level->getBiome(x, z)->hasSnow())
				{	//Taigas/Snowy Plains/etc
					placeBlock(level, x - 3 + random->nextInt(6), y - 1, z - 3 + random->nextInt(6), Tile::dirt_Id, 0);
					(new SpruceFeature(false))->place(level, random, x - 3 + random->nextInt(6), y, z - 3 + random->nextInt(6));
				}
				else if (level->getBiome(x, z) == Biome::birchForest)
				{
					placeBlock(level, x - 3 + random->nextInt(6), y - 1, z - 3 + random->nextInt(6), Tile::dirt_Id, 0);
					(new BirchFeature(false))->place(level, random, x - 3 + random->nextInt(6), y, z - 3 + random->nextInt(6));
				}
				else if (level->getBiome(x, z) == Biome::jungle || level->getBiome(x, z) == Biome::jungleHills)
				{
					placeBlock(level, x - 3 + random->nextInt(6), y - 1, z - 3 + random->nextInt(6), Tile::dirt_Id, 0);
					(new TreeFeature(false, 4 + random->nextInt(7), TreeTile::JUNGLE_TRUNK, LeafTile::JUNGLE_LEAF, true))->place(level, random, x - 3 + random->nextInt(6), y, z - 3 + random->nextInt(6));
				}
				else
				{	//Default oak tree when any non-specified biome
					placeBlock(level, x - 3 + random->nextInt(6), y - 1, z - 3 + random->nextInt(6), Tile::dirt_Id, 0);
					(new TreeFeature(false))->place(level, random, x - 3 + random->nextInt(6), y, z - 3 + random->nextInt(6));
				}
				//This allows for many many tree possibilities, far better than having a pre-placed tree in a singular spot

				//Tent
				for (int post = 0; post <= 3; post++)
				{
					placeBlock(level, x + 3, y + post, z, Tile::fence_Id, 0);
					placeBlock(level, x - 3, y + post, z, Tile::fence_Id, 0);
				}	//That should fix the fence posts
			
				for (int tent = -3; tent <= 3; tent++)
				{	//Random block placement, for essentially thousands of "variants" as Mojang would consider it
					placeBlock(level, x + tent, y, z + 3, Tile::wool_Id, 0);
					if (random->nextInt(5) == 0) placeBlock(level, x + tent, y, z + 3, 0, 0);
					if (random->nextInt(7) == 0) placeBlock(level, x + tent, y, z + 3, Tile::web_Id, 0);

					placeBlock(level, x + tent, y, z - 3, Tile::wool_Id, 0);
					if (random->nextInt(5) == 0) placeBlock(level, x + tent, y, z - 3, 0, 0);
					if (random->nextInt(7) == 0) placeBlock(level, x + tent, y, z - 3, Tile::web_Id, 0);

					placeBlock(level, x + tent, y + 1, z + 2, Tile::wool_Id, 0);
					if (random->nextInt(5) == 0) placeBlock(level, x + tent, y + 1, z + 2, 0, 0);
					if (random->nextInt(7) == 0) placeBlock(level, x + tent, y + 1, z + 2, Tile::web_Id, 0);

					placeBlock(level, x + tent, y + 1, z - 2, Tile::wool_Id, 0);
					if (random->nextInt(5) == 0) placeBlock(level, x + tent, y + 1, z - 2, 0, 0);
					if (random->nextInt(7) == 0) placeBlock(level, x + tent, y + 1, z - 2, Tile::web_Id, 0);

					placeBlock(level, x + tent, y + 2, z + 1, Tile::wool_Id, 0);
					if (random->nextInt(5) == 0) placeBlock(level, x + tent, y + 2, z + 1, 0, 0);
					if (random->nextInt(7) == 0) placeBlock(level, x + tent, y + 2, z + 1, Tile::web_Id, 0);

					placeBlock(level, x + tent, y + 2, z - 1, Tile::wool_Id, 0);
					if (random->nextInt(5) == 0) placeBlock(level, x + tent, y + 2, z - 1, 0, 0);
					if (random->nextInt(7) == 0) placeBlock(level, x + tent, y + 2, z - 1, Tile::web_Id, 0);

					placeBlock(level, x + tent, y + 3, z, Tile::wool_Id, 0);
					if (random->nextInt(5) == 0) placeBlock(level, x + tent, y + 3, z, 0, 0);
					if (random->nextInt(7) == 0) placeBlock(level, x + tent, y + 3, z, Tile::web_Id, 0);

					(new TallGrassFeature(Tile::tallgrass_Id, 1))->place(level, random, groundx + x, y + random->nextInt(3), groundz + z);
				}
			}
			return true;
		}
	}
  }
  return false;
}