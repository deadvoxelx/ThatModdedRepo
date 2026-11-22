#pragma once

#include "../../../../Minecraft.World/HellSphere.h"
#include "../../../../Minecraft.World/Item.h"
#include "../../../../Minecraft.World/Level.h"
#include "../../../../Minecraft.World/ModProjectile.h"
#include "../../../../Minecraft.World/Player.h"
#include "../../../../Minecraft.World/Snowball.h"
#include "../../../../Minecraft.World/SoundTypes.h"
#include "../ItemDefinition.h"
#include "../ModItem.hpp"

class ModThrowableItem : public ModItemTemplate<Item>
{
public:
    ModThrowableItem(int id, const ItemDefinition &def) : ModItemTemplate<Item>(id, def), projectile(def.projectile)
    {
        this->maxStackSize = 16;
    }

    void setProjectileSpecId(int specId) { projectileSpecId = specId; }

    shared_ptr<ItemInstance> use(shared_ptr<ItemInstance> instance, Level *level, shared_ptr<Player> player) override
    {
        if (itemDefinition.use.valid())
        {
            return ModItemTemplate<Item>::use(instance, level, player);
        }

        if (!player->abilities.instabuild)
        {
            instance->count--;
        }
        level->playEntitySound((shared_ptr<Entity>) player, eSoundType_RANDOM_BOW, 0.5f, 0.4f / (random->nextFloat() * 0.4f + 0.8f));
        player->swing();
        if (!level->isClientSide)
        {
            if (projectileSpecId >= 0)
            {
                level->addEntity(std::make_shared<ModProjectile>(level, player, projectileSpecId));
            }
            else if (projectile == "hellsphere")
            {
                level->addEntity(std::make_shared<HellSphere>(level, player));
            }
            else
            {
                level->addEntity(std::make_shared<Snowball>(level, player));
            }
        }
        return instance;
    }

private:
    std::string projectile;
    int projectileSpecId = -1;
};
