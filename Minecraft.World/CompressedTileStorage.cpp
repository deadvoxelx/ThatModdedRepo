#include "stdafx.h"
#include "CompressedTileStorage.h"

#ifdef __PSVITA__
#define PSVITA_PRECOMPUTED_TABLE
#endif

#ifdef __PS3__
#include "..\SPU_Tasks\CompressedTileStorage_compress\CompressedTileStorage_compress.h"
#include "C4JSpursJob.h"
static const int sc_maxCompressTiles = 64;
static CompressedTileStorage_compress_dataIn g_compressTileDataIn[sc_maxCompressTiles] __attribute__((__aligned__(16)));
static int g_currentCompressTiles = 0;
//#define DISABLE_SPU_CODE
#endif //__PS3__


int CompressedTileStorage::deleteQueueIndex;
XLockFreeStack <unsigned char> CompressedTileStorage::deleteQueue[3];

CRITICAL_SECTION CompressedTileStorage::cs_write;

#ifdef PSVITA_PRECOMPUTED_TABLE
static int *CompressedTile_StorageIndexTable = nullptr;

void CompressedTileStorage_InitTable()
{
	if( CompressedTile_StorageIndexTable == nullptr )
	{
		CompressedTile_StorageIndexTable = (int*) malloc(sizeof(int) * 64);
		for(int j = 0;j < 64;j += 1 )
		{
			int index = ( ( j & 0x30 ) << 7) | ( ( j & 0x0c ) << 5 ) |  ( j & 0x03 );
			CompressedTile_StorageIndexTable[j] = index;
		}
	}
}
#endif

CompressedTileStorage::CompressedTileStorage()
{
	indicesAndData = nullptr;
	allocatedSize = 0;

#ifdef PSVITA_PRECOMPUTED_TABLE
	CompressedTileStorage_InitTable();
#endif
}

CompressedTileStorage::CompressedTileStorage(CompressedTileStorage *copyFrom)
{
	EnterCriticalSection(&cs_write);
	allocatedSize = copyFrom->allocatedSize;
	if(allocatedSize > 0)
	{
		indicesAndData = static_cast<unsigned char *>(XPhysicalAlloc(allocatedSize, MAXULONG_PTR, 4096, PAGE_READWRITE));//(unsigned char *)malloc(allocatedSize);
		XMemCpy(indicesAndData, copyFrom->indicesAndData, allocatedSize);
	}
	else
	{
		indicesAndData = nullptr;
	}
	LeaveCriticalSection(&cs_write);

#ifdef PSVITA_PRECOMPUTED_TABLE
	CompressedTileStorage_InitTable();
#endif
}

CompressedTileStorage::CompressedTileStorage(byteArray initFrom, unsigned int initOffset)
{
	indicesAndData = nullptr;
	allocatedSize = 0;

	indicesAndData = static_cast<unsigned char *>(XPhysicalAlloc(65536 + 4096, MAXULONG_PTR, 4096, PAGE_READWRITE));

	unsigned short *indices = (unsigned short *)indicesAndData;
	unsigned short *data = (unsigned short *)(indicesAndData + 1024);

	int offset = 0;
	for( int i = 0; i < 512; i++ )
	{
		indices[i] = INDEX_TYPE_0_OR_8_BIT | offset;

		if( initFrom.data )
		{
			for( int j = 0; j < 64; j++ )
			{
				*data++ = initFrom[getIndex(i,j) + initOffset];
			}
		}
		else
		{
			for( int j = 0; j < 64; j++ )
			{
				*data++ = 0;
			}
		}

		offset += 128;
	}

	allocatedSize = 65536 + 1024;

#ifdef PSVITA_PRECOMPUTED_TABLE
	CompressedTileStorage_InitTable();
#endif
}

bool CompressedTileStorage::isCompressed()
{
	return allocatedSize != 65536 + 1024;
}

CompressedTileStorage::CompressedTileStorage(bool isEmpty)
{
	indicesAndData = nullptr;
	allocatedSize = 0;

#ifdef __PS3__
	indicesAndData = (unsigned char *)XPhysicalAlloc(1024, MAXULONG_PTR, 4096, PAGE_READWRITE);
#else
	indicesAndData = static_cast<unsigned char *>(XPhysicalAlloc(4096, MAXULONG_PTR, 4096, PAGE_READWRITE));
#endif //__PS3__
	unsigned short *indices = (unsigned short *)indicesAndData;

	for( int i = 0; i < 512; i++ )
	{
		indices[i] = INDEX_TYPE_0_OR_8_BIT | INDEX_TYPE_0_BIT_FLAG;
	}

	allocatedSize = 1024;

#ifdef PSVITA_PRECOMPUTED_TABLE
	CompressedTileStorage_InitTable();
#endif
}

bool CompressedTileStorage::isRenderChunkEmpty(int y)
{
	int block;
	unsigned char *localIndicesAndData = indicesAndData;
	if(!localIndicesAndData) return true;
	unsigned short *blockIndices = (unsigned short *)localIndicesAndData;

	for( int x = 0; x < 16; x += 4 )
		for( int z = 0; z < 16; z += 4 )
	{
		getBlock(&block, x, y, z);
		uint64_t *comp = (uint64_t *)&blockIndices[block];
		if( ( *comp ) != 0x0007000700070007L ) return false;
	}
	return true;
}

bool CompressedTileStorage::isSameAs(CompressedTileStorage *other)
{
	EnterCriticalSection(&cs_write);
	if( allocatedSize != other->allocatedSize )
	{
		LeaveCriticalSection(&cs_write);
		return false;
	}

	int quickCount = allocatedSize / 64;
	int64_t *pOld = (int64_t *)indicesAndData;
	int64_t *pNew = (int64_t *)other->indicesAndData;
	for( int i = 0; i < quickCount; i++ )
	{
		int64_t d0 = pOld[0] ^ pNew[0];
		int64_t d1 = pOld[1] ^ pNew[1];
		int64_t d2 = pOld[2] ^ pNew[2];
		int64_t d3 = pOld[3] ^ pNew[3];
		int64_t d4 = pOld[4] ^ pNew[4];
		int64_t d5 = pOld[5] ^ pNew[5];
		int64_t d6 = pOld[6] ^ pNew[6];
		int64_t d7 = pOld[7] ^ pNew[7];
		d0 |= d1;
		d2 |= d3;
		d4 |= d5;
		d6 |= d7;
		d0 |= d2;
		d4 |= d6;
		if( d0 | d4 )
		{
			LeaveCriticalSection(&cs_write);
			return false;
		}
		pOld += 8;
		pNew += 8;
	}

	unsigned char *pucOld = (unsigned char *)pOld;
	unsigned char *pucNew = (unsigned char *)pNew;
	for( int i = 0; i < allocatedSize - (quickCount * 64); i++ )
	{
		if( *pucOld++ != *pucNew++ )
		{
			LeaveCriticalSection(&cs_write);
			return false;
		}
	}

	LeaveCriticalSection(&cs_write);
	return true;
}

CompressedTileStorage::~CompressedTileStorage()
{
#if 1
	if(indicesAndData) XPhysicalFree(indicesAndData);
#else
	if( (unsigned int)indicesAndData >= MM_PHYSICAL_4KB_BASE )
	{
		if(indicesAndData) XPhysicalFree(indicesAndData);
	}
	else
	{
		if(indicesAndData) free(indicesAndData);
	}
#endif
}

inline int CompressedTileStorage::getIndex(int block, int tile)
{
	int index = ( ( block & 0x180) << 6 ) | ( ( block & 0x060 ) << 4 ) | ( ( block & 0x01f ) << 2 );
	index |= ( ( tile & 0x30 ) << 7) | ( ( tile & 0x0c ) << 5 ) |  ( tile & 0x03 );

	return index;
}

inline void CompressedTileStorage::getBlockAndTile(int *block, int *tile, int x, int y, int z)
{
	*block = ( ( x  & 0x0c ) << 5 ) | ( ( z & 0x0c ) << 3 ) | ( y >> 2 );
	*tile = ( ( x & 0x03 ) << 4 ) | ( ( z & 0x03 ) << 2 ) | ( y & 0x03 );
}

inline void CompressedTileStorage::getBlock(int *block, int x, int y, int z)
{
	*block = ( ( x  & 0x0c ) << 5 ) | ( ( z & 0x0c ) << 3 ) | ( y >> 2 );
}

void CompressedTileStorage::setData(byteArray dataIn, unsigned int inOffset)
{
	unsigned short _blockIndices[512];

	EnterCriticalSection(&cs_write);
	unsigned char *data = dataIn.data + inOffset;

	if( allocatedSize == ( 65536 + 1024 ) )
	{
		unsigned short *dataOut = (unsigned short *)(indicesAndData + 1024);

		for( int i = 0; i < 512; i++ )
		{
			for( int j = 0; j < 64; j++ )
			{
				*dataOut++ = data[getIndex(i,j)];
			}
		}
		LeaveCriticalSection(&cs_write);
		return;
	}

	int offsets[512];
	int memToAlloc = 0;

	for( int i = 0; i < 512; i++ )
	{
		offsets[i] = memToAlloc;

#ifdef __PSVITA__
		unsigned int usedFlags[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		__int32 i32_1 = 1;
		for( int j = 0; j < 64; j++ )
		{
			int tile = data[getIndex(i,j)];
			if( tile < 512 )
			{
				usedFlags[tile & 15] |= ( i32_1 << ( tile >> 4 ) );
			}
		}
		int count = 0;
		for( int tile = 0; tile < 512; tile++ )
		{
			if( usedFlags[tile & 15] & ( i32_1 << ( tile >> 4 ) ) )
			{
				count++;
			}
		}
#else
		uint64_t usedFlags[8] = {0,0,0,0,0,0,0,0};
		int64_t i64_1 = 1;
		for( int j = 0; j < 64; j++ )
		{
			int tile = data[getIndex(i,j)];

			usedFlags[tile & 7] |= ( i64_1 << ( tile >> 3 ) );
		}
		int count = 0;
		for( int tile = 0; tile < 512; tile++ )
		{
			if( usedFlags[tile & 7] & ( i64_1 << ( tile >> 3 ) ) )
			{
				count++;
			}
		}
#endif
		if( count == 1 )
		{
			_blockIndices[i] = INDEX_TYPE_0_OR_8_BIT | INDEX_TYPE_0_BIT_FLAG;
		}
		else if( count == 2 )
		{
			_blockIndices[i] = INDEX_TYPE_1_BIT;
			memToAlloc += 12;
		}
		else if ( count <= 4 )
		{
			_blockIndices[i] = INDEX_TYPE_2_BIT;
			memToAlloc += 24;
		}
		else if ( count <= 16 )
		{
			_blockIndices[i] = INDEX_TYPE_4_BIT;
			memToAlloc += 64;
		}
		else
		{
			_blockIndices[i] = INDEX_TYPE_0_OR_8_BIT;
			memToAlloc = ( memToAlloc + 7 ) & 0xfff8;
			memToAlloc += 128;
		}
	}


	memToAlloc += 1024;
	unsigned char *newIndicesAndData = static_cast<unsigned char *>(XPhysicalAlloc(memToAlloc, MAXULONG_PTR, 4096, PAGE_READWRITE));
	unsigned char *pucData = newIndicesAndData + 1024;
	unsigned short usDataOffset = 0;
	unsigned short *newIndices = (unsigned short *) newIndicesAndData;

	for( int i = 0; i < 512; i++ )
	{
		unsigned short indexTypeNew = _blockIndices[i] & INDEX_TYPE_MASK;
		newIndices[i] = indexTypeNew;

		if( indexTypeNew == INDEX_TYPE_0_OR_8_BIT )
		{
			if( _blockIndices[i] & INDEX_TYPE_0_BIT_FLAG )
			{
				newIndices[i] = INDEX_TYPE_0_OR_8_BIT | INDEX_TYPE_0_BIT_FLAG | (static_cast<unsigned short>(data[getIndex(i, 0)]) << INDEX_TILE_SHIFT);
			}
			else
			{
				usDataOffset = (usDataOffset + 7 ) & 0xfff8;
				for( int j = 0; j < 64; j++ )
				{
					((unsigned short *)(pucData + usDataOffset))[j] = data[getIndex(i,j)];
				}
				newIndices[i] |= ( usDataOffset & INDEX_OFFSET_MASK) << INDEX_OFFSET_SHIFT;
				usDataOffset += 128;
			}
		}
		else
		{
			unsigned char ucMappings[512] = {0};
			for( int j = 0; j < 512; j++ )
			{
				ucMappings[j] = 255;
			}

			unsigned char *repacked = nullptr;

			int bitspertile = 1 << indexTypeNew;		// will be 1, 2 or 4 (from index values of 0, 1, 2)
			int tiletypecount = 1 << bitspertile;		// will be 2, 4 or 16 (from index values of 0, 1, 2)
			//int tiletypemask = tiletypecount - 1;		// will be 1, 3 or 15 (from index values of 0, 1, 2)
			int tiledatasize = 8 << indexTypeNew;		// will be 8, 16 or 32 (from index values of 0, 1, 2)
			int indexshift = 3 - indexTypeNew;			// will be 3, 2 or 1 (from index values of 0, 1, 2)
			int indexmask_bits = 7 >> indexTypeNew;		// will be 7, 3 or 1 (from index values of 0, 1, 2)
			int indexmask_bytes = 62 >> indexshift;	    // will be 7, 15 or 31 (from index values of 0, 1, 2)

			unsigned short *tile_types = (unsigned short *)(pucData + usDataOffset);
			repacked = (unsigned char *)(tile_types + tiletypecount);
			XMemSet(tile_types, 255, tiletypecount * 2);
			XMemSet(repacked, 0,tiledatasize);
			newIndices[i] |= ( usDataOffset & INDEX_OFFSET_MASK) << INDEX_OFFSET_SHIFT;
			usDataOffset += ( tiletypecount * 2 ) + tiledatasize;
			int count = 0;
			for( int j = 0; j < 64; j++ )
			{
				int tile = data[getIndex(i,j)];
				if( ucMappings[tile] == 255 )
				{
					ucMappings[tile] = count;
					tile_types[count++] = tile;
				}
				int idx = (j >> indexshift) & indexmask_bytes;
				int bit = ( j & indexmask_bits ) * bitspertile;
				repacked[idx] |= ucMappings[tile] << bit;
			}
		}
	}

	if( indicesAndData )
	{
		queueForDelete( indicesAndData );
	}
	indicesAndData = newIndicesAndData;
	allocatedSize = memToAlloc;
	LeaveCriticalSection(&cs_write);
}

#ifdef PSVITA_PRECOMPUTED_TABLE

void CompressedTileStorage::getData(byteArray retArray, unsigned int retOffset)
{
	unsigned short *blockIndices = (unsigned short *)indicesAndData;
	unsigned char *data = indicesAndData + 1024;

	int k = 0;
	unsigned char *Array = &retArray.data[retOffset];
	int *Table = CompressedTile_StorageIndexTable;
	for( int i = 0; i < 512; i++ )
	{
		int indexType = blockIndices[i] & INDEX_TYPE_MASK;

		int index = ( ( i & 0x180) << 6 ) | ( ( i & 0x060 ) << 4 ) | ( ( i & 0x01f ) << 2 );
		unsigned char *NewArray = &Array[index];

		if( indexType == INDEX_TYPE_0_OR_8_BIT )
		{
			if( blockIndices[i] & INDEX_TYPE_0_BIT_FLAG )
			{
				unsigned char val = ( blockIndices[i] >> INDEX_TILE_SHIFT ) & INDEX_TILE_MASK;
				for( int j = 0; j < 64; j++ )
				{
					NewArray[Table[j]] = val;
				}
			}
			else
			{
				unsigned short *packed = (unsigned short *)(data + ( ( blockIndices[i] >> INDEX_OFFSET_SHIFT ) & INDEX_OFFSET_MASK ));

				for( int j = 0; j < 64; j++ )
				{
					unsigned short val = packed[j];
					NewArray[Table[j]] = val < 512 ? val : 0;
				}
			}
		}
		else
		{

			int bitspertile = 1 << indexType;			// will be 1, 2 or 4 (from index values of 0, 1, 2)
			int tiletypecount = 1 << bitspertile;		// will be 2, 4 or 16 (from index values of 0, 1, 2)
			int tiletypemask = tiletypecount - 1;		// will be 1, 3 or 15 (from index values of 0, 1, 2)
			int indexshift = 3 - indexType;				// will be 3, 2 or 1 (from index values of 0, 1, 2)
			int indexmask_bits = 7 >> indexType;		// will be 7, 3 or 1 (from index values of 0, 1, 2)
			int indexmask_bytes = 62 >> indexshift;		// will be 7, 15 or 31 (from index values of 0, 1, 2)

			unsigned short *tile_types = (unsigned short *)(data + ( ( blockIndices[i] >> INDEX_OFFSET_SHIFT ) & INDEX_OFFSET_MASK ));
			unsigned char *packed = (unsigned char *)(tile_types + tiletypecount);

			for( int j = 0; j < 64; j++ )
			{
				int idx = ( j >> indexshift ) & indexmask_bytes;
				int bit = ( j & indexmask_bits ) << indexType;
				unsigned short val = tile_types[( packed[idx] >> bit ) & tiletypemask];
				NewArray[Table[j]] = val < 512 ? val : 0;
			}
		}
	}
}

void CompressedTileStorage::getData16(unsigned short *retArray, unsigned int retOffset)
{
	unsigned short *blockIndices = (unsigned short *)indicesAndData;
	unsigned char *data = indicesAndData + 1024;

	unsigned short *Array = &retArray[retOffset];
	int *Table = CompressedTile_StorageIndexTable;
	for( int i = 0; i < 512; i++ )
	{
		int indexType = blockIndices[i] & INDEX_TYPE_MASK;

		int index = ( ( i & 0x180) << 6 ) | ( ( i & 0x060 ) << 4 ) | ( ( i & 0x01f ) << 2 );
		unsigned short *NewArray = &Array[index];

		if( indexType == INDEX_TYPE_0_OR_8_BIT )
		{
			if( blockIndices[i] & INDEX_TYPE_0_BIT_FLAG )
			{
				unsigned short val = ( blockIndices[i] >> INDEX_TILE_SHIFT ) & INDEX_TILE_MASK;
				for( int j = 0; j < 64; j++ )
				{
					NewArray[Table[j]] = val;
				}
			}
			else
			{
				unsigned short *packed = (unsigned short *)(data + ( ( blockIndices[i] >> INDEX_OFFSET_SHIFT ) & INDEX_OFFSET_MASK ));

				for( int j = 0; j < 64; j++ )
				{
					unsigned short val = packed[j];
					NewArray[Table[j]] = val < 512 ? val : 0;
				}
			}
		}
		else
		{
			int bitspertile = 1 << indexType;
			int tiletypecount = 1 << bitspertile;
			int tiletypemask = tiletypecount - 1;
			int indexshift = 3 - indexType;
			int indexmask_bits = 7 >> indexType;
			int indexmask_bytes = 62 >> indexshift;

			unsigned short *tile_types = (unsigned short *)(data + ( ( blockIndices[i] >> INDEX_OFFSET_SHIFT ) & INDEX_OFFSET_MASK ));
			unsigned char *packed = (unsigned char *)(tile_types + tiletypecount);

			for( int j = 0; j < 64; j++ )
			{
				int idx = ( j >> indexshift ) & indexmask_bytes;
				int bit = ( j & indexmask_bits ) << indexType;
				unsigned short val = tile_types[( packed[idx] >> bit ) & tiletypemask];
				NewArray[Table[j]] = val < 512 ? val : 0;
			}
		}
	}
}

#else

void CompressedTileStorage::getData(byteArray retArray, unsigned int retOffset)
{
	unsigned int localAllocatedSize = allocatedSize;
	unsigned char *localIndicesAndData = indicesAndData;
	if(!localIndicesAndData) return;

	unsigned short *blockIndices = (unsigned short *)localIndicesAndData;
	unsigned char *data = localIndicesAndData + 1024;

	for( int i = 0; i < 512; i++ )
	{
		int indexType = blockIndices[i] & INDEX_TYPE_MASK;
		if( indexType == INDEX_TYPE_0_OR_8_BIT )
		{
			if( blockIndices[i] & INDEX_TYPE_0_BIT_FLAG )
			{
				for( int j = 0; j < 64; j++ )
				{
					retArray[getIndex(i,j) + retOffset] = ( blockIndices[i] >> INDEX_TILE_SHIFT ) & INDEX_TILE_MASK;
				}
			}
			else
			{
				unsigned int off = ( blockIndices[i] >> INDEX_OFFSET_SHIFT ) & INDEX_OFFSET_MASK;
				if( ( localAllocatedSize >= 1024 ) && ( off + 128 <= (unsigned int)( localAllocatedSize - 1024 ) ) )
				{
					unsigned short *packed = (unsigned short *)(data + off);

					for( int j = 0; j < 64; j++ )
					{
						unsigned short val = packed[j];
						retArray[getIndex(i,j) + retOffset] = val < 512 ? val : 0;
					}
				}
				else
				{
					for( int j = 0; j < 64; j++ )
					{
						retArray[getIndex(i,j) + retOffset] = 0;
					}
				}
			}
		}
		else
		{
			int bitspertile = 1 << indexType;			// will be 1, 2 or 4 (from index values of 0, 1, 2)
			int tiletypecount = 1 << bitspertile;		// will be 2, 4 or 16 (from index values of 0, 1, 2)
			int tiletypemask = tiletypecount - 1;		// will be 1, 3 or 15 (from index values of 0, 1, 2)
			int indexshift = 3 - indexType;				// will be 3, 2 or 1 (from index values of 0, 1, 2)
			int indexmask_bits = 7 >> indexType;		// will be 7, 3 or 1 (from index values of 0, 1, 2)
			int indexmask_bytes = 62 >> indexshift;		// will be 7, 15 or 31 (from index values of 0, 1, 2)

			unsigned int off = ( blockIndices[i] >> INDEX_OFFSET_SHIFT ) & INDEX_OFFSET_MASK;
			if( ( localAllocatedSize >= 1024 ) && ( off + ( tiletypecount * 2 ) + ( 8 << indexType ) <= (unsigned int)( localAllocatedSize - 1024 ) ) )
			{
				unsigned short *tile_types = (unsigned short *)(data + off);
				unsigned char *packed = (unsigned char *)(tile_types + tiletypecount);

				for( int j = 0; j < 64; j++ )
				{
					int idx = ( j >> indexshift ) & indexmask_bytes;
					int bit = ( j & indexmask_bits ) * bitspertile;
					unsigned short val = tile_types[( packed[idx] >> bit ) & tiletypemask];
					retArray[getIndex(i,j) + retOffset] = val < 512 ? val : 0;
				}
			}
			else
			{
				for( int j = 0; j < 64; j++ )
				{
					retArray[getIndex(i,j) + retOffset] = 0;
				}
			}
		}
	}
}

void CompressedTileStorage::getData16(unsigned short *retArray, unsigned int retOffset)
{
	unsigned int localAllocatedSize = allocatedSize;
	unsigned char *localIndicesAndData = indicesAndData;
	if(!localIndicesAndData) return;

	unsigned short *blockIndices = (unsigned short *)localIndicesAndData;
	unsigned char *data = localIndicesAndData + 1024;

	for( int i = 0; i < 512; i++ )
	{
		int indexType = blockIndices[i] & INDEX_TYPE_MASK;
		if( indexType == INDEX_TYPE_0_OR_8_BIT )
		{
			if( blockIndices[i] & INDEX_TYPE_0_BIT_FLAG )
			{
				unsigned short val = ( blockIndices[i] >> INDEX_TILE_SHIFT ) & INDEX_TILE_MASK;
				for( int j = 0; j < 64; j++ )
				{
					retArray[getIndex(i,j) + retOffset] = val;
				}
			}
			else
			{
				unsigned int off = ( blockIndices[i] >> INDEX_OFFSET_SHIFT ) & INDEX_OFFSET_MASK;
				if( ( localAllocatedSize >= 1024 ) && ( off + 128 <= (unsigned int)( localAllocatedSize - 1024 ) ) )
				{
					unsigned short *packed = (unsigned short *)(data + off);

					for( int j = 0; j < 64; j++ )
					{
						unsigned short val = packed[j];
						retArray[getIndex(i,j) + retOffset] = val < 512 ? val : 0;
					}
				}
				else
				{
					for( int j = 0; j < 64; j++ )
					{
						retArray[getIndex(i,j) + retOffset] = 0;
					}
				}
			}
		}
		else
		{
			int bitspertile = 1 << indexType;
			int tiletypecount = 1 << bitspertile;
			int tiletypemask = tiletypecount - 1;
			int indexshift = 3 - indexType;
			int indexmask_bits = 7 >> indexType;
			int indexmask_bytes = 62 >> indexshift;

			unsigned int off = ( blockIndices[i] >> INDEX_OFFSET_SHIFT ) & INDEX_OFFSET_MASK;
			if( ( localAllocatedSize >= 1024 ) && ( off + ( tiletypecount * 2 ) + ( 8 << indexType ) <= (unsigned int)( localAllocatedSize - 1024 ) ) )
			{
				unsigned short *tile_types = (unsigned short *)(data + off);
				unsigned char *packed = (unsigned char *)(tile_types + tiletypecount);

				for( int j = 0; j < 64; j++ )
				{
					int idx = ( j >> indexshift ) & indexmask_bytes;
					int bit = ( j & indexmask_bits ) * bitspertile;
					unsigned short val = tile_types[( packed[idx] >> bit ) & tiletypemask];
					retArray[getIndex(i,j) + retOffset] = val < 512 ? val : 0;
				}
			}
			else
			{
				for( int j = 0; j < 64; j++ )
				{
					retArray[getIndex(i,j) + retOffset] = 0;
				}
			}
		}
	}
}

#endif

int  CompressedTileStorage::get(int x, int y, int z)
{
	unsigned int localAllocatedSize = allocatedSize;
	unsigned char *localIndicesAndData = indicesAndData;
	if(!localIndicesAndData) return 0;

	unsigned short *blockIndices = (unsigned short *)localIndicesAndData;
	unsigned char *data = localIndicesAndData + 1024;

	int block, tile;
	getBlockAndTile( &block, &tile, x, y, z );
	int indexType = blockIndices[block] & INDEX_TYPE_MASK;

	if( indexType == INDEX_TYPE_0_OR_8_BIT )
	{
		if( blockIndices[block] & INDEX_TYPE_0_BIT_FLAG )
		{
			return ( blockIndices[block] >> INDEX_TILE_SHIFT ) & INDEX_TILE_MASK;
		}
		else
		{
			unsigned int off = ( blockIndices[block] >> INDEX_OFFSET_SHIFT ) & INDEX_OFFSET_MASK;
			if( ( localAllocatedSize >= 1024 ) && ( off + 128 <= (unsigned int)( localAllocatedSize - 1024 ) ) )
			{
				int val = ((unsigned short *)(data + off))[tile];
				return val < 512 ? val : 0;
			}
			return 0;
		}
	}
	else
	{
		int bitspertile = 1 << indexType;			// will be 1, 2 or 4 (from index values of 0, 1, 2)
		int tiletypecount = 1 << bitspertile;		// will be 2, 4 or 16 (from index values of 0, 1, 2)
		int tiletypemask = tiletypecount - 1;		// will be 1, 3 or 15 (from index values of 0, 1, 2)
		int indexshift = 3 - indexType;				// will be 3, 2 or 1 (from index values of 0, 1, 2)
		int indexmask_bits = 7 >> indexType;		// will be 7, 3 or 1 (from index values of 0, 1, 2)
		int indexmask_bytes = 62 >> indexshift;		// will be 7, 15 or 31 (from index values of 0, 1, 2)

		unsigned int off = ( blockIndices[block] >> INDEX_OFFSET_SHIFT ) & INDEX_OFFSET_MASK;
		if( ( localAllocatedSize >= 1024 ) && ( off + ( tiletypecount * 2 ) + ( 8 << indexType ) <= (unsigned int)( localAllocatedSize - 1024 ) ) )
		{
			unsigned short *tile_types = (unsigned short *)(data + off);
			unsigned char *packed = (unsigned char *)(tile_types + tiletypecount);
			int idx = ( tile >> indexshift ) & indexmask_bytes;
			int bit = ( tile & indexmask_bits ) * bitspertile;
			int val = tile_types[( packed[idx] >> bit ) & tiletypemask];
			return val < 512 ? val : 0;
		}
		return 0;
	}
	return 0;
}

void CompressedTileStorage::set(int x, int y, int z, int val)
{
	EnterCriticalSection(&cs_write);
	assert(val != 0xffff );
	assert(val >= 0 && val < 512);
	int block, tile;
	getBlockAndTile( &block, &tile, x, y, z );

	for( int pass = 0; pass < 2; pass++ )
	{
		unsigned short *blockIndices = (unsigned short *)indicesAndData;
		unsigned char *data = indicesAndData + 1024;

		int indexType = blockIndices[block] & INDEX_TYPE_MASK;

		if( indexType == INDEX_TYPE_0_OR_8_BIT )
		{
			if( blockIndices[block] & INDEX_TYPE_0_BIT_FLAG )
			{
				if ( val == ( ( blockIndices[block] >> INDEX_TILE_SHIFT ) & INDEX_TILE_MASK ) )
				{
					LeaveCriticalSection(&cs_write);
					return;
				}
			}
			else
			{
				unsigned int off = ( blockIndices[block] >> INDEX_OFFSET_SHIFT ) & INDEX_OFFSET_MASK;
				if( ( allocatedSize >= 1024 ) && ( off + 128 <= (unsigned int)( allocatedSize - 1024 ) ) )
				{
					unsigned short *packed = (unsigned short *)(data + off);
					packed[ tile ] = val;
					LeaveCriticalSection(&cs_write);
					return;
				}
			}
		}
		else
		{
			int bitspertile = 1 << indexType;			// will be 1, 2 or 4 (from index values of 0, 1, 2)
			int tiletypecount = 1 << bitspertile;		// will be 2, 4 or 16 (from index values of 0, 1, 2)
			int tiletypemask = tiletypecount - 1;		// will be 1, 3 or 15 (from index values of 0, 1, 2)
			int indexshift = 3 - indexType;				// will be 3, 2 or 1 (from index values of 0, 1, 2)
			int indexmask_bits = 7 >> indexType;		// will be 7, 3 or 1 (from index values of 0, 1, 2)
			int indexmask_bytes = 62 >> indexshift;		// will be 7, 15 or 31 (from index values of 0, 1, 2)

			unsigned int off = ( blockIndices[block] >> INDEX_OFFSET_SHIFT ) & INDEX_OFFSET_MASK;
			unsigned int dataSize = ( tiletypecount * 2 ) + ( 8 << indexType );
			if( ( allocatedSize >= 1024 ) && ( off + dataSize <= (unsigned int)( allocatedSize - 1024 ) ) )
			{
				unsigned short *tile_types = (unsigned short *)(data + off);

				for( int i = 0; i < tiletypecount; i++ )
				{
					if( ( tile_types[i] == val ) || ( tile_types[i] == 0xffff ) )
					{
						tile_types[i] = val;
						unsigned char *packed = (unsigned char *)(tile_types + tiletypecount);
						int idx = ( tile >> indexshift ) & indexmask_bytes;
						int bit = ( tile & indexmask_bits ) * bitspertile;
						packed[idx] &= ~( tiletypemask << bit );
						packed[idx] |= i << bit;
						LeaveCriticalSection(&cs_write);
						return;
					}
				}
			}
		}
		if( pass == 0 )
		{
			compress(block);
		}
	};
	LeaveCriticalSection(&cs_write);
}

int  CompressedTileStorage::setDataRegion(byteArray dataIn, int x0, int y0, int z0, int x1, int y1, int z1, int offset, tileUpdatedCallback callback, void *param, int yparam)
{
	unsigned short *pusIn = (unsigned short *)(dataIn.data + offset);

	if( callback )
	{
		for( int x = x0; x < x1; x++ )
		{
			for( int z = z0; z < z1; z++ )
			{
				for( int y = y0; y < y1; y++ )
				{
					int v = *pusIn;
					if( v >= 512 ) v = 0;
					if(get(x, y, z) != v)
					{
						set(x, y, z, v);
						callback(x, y, z, param, yparam);
					}
					pusIn++;
				}
			}
		}
	}
	else
	{
		for( int x = x0; x < x1; x++ )
		{
			for( int z = z0; z < z1; z++ )
			{
				for( int y = y0; y < y1; y++ )
				{
					int v = *pusIn++;
					if( v >= 512 ) v = 0;
					set(x, y, z, v);
				}
			}
		}
	}
	ptrdiff_t count = (pusIn - (unsigned short *)(dataIn.data + offset)) * sizeof(unsigned short);

	return static_cast<int>(count);
}

bool  CompressedTileStorage::testSetDataRegion(byteArray dataIn, int x0, int y0, int z0, int x1, int y1, int z1, int offset)
{
	unsigned short *pusIn = (unsigned short *)(dataIn.data + offset);
	for( int x = x0; x < x1; x++ )
	{
		for( int z = z0; z < z1; z++ )
		{
			for( int y = y0; y < y1; y++ )
			{
				if(get(x, y, z) != *pusIn++)
				{
					return true;
				}
			}
		}
	}
	return false;
}

int  CompressedTileStorage::getDataRegion(byteArray dataInOut, int x0, int y0, int z0, int x1, int y1, int z1, int offset)
{
	unsigned short *pusOut = (unsigned short *)(dataInOut.data + offset);
	for( int x = x0; x < x1; x++ )
	{
		for( int z = z0; z < z1; z++ )
		{
			for( int y = y0; y < y1; y++ )
			{
				*pusOut++ = static_cast<unsigned short>(get(x, y, z));
			}
		}
	}
	ptrdiff_t count = (pusOut - (unsigned short *)(dataInOut.data + offset)) * sizeof(unsigned short);

	return static_cast<int>(count);
}

void CompressedTileStorage::staticCtor()
{
	InitializeCriticalSectionAndSpinCount(&cs_write, 5120);
	for( int i = 0; i < 3; i++ )
	{
		deleteQueue[i].Initialize();
	}
}

void CompressedTileStorage::queueForDelete(unsigned char *data)
{
	if( data )
	{
		deleteQueue[deleteQueueIndex].Push( data );
	}
}

void CompressedTileStorage::tick()
{
	int freeIndex = ( deleteQueueIndex + 1 ) % 3;

	unsigned char *toFree = nullptr;
	do
	{
		toFree = deleteQueue[freeIndex].Pop();
#if 1
		if( toFree ) XPhysicalFree(toFree);
#else
		if( (unsigned int)toFree >= MM_PHYSICAL_4KB_BASE )
		{
			XPhysicalFree(toFree);
		}
		else
		{
			free(toFree);
		}
#endif
	} while( toFree );

	deleteQueueIndex = ( deleteQueueIndex + 1 ) % 3;
}

#ifdef __PS3__
void  CompressedTileStorage::compress_SPU(int upgradeBlock/*=-1*/)
{
	EnterCriticalSection(&cs_write);
	static unsigned char compBuffer[32768+4096]  __attribute__((__aligned__(16)));
	CompressedTileStorage_compress_dataIn& dataIn = g_compressTileDataIn[0];
	dataIn.allocatedSize = allocatedSize;
	dataIn.indicesAndData = indicesAndData;
	dataIn.newIndicesAndData = compBuffer;
	dataIn.upgradeBlock = upgradeBlock;

	static C4JSpursJobQueue::Port p("CompressedTileStorage::compress_SPU");
	C4JSpursJob_CompressedTileStorage_compress compressJob(&dataIn);
	p.submitJob(&compressJob);
	p.waitForCompletion();

	if(dataIn.neededCompressed)
	{
		unsigned char *newIndicesAndData = (unsigned char *)XPhysicalAlloc(dataIn.newAllocatedSize, MAXULONG_PTR, 4096, PAGE_READWRITE);//(unsigned char *)malloc( memToAlloc );
		memcpy(newIndicesAndData, compBuffer, dataIn.newAllocatedSize);
		queueForDelete( indicesAndData );
		indicesAndData = newIndicesAndData;
		allocatedSize = dataIn.newAllocatedSize;
	}

	LeaveCriticalSection(&cs_write);

}
#endif

void  CompressedTileStorage::compress(int upgradeBlock/*=-1*/)
{
#if defined __PS3__ && !defined DISABLE_SPU_CODE
	compress_SPU(upgradeBlock);
		return;
#endif

	unsigned short tempdata[64];
	unsigned short _blockIndices[512];

	if( ( allocatedSize == 1024 ) && ( upgradeBlock == -1 ) ) return;

	bool needsCompressed = ( upgradeBlock > -1 );

	EnterCriticalSection(&cs_write);

	unsigned short *blockIndices = (unsigned short *)indicesAndData;
	unsigned char *data = indicesAndData + 1024;

	int memToAlloc = 0;
	for( int i = 0; i < 512; i++ )
	{
		unsigned short indexType = blockIndices[i] & INDEX_TYPE_MASK;

		unsigned short *unpacked_data = nullptr;
		unsigned char *packed_data;

		if( upgradeBlock == -1 )
		{
			if( indexType == INDEX_TYPE_0_OR_8_BIT )
			{
				if( ( blockIndices[i] & INDEX_TYPE_0_BIT_FLAG ) == 0 )
				{
					unsigned int off = ( blockIndices[i] >> INDEX_OFFSET_SHIFT ) & INDEX_OFFSET_MASK;
					if( ( allocatedSize >= 1024 ) && ( off + 128 <= (unsigned int)( allocatedSize - 1024 ) ) )
					{
						unpacked_data = (unsigned short *)(data + off);
					}
				}
			}
			else
			{
				int bitspertile = 1 << indexType;			// will be 1, 2 or 4 (from index values of 0, 1, 2)
				int tiletypecount = 1 << bitspertile;		// will be 2, 4 or 16
				int tiletypemask = tiletypecount - 1;		// will be 1, 3 or 15
				int indexshift = 3 - indexType;				// will be 3, 2 or 1 (from index values of 0, 1, 2)
				int indexmask_bits = 7 >> indexType;		// will be 7, 3 or 1 (from index values of 0, 1, 2)
				int indexmask_bytes = 62 >> indexshift;		// will be 7, 15 or 31 (from index values of 0, 1, 2)

				unsigned int off = ( blockIndices[i] >> INDEX_OFFSET_SHIFT ) & INDEX_OFFSET_MASK;
				if( ( allocatedSize >= 1024 ) && ( off + ( tiletypecount * 2 ) + ( 8 << indexType ) <= (unsigned int)( allocatedSize - 1024 ) ) )
				{
					unpacked_data = tempdata;
					packed_data = (unsigned char *)((unsigned short *)(data + off) + tiletypecount);

					for( int j = 0; j < 64; j++ )
					{
						int idx = (j >> indexshift) & indexmask_bytes;
						int bit = ( j & indexmask_bits ) * bitspertile;

						unpacked_data[j] = ( packed_data[idx] >> bit ) & tiletypemask;	// Doesn't need the actual data for each tile, just unique values
					}
				}
			}

			if( unpacked_data )
			{
#ifdef __PSVITA__
				unsigned int usedFlags[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
				__int32 i32_1 = 1;
				for( int j = 0; j < 64; j++ )
				{
					int tiletype = unpacked_data[j];
					if( tiletype < 512 ) usedFlags[tiletype & 15] |= ( i32_1 << ( tiletype >> 4 ) );
				}
				unsigned int count = 0;
				for( int Index = 0;Index < 16;Index += 1 )
				{
					unsigned int i = usedFlags[Index];
					i = i - ((i >> 1) & 0x55555555);
					i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
					count += (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
				}
#else

				uint64_t usedFlags[8] = {0,0,0,0,0,0,0,0};
				int64_t i64_1 = 1;
				for( int j = 0; j < 64; j++ )
				{
					int tiletype = unpacked_data[j];
					if( tiletype < 512 ) usedFlags[tiletype & 7] |= ( i64_1 << ( tiletype >> 3 ) );
				}
				int count = 0;
				for( int tiletype = 0; tiletype < 512; tiletype++ )
				{
					if( usedFlags[tiletype & 7] & ( i64_1 << ( tiletype >> 3 ) ) )
					{
						count++;
					}
				}
#endif

				if( count == 1 )
				{
					_blockIndices[i] = INDEX_TYPE_0_OR_8_BIT | INDEX_TYPE_0_BIT_FLAG;

					needsCompressed = true;
				}
				else if( count == 2 )
				{
					_blockIndices[i] = INDEX_TYPE_1_BIT;
					if( indexType != INDEX_TYPE_1_BIT ) needsCompressed = true;
					memToAlloc += 12;
				}
				else if ( count <= 4 )
				{
					_blockIndices[i] = INDEX_TYPE_2_BIT;
					if( indexType != INDEX_TYPE_2_BIT ) needsCompressed = true;
					memToAlloc += 24;
				}
				else if ( count <= 16 )
				{
					_blockIndices[i] = INDEX_TYPE_4_BIT;
					if( indexType != INDEX_TYPE_4_BIT ) needsCompressed = true;
					memToAlloc += 64;
				}
				else
				{
					_blockIndices[i] = INDEX_TYPE_0_OR_8_BIT;
					memToAlloc = ( memToAlloc + 7 ) & 0xfff8;
					memToAlloc += 128;
				}
			}
			else
			{
				_blockIndices[i] = blockIndices[i];
			}
		}
		else
		{
			if( i == upgradeBlock )
			{
				_blockIndices[i] = ( ( blockIndices[i] & INDEX_TYPE_MASK ) + 1 ) & INDEX_TYPE_MASK;
			}
			else
			{
				_blockIndices[i] = blockIndices[i] & INDEX_TYPE_MASK;
				if( _blockIndices[i] == INDEX_TYPE_0_OR_8_BIT )
				{
					_blockIndices[i] |= ( blockIndices[i] & INDEX_TYPE_0_BIT_FLAG );
				}
			}
			switch(_blockIndices[i])
			{
				case INDEX_TYPE_1_BIT:
					memToAlloc += 12;
					break;
				case INDEX_TYPE_2_BIT:
					memToAlloc += 24;
					break;
				case INDEX_TYPE_4_BIT:
					memToAlloc += 64;
					break;
				case INDEX_TYPE_0_OR_8_BIT:
					memToAlloc = ( memToAlloc + 7 ) & 0xfff8;
					memToAlloc += 128;
					break;
			}
		}
	}

	if( needsCompressed )
	{
		memToAlloc += 1024;
		unsigned char *newIndicesAndData = static_cast<unsigned char *>(XPhysicalAlloc(memToAlloc, MAXULONG_PTR, 4096, PAGE_READWRITE));//(unsigned char *)malloc( memToAlloc );
		if( newIndicesAndData == nullptr )
		{
			DWORD lastError = GetLastError();
#ifndef _DURANGO
			MEMORYSTATUS memStatus;
			GlobalMemoryStatus(&memStatus);
			__debugbreak();
#endif
		}
		unsigned char *pucData = newIndicesAndData + 1024;
		unsigned short usDataOffset = 0;
		unsigned short *newIndices = (unsigned short *) newIndicesAndData;

		for( int i = 0; i < 512; i++ )
		{
			unsigned short indexTypeNew = _blockIndices[i] & INDEX_TYPE_MASK;
			unsigned short indexTypeOld = blockIndices[i] & INDEX_TYPE_MASK;
			newIndices[i] = indexTypeNew;

			bool done = false;
			if( indexTypeOld == indexTypeNew )
			{
				unsigned char *packed_data;
				if( indexTypeOld == INDEX_TYPE_0_OR_8_BIT )
				{
					if( ( blockIndices[i] & INDEX_TYPE_0_BIT_FLAG ) == ( _blockIndices[i] & INDEX_TYPE_0_BIT_FLAG ) )
					{
						if( blockIndices[i] & INDEX_TYPE_0_BIT_FLAG )
						{
							newIndices[i] = blockIndices[i];
						}
						else
						{
							unsigned int off = ( blockIndices[i] >> INDEX_OFFSET_SHIFT ) & INDEX_OFFSET_MASK;
							if( ( allocatedSize >= 1024 ) && ( off + 128 <= (unsigned int)( allocatedSize - 1024 ) ) )
							{
								packed_data = data + off;
								usDataOffset = (usDataOffset + 7 ) & 0xfff8;
								XMemCpy( pucData + usDataOffset, packed_data, 128 );
								newIndices[i] |= ( usDataOffset & INDEX_OFFSET_MASK) << INDEX_OFFSET_SHIFT;
								usDataOffset += 128;
							}
							else
							{
								newIndices[i] = INDEX_TYPE_0_OR_8_BIT | INDEX_TYPE_0_BIT_FLAG;
							}
						}
						done = true;
					}
				}
				else
				{
					unsigned int off = ( blockIndices[i] >> INDEX_OFFSET_SHIFT ) & INDEX_OFFSET_MASK;

					int dataSize = 8 << indexTypeOld;
					dataSize += 2 << ( 1 << indexTypeOld );
					if( ( allocatedSize >= 1024 ) && ( off + dataSize <= (unsigned int)( allocatedSize - 1024 ) ) )
					{
						packed_data = data + off;
						newIndices[i] |= ( usDataOffset & INDEX_OFFSET_MASK) << INDEX_OFFSET_SHIFT;
						XMemCpy( pucData + usDataOffset, packed_data, dataSize );
						usDataOffset += dataSize;
					}
					else
					{
						newIndices[i] = INDEX_TYPE_0_OR_8_BIT | INDEX_TYPE_0_BIT_FLAG;
					}
					done = true;
				}
			}


			if( !done )
			{
				unsigned short *unpacked_data = nullptr;
				unsigned short *tile_types = nullptr;
				unsigned char *packed_data = nullptr;
				if( indexTypeOld == INDEX_TYPE_0_OR_8_BIT )
				{
					if( blockIndices[i] & INDEX_TYPE_0_BIT_FLAG )
					{
						unpacked_data  = tempdata;
						int value = ( blockIndices[i] >> INDEX_TILE_SHIFT ) & INDEX_TILE_MASK;
						for( int j = 0; j < 64; j++ ) tempdata[j] = value;
					}
					else
					{
						unsigned int off = ( blockIndices[i] >> INDEX_OFFSET_SHIFT ) & INDEX_OFFSET_MASK;
						if( ( allocatedSize >= 1024 ) && ( off + 128 <= (unsigned int)( allocatedSize - 1024 ) ) )
						{
							unpacked_data = (unsigned short *)(data + off);
						}
					}
				}
				else
				{
					int bitspertile = 1 << indexTypeOld;		// will be 1, 2 or 4 (from index values of 0, 1, 2)
					int tiletypecount = 1 << bitspertile;		// will be 2, 4 or 16
					int tiletypemask = tiletypecount - 1;		// will be 1, 3 or 15
					int indexshift = 3 - indexTypeOld;			// will be 3, 2 or 1 (from index values of 0, 1, 2)
					int indexmask_bits = 7 >> indexTypeOld;		// will be 7, 3 or 1 (from index values of 0, 1, 2)
					int indexmask_bytes = 62 >> indexshift;		// will be 7, 15 or 31 (from index values of 0, 1, 2)

					unpacked_data = tempdata;
					unsigned int off = ( blockIndices[i] >> INDEX_OFFSET_SHIFT ) & INDEX_OFFSET_MASK;
					if( ( allocatedSize >= 1024 ) && ( off + ( tiletypecount * 2 ) + ( 8 << indexTypeOld ) <= (unsigned int)( allocatedSize - 1024 ) ) )
					{
						tile_types = (unsigned short *)(data + off);
						packed_data = (unsigned char *)(tile_types + tiletypecount);
						for( int j = 0; j < 64; j++ )
						{
							int idx = ( j >> indexshift ) & indexmask_bytes;
							int bit = ( j & indexmask_bits ) * bitspertile;

							unpacked_data[j] = tile_types[(packed_data[idx] >> bit) & tiletypemask];
						}
					}
					else
					{
						for( int j = 0; j < 64; j++ ) unpacked_data[j] = 0;
					}
				}

				if( !unpacked_data )
				{
					unpacked_data = tempdata;
					for( int j = 0; j < 64; j++ ) tempdata[j] = 0;
				}

				unsigned char ucMappings[512] = {0};
#ifdef __PSVITA__
				memset(ucMappings, 255, 512);
#else
				for( int j = 0; j < 512; j++ )
				{
					ucMappings[j] = 255;
				}
#endif

				unsigned char *repacked = nullptr;

				if( indexTypeNew == INDEX_TYPE_0_OR_8_BIT )
				{
					if( _blockIndices[i] & INDEX_TYPE_0_BIT_FLAG )
					{
						unsigned short tile0 = unpacked_data[0];
						if( tile0 >= 512 ) tile0 = 0;
						newIndices[i] = INDEX_TYPE_0_OR_8_BIT | INDEX_TYPE_0_BIT_FLAG | (static_cast<unsigned short>(tile0) << INDEX_TILE_SHIFT);
					}
					else
					{
						usDataOffset = (usDataOffset + 7 ) & 0xfff8;
						XMemCpy( pucData + usDataOffset, unpacked_data, 128 );
						newIndices[i] |= ( usDataOffset & INDEX_OFFSET_MASK) << INDEX_OFFSET_SHIFT;
						usDataOffset += 128;
					}
				}
				else
				{
					int bitspertile = 1 << indexTypeNew;		// will be 1, 2 or 4 (from index values of 0, 1, 2)
					int tiletypecount = 1 << bitspertile;		// will be 2, 4 or 16 (from index values of 0, 1, 2)
					int tiletypemask = tiletypecount - 1;		// will be 1, 3 or 15 (from index values of 0, 1, 2)
					int tiledatasize = 8 << indexTypeNew;		// will be 8, 16 or 32 (from index values of 0, 1, 2)
					int indexshift = 3 - indexTypeNew;			// will be 3, 2 or 1 (from index values of 0, 1, 2)
					int indexmask_bits = 7 >> indexTypeNew;		// will be 7, 3 or 1 (from index values of 0, 1, 2)
					int indexmask_bytes = 62 >> indexshift;		// will be 7, 15 or 31 (from index values of 0, 1, 2)

					tile_types = (unsigned short *)(pucData + usDataOffset);
					repacked = (unsigned char *)(tile_types + tiletypecount);
					XMemSet(tile_types, 255, tiletypecount * 2);
					XMemSet(repacked, 0,tiledatasize);
					newIndices[i] |= ( usDataOffset & INDEX_OFFSET_MASK) << INDEX_OFFSET_SHIFT;
					usDataOffset += ( tiletypecount * 2 ) + tiledatasize;
					int count = 0;
					for( int j = 0; j < 64; j++ )
					{
						int tile = unpacked_data[j];
						if( tile >= 512 ) tile = 0;
						if( ucMappings[tile] == 255 )
						{
							ucMappings[tile] = count;
							tile_types[count++] = tile;
						}
						int idx = (j >> indexshift) & indexmask_bytes;
						int bit = ( j & indexmask_bits ) * bitspertile;
						repacked[idx] |= ucMappings[tile] << bit;
					}
				}
			}
		}

		queueForDelete( indicesAndData );
		indicesAndData = newIndicesAndData;
		allocatedSize = memToAlloc;
	}
	LeaveCriticalSection(&cs_write);
}

int CompressedTileStorage::getAllocatedSize(int *count0, int *count1, int *count2, int *count4, int *count8)
{
	*count0 = 0;
	*count1 = 0;
	*count2 = 0;
	*count4 = 0;
	*count8 = 0;

	unsigned char *localIndicesAndData = *(unsigned char *volatile *)&indicesAndData;
	if(!localIndicesAndData) return 0;
	unsigned short *blockIndices = (unsigned short *)localIndicesAndData;
	for(int i = 0; i < 512; i++ )
	{
		unsigned short idxType = blockIndices[i] & INDEX_TYPE_MASK;
		if( idxType == INDEX_TYPE_1_BIT )
		{
			(*count1)++;
		}
		else if( idxType == INDEX_TYPE_2_BIT )
		{
			(*count2)++;
		}
		else if( idxType == INDEX_TYPE_4_BIT )
		{
			(*count4)++;
		}
		else if( idxType == INDEX_TYPE_0_OR_8_BIT )
		{
			if( blockIndices[i] & INDEX_TYPE_0_BIT_FLAG )
			{
				(*count0)++;
			}
			else
			{
				(*count8)++;
			}
		}
	}
	return allocatedSize;
}

int CompressedTileStorage::getHighestNonEmptyY()
{
	unsigned char *localIndicesAndData = indicesAndData;
	if(!localIndicesAndData) return -1;
	unsigned short *blockIndices = (unsigned short *)localIndicesAndData;
	unsigned int highestYBlock = 0;
	bool found = false;

	for(int yBlock = 31; yBlock >= 0; --yBlock)
	{
		for(unsigned int xzBlock = 0; xzBlock < 16; ++xzBlock)
		{
			int index = yBlock + (xzBlock * 32);

			int indexType = blockIndices[index] & INDEX_TYPE_MASK;
			if( indexType == INDEX_TYPE_0_OR_8_BIT && blockIndices[index] & INDEX_TYPE_0_BIT_FLAG )
			{
				int val = ( blockIndices[index] >> INDEX_TILE_SHIFT ) & INDEX_TILE_MASK;
				if(val != 0)
				{
					highestYBlock = yBlock;
					found = true;
					break;
				}
			}
			else
			{
				highestYBlock = yBlock;
				found = true;
				break;
			}
		}

		if(found) break;
	}

	int highestNonEmptyY = -1;
	if(found)
	{
		highestNonEmptyY = (highestYBlock * 4) + 4;
	}
	else if( allocatedSize != 1024 )
	{
		app.DebugPrintf("[CTS-WARN] getHighestNonEmptyY() returned -1! allocatedSize=%d indicesAndData=%p\n",
			allocatedSize, indicesAndData);
	}
	return highestNonEmptyY;
}

void CompressedTileStorage::write(DataOutputStream *dos)
{
	dos->writeInt(allocatedSize);
	unsigned char *localIndicesAndData = *(unsigned char *volatile *)&indicesAndData;
	if(localIndicesAndData)
	{
		if(LOCALSYTEM_ENDIAN == BIGENDIAN)
		{
			byteArray indicesCopy(1024);
			memcpy(indicesCopy.data, localIndicesAndData, 1024);
			reverseIndices(indicesCopy.data);
			dos->write(indicesCopy);
			delete [] indicesCopy.data;

			if(allocatedSize > 1024)
			{
				byteArray dataWrapper(localIndicesAndData + 1024, allocatedSize - 1024);
				dos->write(dataWrapper);
			}
		}
		else
		{
			byteArray wrapper(localIndicesAndData, allocatedSize);
			dos->write(wrapper);
		}
	}
}

void CompressedTileStorage::read(DataInputStream *dis)
{
	allocatedSize = dis->readInt();
	if( allocatedSize < 1024 || allocatedSize > ( 65536 + 1024 ) )
	{
		allocatedSize = 1024;
		if( indicesAndData )
		{
			XPhysicalFree(indicesAndData);
		}
		indicesAndData = static_cast<unsigned char *>(XPhysicalAlloc(1024, MAXULONG_PTR, 4096, PAGE_READWRITE));
		unsigned short *emptyIndices = (unsigned short *)indicesAndData;
		for( int i = 0; i < 512; i++ )
		{
			emptyIndices[i] = INDEX_TYPE_0_OR_8_BIT | INDEX_TYPE_0_BIT_FLAG;
		}
		return;
	}
	if(allocatedSize > 0)
	{
		if( indicesAndData )
		{
			XPhysicalFree(indicesAndData);
		}
		indicesAndData = static_cast<unsigned char *>(XPhysicalAlloc(allocatedSize, MAXULONG_PTR, 4096, PAGE_READWRITE));

		byteArray wrapper(indicesAndData, allocatedSize);
		dis->readFully(wrapper);
		if(LOCALSYTEM_ENDIAN == BIGENDIAN)
		{
			reverseIndices(indicesAndData);
		}

		compress();
	}
}

void CompressedTileStorage::reverseIndices(unsigned char *indices)
{
	unsigned short *blockIndices = (unsigned short *)indices;
	for( int i = 0; i < 512; i++ )
	{
		System::ReverseUSHORT(&blockIndices[i]);
	}
}
