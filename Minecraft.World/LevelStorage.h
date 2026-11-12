#pragma once
using namespace std;

#include "ConsoleSavePath.h"
#include <map>
#include <string>

class PlayerIO;
class Dimension;
class ChunkStorage;
class LevelData;
class Player;
class File;

class ConsoleSaveFile;

class LevelStorage 
{
public:
	static const wstring NETHER_FOLDER;
	static const wstring ENDER_FOLDER;
    static const wstring OUTEREND_FOLDER;
    static const wstring AETHER_FOLDER;
    static const wstring NUREALM_FOLDER;

	static const wstring &folderForDimension(int id)
	{
		if (id == 0)
		{
			static const wstring overworld = L"";
			return overworld;
		}

		switch (id)
		{
		case -1: return NETHER_FOLDER;
		case 1: return ENDER_FOLDER;
		case 2: return OUTEREND_FOLDER;
		case 3: return AETHER_FOLDER;
		case 4: return NUREALM_FOLDER;
		default: break;
		}

		static map<int, wstring> customFolders;
		wstring &folder = customFolders[id];
		if (folder.empty()) folder = L"dimension_" + to_wstring(id);
		return folder;
	}

    virtual LevelData *prepareLevel() = 0;
    virtual void checkSession() = 0;
    virtual ChunkStorage *createChunkStorage(Dimension *dimension) = 0;
    virtual void saveLevelData(LevelData *levelData, vector<shared_ptr<Player> > *players) = 0;
    virtual void saveLevelData(LevelData *levelData) = 0;
    virtual PlayerIO *getPlayerIO() = 0;
    virtual void closeAll() = 0;
    virtual ConsoleSavePath getDataFile(const wstring& id) = 0;
	virtual wstring getLevelId() = 0;

public:
	virtual ConsoleSaveFile *getSaveFile() { return nullptr; }
	virtual void flushSaveFile(bool autosave) {}

	// 4J Added
	virtual int getAuxValueForMap(PlayerUID xuid, int dimension, int centreXC, int centreZC, int scale) { return 0; }
};
