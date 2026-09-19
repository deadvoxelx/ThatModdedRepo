#pragma once

class Texture;
class SimpleIcon;
class StitchedTexture;
class Icon;
class PreStitchedTextureMap;

class ModTextureAtlas {
public:
    static void createInstance();
    static ModTextureAtlas* getInstance();

    void registerTexture(const std::wstring& name, std::vector<int> pixels, int width, int height);
    void build();

    StitchedTexture* getIcon(const std::wstring& name);
    bool isModIcon(Icon* icon);
    int getAtlasGlId();

    void registerPendingTexturesIntoTerrainAtlas(PreStitchedTextureMap* terrainMap);
    void finalizeIntoTerrainMap(PreStitchedTextureMap *terrainMap);
    bool isStitchedToTerrain() const { return stitchedIntoTerrain; }

private:
    static ModTextureAtlas* instance;
    static constexpr int MAX_ATLAS_SIZE = 8192;

    struct PendingTexture {
        std::wstring name;
        std::vector<int> pixels;
        int width, height;
    };

    std::vector<PendingTexture> pending;
    std::unordered_map<std::wstring, SimpleIcon*> icons;
    Texture* atlasTexture = nullptr;
    bool built = false;
    bool stitchedIntoTerrain = false;
};