#pragma once

#include <unordered_map>
#include <string>
#include <cstdint>

class Item;

class IDMapping {
public:
    struct MappedItem {
        int id;
        bool isBlock;
        int aux;
    };

    std::unordered_map<std::string, MappedItem> stringToID;
    std::unordered_map<uint32_t, std::string> idToString;

    static IDMapping* get();

    void init();
    void add(std::string ns, std::string name, bool isBlock, int id);
    MappedItem getID(std::string k);

    static uint32_t makeKey(int id, int aux) {
        return (static_cast<uint32_t>(id) << 16) | static_cast<uint32_t>(aux);
    }

    void buildReverseMap();

    std::string getByID(int id, int aux = 0);
    std::string getByStringedID(const std::string& numIdStr);

private:
    IDMapping() = default;
    IDMapping(const IDMapping&) = delete;
    IDMapping& operator=(const IDMapping&) = delete;
};