#include "stdafx.h"
#include "BlockReplacements.h"
#include "net.minecraft.world.level.tile.h"

byteArray BlockReplacements::replacements = byteArray(512);

void BlockReplacements::staticCtor()
{
    for (int i = 0; i < 512; i++)
	{
        byte b = static_cast<byte>(i);
        if (b != 0 && Tile::tiles[b & 0x1ff] == nullptr)
		{
            b = 0;
        }
        BlockReplacements::replacements[i] = b;
    }
}

void BlockReplacements::replace(byteArray blocks)
{
    for (unsigned int i = 0; i < blocks.length; i++)
	{
        blocks[i] = replacements[blocks[i] & 0x1ff];
    }
}
