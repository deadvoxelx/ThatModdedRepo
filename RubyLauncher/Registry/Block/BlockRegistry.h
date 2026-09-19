#pragma once

#include "sol/sol.hpp"

#include <string>
#include <vector>

class ModTile;

enum EBlockTool {
    BlockTool_None = 0,
    BlockTool_Pickaxe,
    BlockTool_Hatchet,
    BlockTool_Shovel,
    BlockTool_Hoe,
};

struct BlockDefinition {
    float hardness = 1.5f;
    float resistance = -1.0f;
    std::string topTexture;
    std::string bottomTexture;
    std::string sound;
    std::string drop;
    int dropMin = 1;
    int dropMax = 1;
    float dropChance = 1.0f;
    bool dropNothing = false;
    int dropId = -1;
    int dropAux = 0;
    bool dropPending = false;
    bool transparent = false;

    EBlockTool tool = BlockTool_Pickaxe;

    BlockDefinition() = default;
    BlockDefinition(sol::table table);

    void finalizeDrops(const std::string &modId, const std::string &context);
};

struct PendingBlockDrop
{
    ModTile *tile;
    std::string drop;
    std::string context;
};

class BlockRegistry {
public:
    static int nextItemId();

    static int maxBlockCount();

    static int registerBlock(const std::wstring& path, const std::string& id, const std::string& name, std::string modId, const std::string& texturePath, const BlockDefinition& def);

    static void finalizeDrops();

private:
    static std::vector<PendingBlockDrop> pendingDrops;
};
