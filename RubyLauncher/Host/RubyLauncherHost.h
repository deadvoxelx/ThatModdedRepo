#pragma once

#include <string>
#include <vector>

struct RubyModInfo
{
	std::string folderName;
	std::string name;
	std::string version;
	std::string modId;
	bool hasManifest = false;
	bool loaded = false;
	std::string error;
	unsigned long long size = 0;
};

class Entity;
class ItemInstance;
class MinecraftServer;
class ServerLevel;
class ServerPlayer;

enum RubyCreativeGroup
{
	RubyCreativeGroup_BuildingBlocks = 0,
	RubyCreativeGroup_Materials,
	RubyCreativeGroup_Food,
	RubyCreativeGroup_Tools,
	RubyCreativeGroup_Decoration,
};

struct RubyCreativeEntry
{
	int id;
	int aux;
	RubyCreativeGroup group;
};

namespace RubyCreative
{
	void addEntry(int id, int aux, RubyCreativeGroup group);
}

struct RubyModTexture
{
	std::string iconName;
	std::string filePath;
	std::string fallback;
	bool block = true;
};

inline std::string rubyModTextureIconName(const std::string &modId, const std::string &id)
{
	return "mod:" + modId + ":" + id;
}

namespace RubyLoader
{
	void onClientBoot();
	void onServerStart(MinecraftServer *server);
	void onServerTick(MinecraftServer *server);
	void onClientTick();
	void onModNetPacket(ServerPlayer *player, const std::string &channel, const std::string &data);
	void onStringTableReloaded();
	void addModTexture(const RubyModTexture &texture);
	const std::vector<RubyModTexture> &getModTextures();
	int getModCount();
	const wchar_t *getModsFolder();
	std::vector<RubyModInfo> getInstalledMods();
	std::vector<RubyCreativeEntry> getCreativeEntries();
	int registerArmorSet(const std::string &setName, const std::string &modFolderName, int vanillaFallback);
	std::wstring getArmorSetName(int modelIndex);
	bool armorSetHasOverlay(int modelIndex);
	bool resolveArmorTexture(const std::wstring &relativeName, std::wstring &drive, std::wstring &texturePath);
	bool firePlayerBlockBreak(ServerPlayer *player, int x, int y, int z, int blockId, int blockAux);
	bool firePlayerBlockPlace(ServerPlayer *player, int x, int y, int z, ItemInstance *item, int targetTile);
	void fireItemCompleteUse(ItemInstance *item, ServerLevel *level, ServerPlayer *player);
	void fireItemInteract(ItemInstance *item, ServerLevel *level, ServerPlayer *player);
	void fireItemTick(ItemInstance *item, ServerLevel *level, ServerPlayer *player, int slot);
	void fireItemInteractEntity(ItemInstance *item, Entity *entity);
	void firePlayerConnection(ServerPlayer *player);
	void firePlayerJoin(ServerPlayer *player);
	void fireFlightStarted(ServerPlayer *player);
	void fireFlightEnded(ServerPlayer *player);
}
