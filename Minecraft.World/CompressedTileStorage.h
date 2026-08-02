#pragma once
#include "xmcore.h"

//#define BLOCK_COMPRESSION_STATS
class TileCompressData_SPU;
class CompressedTileStorage
{
	friend class TileCompressData_SPU;
private:
	unsigned char	*indicesAndData;
public:
	int				allocatedSize;
private:

	static const int INDEX_OFFSET_MASK			= 0xFFFC;
	static const int INDEX_OFFSET_SHIFT			= 0;
	static const int INDEX_TILE_MASK			= 0x01FF;
	static const int INDEX_TILE_SHIFT			= 7;
	static const int INDEX_TYPE_MASK			= 0x0003;
	static const int INDEX_TYPE_1_BIT			= 0x0000;
	static const int INDEX_TYPE_2_BIT			= 0x0001;
	static const int INDEX_TYPE_4_BIT			= 0x0002;
	static const int INDEX_TYPE_0_OR_8_BIT		= 0x0003;
	static const int INDEX_TYPE_0_BIT_FLAG		= 0x0004;

	static const unsigned int MM_PHYSICAL_4KB_BASE = 0xE0000000;
public:
	CompressedTileStorage();
	CompressedTileStorage(CompressedTileStorage *copyFrom);
	CompressedTileStorage(byteArray dataIn, unsigned int initOffset);
	CompressedTileStorage(bool isEmpty);
	~CompressedTileStorage();
	bool isSameAs(CompressedTileStorage *other);
	bool isRenderChunkEmpty(int y);
private:
	inline static int getIndex(int block, int tile);
	inline static void getBlockAndTile(int *block, int *tile, int x, int y, int z);
	inline static void getBlock(int *block, int x, int y, int z);
public:
	void setData(byteArray dataIn, unsigned int inOffset);
	void getData(byteArray retArray, unsigned int retOffset);
	void getData16(unsigned short *retArray, unsigned int retOffset);
    int  get(int x, int y, int z);
    void set(int x, int y, int z, int val);
	typedef void (*tileUpdatedCallback)(int x, int y , int z, void *param, int yparam);
	int  setDataRegion(byteArray dataIn, int x0, int y0, int z0, int x1, int y1, int z1, int offset, tileUpdatedCallback callback, void *param, int yparam);
	bool testSetDataRegion(byteArray dataIn, int x0, int y0, int z0, int x1, int y1, int z1, int offset);
	int  getDataRegion(byteArray dataInOut, int x0, int y0, int z0, int x1, int y1, int z1, int offset);

	static void staticCtor();

	void compress(int upgradeBlock = -1);
#ifdef __PS3__
	void compress_SPU(int upgradeBlock = -1);
#endif

public:
	void queueForDelete(unsigned char *data);

	static void tick();
	static int deleteQueueIndex;
	static XLockFreeStack <unsigned char> deleteQueue[3];

	static unsigned char compressBuffer[32768 + 256];

	static CRITICAL_SECTION cs_write;

	int getAllocatedSize(int *count0, int *count1, int *count2, int *count4, int *count8);
	int getHighestNonEmptyY();
	bool isCompressed();

	void write(DataOutputStream *dos);
	void read(DataInputStream *dis);
	void reverseIndices(unsigned char *indices);
};

