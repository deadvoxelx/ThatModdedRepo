#pragma once

#include "Item.h"
#include "ItemFactory.h"

class ModItem : public Item {
    public:
        ModItem(int id): Item(id) {};
};
