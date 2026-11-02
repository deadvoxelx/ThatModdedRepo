#pragma once

#include "../../../Minecraft.World/UseAnim.h"
#include "../../Loader.h"
#include "Item.h"
#include "ItemDefinition.h"
#include "ItemInstance.h"
#include "Level.h"
#include "LivingEntity.h"
#include "Player.h"
#include "UseAnim.h"

class ItemInstance;
template <class Base>
class ModItemTemplate : public Base {
public:
    template <class... Args>
    ModItemTemplate(int id, const ItemDefinition &def, Args&&... extra): Base(id, std::forward<Args>(extra)...), itemDefinition(def) {}
    shared_ptr<ItemInstance> use(shared_ptr<ItemInstance> itemInstance, Level *level, shared_ptr<Player> player) override {
        if (itemDefinition.use.valid()) {
            auto result = itemDefinition.use(itemInstance, level, player);

            if (!result.valid()) {
                sol::error err = result;
                Loader::_debugPrint("Error calling item use: "+std::string(err.what()));
            }
        } else {
            return Base::use(itemInstance, level, player);
        }
        return itemInstance;
    };
    void inventoryTick(shared_ptr<ItemInstance> itemInstance, Level *level, shared_ptr<Entity> owner, int slot, bool selected) override {
        if (itemDefinition.tick.valid()) {
            itemDefinition.tick(itemInstance, level, owner, slot, selected);

            //not gonna add a error handler, because... me when no more storage (oops the log ate it all :3)
        } else {
            Base::inventoryTick(itemInstance, level, owner, slot, selected);
        }
    };
    int getUseDuration(shared_ptr<ItemInstance> itemInstance) override {
        if (itemDefinition.useDuration.valid()) {
            auto result = itemDefinition.useDuration(itemInstance);

            if (result.return_count() >= 1) return result.get<sol::optional<int>>().value_or(1);
            return 1;
        } else {
            return Base::getUseDuration(itemInstance);
        }
    };

    UseAnim getUseAnimation(shared_ptr<ItemInstance> itemInstance) override {
        if (itemDefinition.getUseAnimation.valid()) {
            auto result = itemDefinition.getUseAnimation(itemInstance);

            if (result.return_count() >= 1) return result.get<sol::optional<UseAnim>>().value_or(UseAnim_none);
            return UseAnim_none;
        } else {
            return Base::getUseAnimation(itemInstance);
        }
    }

    bool hurtEnemy(shared_ptr<ItemInstance> itemInstance, shared_ptr<LivingEntity> mob, shared_ptr<LivingEntity> attacker) override {
        if (itemDefinition.hurtEnemy.valid()) {
            auto result = itemDefinition.hurtEnemy(itemInstance, mob, attacker);

            if (result.return_count() >= 1) return result.get<sol::optional<bool>>().value_or(true);;
            return true;
        } else {
            return Base::hurtEnemy(itemInstance, mob, attacker);
        }
    };

    protected:
    ItemDefinition itemDefinition;

};

using ModItem = ModItemTemplate<Item>;