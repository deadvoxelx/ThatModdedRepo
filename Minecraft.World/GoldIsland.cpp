#include "net.minecraft.world.level.h"
#include "net.minecraft.world.level.material.h"
#include "net.minecraft.world.level.tile.h"
#include "net.minecraft.world.level.levelgen.feature.h"
#include "net.minecraft.world.item.h"
#include "GoldIsland.h"

GoldIsland::GoldIsland(int blockId) : Feature(blockId)
{
    whiteFlowerFeature = new FlowerFeature(Tile::flower_Id, 1);
	purpleFlowerFeature = new FlowerFeature(Tile::flower_Id, 2);
    grassFeature = new TallGrassFeature(Tile::tallgrass_Id, 1);
}

bool GoldIsland::place(Level *level, Random *random, int x, int y, int z)
{
    return generate(level, random, x, y, z, 24);
}
  
bool GoldIsland::generate(Level *level, Random *random, int x, int y, int z, int l)
{
    if (y - l <= 0) y = l + 1; 
    if (y + l >= 116) y = 116 - l - 1; 
    int i1 = 0;
    int j1 = Mth::floor(l * 0.875);
    for (int k1 = -j1; k1 <= j1; k1++)
    {
      for (int l1 = l; l1 >= -j1; l1--)
      {
        for (int k2 = -j1; k2 <= j1; k2++)
        {
          if (!level->isEmptyTile(k1 + x, l1 + y, k2 + z) && ++i1 > l / 2)
            return false; 
        } 
      } 
    } 
    float f = 0.8f;
    for (int i2 = -l; i2 <= l; i2++)
    {
      for (int l2 = l; l2 >= -l; l2--)
      {
        for (int i3 = -l; i3 <= l; i3++)
        {
          int k3 = Mth::floor(i2 * (1.0 + l2 / l * 10.0) / f);
          int i4 = l2;
          if (l2 > l * 0.625)
          {
            i4 = Mth::floor(i4 * 1.375);
            i4 -= Mth::floor(l * 0.25);
          }
          else if (l2 < l * -0.625)
          {
            i4 = Mth::floor(i4 * 1.350000023841858);
            i4 += Mth::floor(l * 0.25);
          }
          int k4 = Mth::floor(i3 * (1.0 + l2 / l * 10.0) / f);
          if (Mth::sqrt(k3 * k3 + i4 * i4 + k4 * k4 + 0.0) <= l)
            if (level->isEmptyTile(i2 + x, l2 + y + 1, i3 + z) && l2 > Mth::floor(l / 5.0))
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
			else if
			(level->isEmptyTile(i2 + x, l2 + y, i3 + z))
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
    float f = 1.0f;
    for (int i1 = -l; i1 <= l; i1++)
    {
      for (int k1 = l; k1 >= -l; k1--)
      {
        for (int i2 = -l; i2 <= l; i2++)
        {
          int k2 = Mth::floor(i1 / f);
          int i3 = k1;
          if (k1 > l * 0.625)
          {
            i3 = Mth::floor(i3 * 1.375);
            i3 -= Mth::floor(l * 0.25);
          }
          else if (k1 < l * -0.625)
          {
            i3 = Mth::floor(i3 * 1.350000023841858);
            i3 += Mth::floor(l * 0.25);
          } 
          int k3 = Mth::floor(i2 / f);
          if (Mth::sqrt((k2 * k2 + i3 * i3 + k3 * k3 + 0.0)) <= l)
            if (level->isEmptyTile(i1 + x, k1 + y + 1, i2 + z) && k1 > Mth::floor(l / 5.0))
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
            else if (level->isEmptyTile(i1 + x, k1 + y, i2 + z))
            {
              placeBlock(level, i1 + x, k1 + y, i2 + z, Tile::holystone_Id, 0);
            }
        }
      }
    } 
    return true;
}