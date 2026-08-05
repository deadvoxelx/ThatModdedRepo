#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.material.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.dimension.h"
#include "net.minecraft.world.level.levelgen.feature.h"
#include "net.minecraft.world.item.h"
#include "GoldIsland.h"

static bool inIslandShape(int dx, int dy, int dz, int l)
{
	int k3 = Mth::floor(dx * (1.0 + (dy / l) * 10.0) / 0.8f);
	int i4 = dy;
	if (dy > l * 0.625)
	{
		i4 = Mth::floor(i4 * 1.375);
		i4 -= Mth::floor(l * 0.25);
	}
	else if (dy < l * -0.625)
	{
		i4 = Mth::floor(i4 * 1.350000023841858);
		i4 += Mth::floor(l * 0.25);
	}
	int k4 = Mth::floor(dz * (1.0 + (dy / l) * 10.0) / 0.8f);
	return Mth::sqrt(k3 * k3 + i4 * i4 + k4 * k4 + 0.0) <= l;
}

static bool inBlobShape(int dx, int dy, int dz, int l)
{
	int k2 = dx;
	int i3 = dy;
	if (dy > l * 0.625)
	{
		i3 = Mth::floor(i3 * 1.375);
		i3 -= Mth::floor(l * 0.25);
	}
	else if (dy < l * -0.625)
	{
		i3 = Mth::floor(i3 * 1.350000023841858);
		i3 += Mth::floor(l * 0.25);
	}
	int k3 = dz;
	return Mth::sqrt(k2 * k2 + i3 * i3 + k3 * k3 + 0.0) <= l;
}

GoldIsland::GoldIsland(int blockId) : Feature(blockId)
{
    whiteFlowerFeature = new FlowerFeature(Tile::flower_Id, 1);
	purpleFlowerFeature = new FlowerFeature(Tile::flower_Id, 2);
    grassFeature = new TallGrassFeature(Tile::tallgrass_Id, 1);
}

bool GoldIsland::place(Level *level, Random *random, int x, int y, int z)
{
    int r = 16 * 3;

	int xc = Mth::floor(x * 1.0);
	int yc = Mth::floor(y * 1.0);
	int zc = Mth::floor(z * 1.0);

	int XZSIZE = level->dimension->getXZSize() * 16;
	int XZOFFSET = (XZSIZE / 2) - 32;

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
	}
    return generate(level, random, x, y, z, 24);
}
  
bool GoldIsland::generate(Level *level, Random *random, int x, int y, int z, int l)
{
    if (y - l <= 0) y = l + 1; 
    if (y + l >= 116) y = 116 - l - 1; 
    for (int i2 = -l; i2 <= l; i2++)
    {
      for (int l2 = l; l2 >= -l; l2--)
      {
        for (int i3 = -l; i3 <= l; i3++)
        {
          if (!inIslandShape(i2, l2, i3, l)) continue;
          if (level->getTile(i2 + x, l2 + y, i3 + z) == Tile::chest_Id) continue;
          bool surface = (l2 > Mth::floor(l / 5.0)) && !inIslandShape(i2, l2 + 1, i3, l);
          bool aboveIsSurface = (l2 + 1 > Mth::floor(l / 5.0)) && inIslandShape(i2, l2 + 1, i3, l) && !inIslandShape(i2, l2 + 2, i3, l);
          if (surface)
          {
              placeBlock(level, i2 + x, l2 + y, i3 + z, Tile::aetherGrass_Id, 0);
              placeBlock(level, i2 + x, l2 + y - 1, i3 + z, Tile::aetherDirt_Id, 0);
              placeBlock(level, i2 + x, l2 + y - 1 + random->nextInt(2), i3 + z, Tile::aetherDirt_Id, 0);
              if (l2 >= l / 2)
              {
                int j5 = random->nextInt(48);
                if (j5 < 2)
				{
                  (new GoldenOakLargeFeature(false))->place(level, random, i2 + x, l2 + y + 1, i3 + z);
                }
				else if (j5 == 3)
				{
                  if (random->nextInt(2) == 0)
                    grassFeature->place(level, random, i2 + x + random->nextInt(3) - random->nextInt(3), l2 + y, i3 + z + random->nextInt(3) - random->nextInt(3)); 
                }
				else if (j5 == 4)
				{
                  if (random->nextInt(2) == 0)
				  {
                    purpleFlowerFeature->place(level, random, i2 + x + random->nextInt(3) - random->nextInt(3), l2 + y + 1, i3 + z + random->nextInt(3) - random->nextInt(3));
                  }
				  else
				  {
                    whiteFlowerFeature->place(level, random, i2 + x + random->nextInt(3) - random->nextInt(3), l2 + y + 1, i3 + z + random->nextInt(3) - random->nextInt(3));
                  } 
                } 
              } 
            }
          else if (aboveIsSurface)
          {
          }
          else
          {
              placeBlock(level, i2 + x, l2 + y, i3 + z, Tile::holystone_Id, 0);
          }  
        } 
      } 
    } 
    int j2 = 8 + random->nextInt(5);
    float f1 = 0.01745329f;
    for (int j3 = 0; j3 < j2; j3++)
    {
      float f2 = random->nextFloat() * 360.0f;
      float f3 = (random->nextFloat() * 0.125f + 0.7f) * l;
      int l4 = x + Mth::floor(Mth::cos((f1 * f2)) * f3);
      int k5 = y - Mth::floor(l * random->nextFloat() * 0.3f);
      int i6 = z + Mth::floor(-Mth::sin((f1 * f2)) * f3);
      generateBlob(level, random, l4, k5, i6, Mth::floor(l / 3.0));
    }
    bool flag = false;
    flag = (new GoldDungeon(Tile::holystone_Id))->generate(level, random, x, y, z, 24); 
    return true;
}

bool GoldIsland::generateBlob(Level *level, Random *random, int x, int y, int z, int l)
{
    if (y - l <= 0) y = l + 1; 
    if (y + l >= 127) y = 127 - l - 1; 
    for (int i1 = -l; i1 <= l; i1++)
    {
      for (int k1 = l; k1 >= -l; k1--)
      {
        for (int i2 = -l; i2 <= l; i2++)
        {
          if (!inBlobShape(i1, k1, i2, l)) continue;
          if (level->getTile(i1 + x, k1 + y, i2 + z) == Tile::chest_Id) continue;
          bool surface = (k1 > Mth::floor(l / 5.0)) && !inBlobShape(i1, k1 + 1, i2, l);
          bool aboveIsSurface = (k1 + 1 > Mth::floor(l / 5.0)) && inBlobShape(i1, k1 + 1, i2, l) && !inBlobShape(i1, k1 + 2, i2, l);
          if (surface)
          {
              placeBlock(level, i1 + x, k1 + y, i2 + z, Tile::aetherGrass_Id, 0);
              placeBlock(level, i1 + x, k1 + y - 1, i2 + z, Tile::aetherDirt_Id, 0);
              placeBlock(level, i1 + x, k1 + y - 1 + random->nextInt(2), i2 + z, Tile::aetherDirt_Id, 0);
              if (k1 >= l / 2)
              {
                int l3 = random->nextInt(64);
                if (l3 == 0)
                {
                  (new GoldenOakLargeFeature(false))->place(level, random, i1 + x, k1 + y + 1, i2 + z);
                }
                else if (l3 == 5)
                {
                  if (random->nextInt(3) == 0)
                    grassFeature->place(level, random, i1 + x + random->nextInt(3) - random->nextInt(3), k1 + y, i2 + z + random->nextInt(3) - random->nextInt(3)); 
                }
              }
            }
          else if (aboveIsSurface)
          {
          }
          else
          {
              placeBlock(level, i1 + x, k1 + y, i2 + z, Tile::holystone_Id, 0);
          }
        }
      }
    } 
    return true;
}
