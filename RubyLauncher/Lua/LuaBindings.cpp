#include "LuaBindings.h"
#include "Common/ModPaths.h"
#include "Loader.h"

#include <map>

#include "ServerPlayerGameMode.h"
#include "ServerPlayer.h"
#include "PlayerList.h"
#include "MinecraftServer.h"
#include "LevelSettings.h"
#include "CommandDispatcher.h"
#include "Level.h"
#include "PlayerConnection.h"
#include "Tile.h"

/* Server Includes */

#include "PlayerAbilitiesPacket.h"
#include "ServerLevel.h"
#include "Item.h"
#include "MobEffect.h"
#include "MobEffectInstance.h"

#include "Registry/Item/ItemRegistry.h"
#include "Registry/Item/ItemFactory.h"
#include "Registry/Block/BlockRegistry.h"
#include "Registry/Block/SaplingRegistry.h"
#include "Registry/Recipe/RecipeRegistry.h"
#include "Registry/WorldGen/OreFeatureRegistry.h"
#include "Registry/WorldGen/TreeFeatureRegistry.h"
#include "Registry/Item/TierRegistry.h"
#include "Registry/Item/ArmorMaterialRegistry.h"
#include "Registry/IDs.h"

#include "Server/Events/Item/ItemCompleteUseEvent.h"
#include "Server/Events/Item/ItemInteractEvent.h"
#include "Server/Events/Item/ItemInteractEntityEvent.h"
#include "Server/Events/Player/PlayerBlockBreakEvent.h"
#include "Server/Events/Player/PlayerBlockPlaceEvent.h"
#include "Server/Events/Player/PlayerConnectionEvent.h"
#include "Server/Events/Player/PlayerJoinEvent.h"
#include "Server/Events/Player/PlayerFlightStartedEvent.h"
#include "Server/Events/Player/PlayerFlightEndedEvent.h"

#include "Common/RubyUtils.h"
#include "LuaStructs.h"

namespace
{
	void readRecipeOptions(sol::object options, int &count, int &aux, RecipeRegistry::Group &group)
	{
		count = 1;
		aux = -1;
		group = Recipy::eGroupType_Decoration;

		if (!options.valid() || !options.is<sol::table>()) return;

		sol::table table = options.as<sol::table>();

		sol::optional<int> countValue = table["count"];
		if (countValue && countValue.value() > 0) count = countValue.value();

		sol::optional<int> auxValue = table["aux"];
		if (auxValue && auxValue.value() >= 0) aux = auxValue.value();

		sol::optional<RecipeRegistry::Group> enumGroup = table["group"];
		if (enumGroup)
		{
			const int value = static_cast<int>(enumGroup.value());
			if (value >= Recipy::eGroupType_First && value < Recipy::eGroupType_Max)
			{
				group = static_cast<RecipeRegistry::Group>(value);
			}
			else
			{
				Loader::_debugPrint("recipe: unknown crafting group " + std::to_string(value) + ", using Decoration");
			}
			return;
		}

		sol::optional<int> groupValue = table["group"];
		if (groupValue)
		{
			const int value = groupValue.value();
			if (value >= Recipy::eGroupType_First && value < Recipy::eGroupType_Max)
			{
				group = static_cast<RecipeRegistry::Group>(value);
			}
			else
			{
				Loader::_debugPrint("recipe: unknown crafting group " + std::to_string(value) + ", using Decoration");
			}
		}
	}

	bool luaStringArrayToVector(sol::object value, std::vector<std::string> &out, const char *context)
	{
		if (!value.is<sol::table>())
		{
			Loader::_debugPrint(std::string(context) + ": expected a table of strings");
			return false;
		}
		lua_State *L = value.lua_state();
		value.push();
		const int tableIndex = lua_gettop(L);
		const lua_Unsigned n = lua_rawlen(L, tableIndex);
		out.reserve(static_cast<size_t>(n));
		for (lua_Unsigned i = 1; i <= n; ++i)
		{
			lua_rawgeti(L, tableIndex, static_cast<lua_Integer>(i));
			if (lua_type(L, -1) != LUA_TSTRING)
			{
				lua_pop(L, 1);
				lua_pop(L, 1);
				Loader::_debugPrint(std::string(context) + ": entry " + std::to_string(i) + " must be a string");
				return false;
			}
			out.emplace_back(lua_tostring(L, -1));
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
		return true;
	}

	bool luaTableToStringMap(sol::object value, std::map<std::string, std::string> &out, const char *context)
	{
		if (!value.is<sol::table>())
		{
			Loader::_debugPrint(std::string(context) + ": expected a key table");
			return false;
		}
		lua_State *L = value.lua_state();
		value.push();
		lua_pushnil(L);
		while (lua_next(L, -2) != 0)
		{
			if (lua_type(L, -2) == LUA_TSTRING && lua_type(L, -1) == LUA_TSTRING)
			{
				out[lua_tostring(L, -2)] = lua_tostring(L, -1);
			}
			else
			{
				Loader::_debugPrint(std::string(context) + ": keys and values must be strings");
				lua_pop(L, 2);
				lua_pop(L, 1);
				return false;
			}
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
		return true;
	}
}

void LuaBindings::bindCommonFunctions(const std::vector<sol::state*> &luaStates) {
    for (sol::state* lua : luaStates) {
        lua->set_function("log", [](const std::string &message) {
            Loader::log(message);
        });

        lua->new_usertype<LuaVec3>("Vec3",
            sol::constructors<LuaVec3(double, double, double)>(),
            "x", &LuaVec3::x,
            "y", &LuaVec3::y,
            "z", &LuaVec3::z,
            "distanceTo", &LuaVec3::distanceTo,
            sol::meta_function::to_string, &LuaVec3::toString,
            sol::meta_function::addition, &LuaVec3::addition,
            sol::meta_function::subtraction, &LuaVec3::subtraction,
            sol::meta_function::multiplication, &LuaVec3::multiplication,
            sol::meta_function::division, &LuaVec3::division
        );

        lua->new_usertype<LuaBlock>("Block",
            "pos", &LuaBlock::pos,
            "id", sol::property(&LuaBlock::getID),
            "oid", sol::property(&LuaBlock::getOID)
        );

        (*lua)["Effect"] = lua->create_table_with(
            // Positive effects
            "Speed",            MobEffect::movementSpeed->id,
            "Haste",            MobEffect::digSpeed->id,
            "Strength",         MobEffect::damageBoost->id,
            "Healing",          MobEffect::heal->id,
            "JumpBoost",        MobEffect::jump->id,
            "Regeneration",     MobEffect::regeneration->id,
            "Resistance",       MobEffect::damageResistance->id,
            "FireResistance",   MobEffect::fireResistance->id,
            "WaterBreathing",   MobEffect::waterBreathing->id,
            "Invisibility",     MobEffect::invisibility->id,
            "NightVision",      MobEffect::nightVision->id,
            "HealthBoost",      MobEffect::healthBoost->id,
            "Absorption",       MobEffect::absorption->id,
            "Saturation",       MobEffect::saturation->id,

            // Negative effects
            "Slowness",         MobEffect::movementSlowdown->id,
            "MiningFatigue",    MobEffect::digSlowdown->id,
            "Harming",          MobEffect::harm->id,
            "Nausea",           MobEffect::confusion->id,
            "Blindness",        MobEffect::blindness->id,
            "Hunger",           MobEffect::hunger->id,
            "Weakness",         MobEffect::weakness->id,
            "Poison",           MobEffect::poison->id,
            "Wither",           MobEffect::wither->id
        );

        lua->set_function("registerOreFeature", [](sol::this_environment env, const std::string &oreId, const std::string &block, sol::optional<sol::object> options) -> bool
        {
            sol::environment& modEnv = env;
            std::string modId = modEnv["modId"];

            int size = 8, dimension = OreFeatureRegistry::DIMENSION_ALL, yMin = 0, yMax = Level::genDepth, count = 8;
            std::string target = "minecraft:stone";

            if (options.has_value() && options->valid() && options->is<sol::table>())
            {
                sol::table t = options->as<sol::table>();

                sol::optional<int> sizeValue = t["size"];
                if (sizeValue && sizeValue.value() > 0) size = sizeValue.value();

                sol::optional<int> dimensionValue = t["dimension"];
                if (dimensionValue) dimension = dimensionValue.value();

                sol::optional<int> yMinValue = t["yMin"];
                if (yMinValue && yMinValue.value() >= 0) yMin = yMinValue.value();

                sol::optional<int> yMaxValue = t["yMax"];
                if (yMaxValue && yMaxValue.value() > 0) yMax = yMaxValue.value();

                sol::optional<int> countValue = t["count"];
                if (countValue && countValue.value() > 0) count = countValue.value();

                sol::optional<std::string> targetValue = t["target"];
                if (targetValue && !targetValue.value().empty()) target = targetValue.value();
            }
            return OreFeatureRegistry::registerOre(modId, oreId, block, size, target, dimension, yMin, yMax, count);
        });

        lua->set_function("registerTreeFeature", [](sol::this_environment env, const std::string &treeId, const std::string &shape, sol::optional<sol::object> options) -> bool
        {
            sol::environment& modEnv = env;
            std::string modId = modEnv["modId"];

            std::string trunk, leaves, biome;
            int count = 1, height = 0;

            if (options.has_value() && options->valid() && options->is<sol::table>())
            {
                sol::table t = options->as<sol::table>();

                sol::optional<std::string> trunkValue = t["trunk"];
                if (trunkValue && !trunkValue.value().empty()) trunk = trunkValue.value();

                sol::optional<std::string> leavesValue = t["leaves"];
                if (leavesValue && !leavesValue.value().empty()) leaves = leavesValue.value();

                sol::optional<std::string> biomeValue = t["biome"];
                if (biomeValue && !biomeValue.value().empty()) biome = biomeValue.value();

                sol::optional<int> countValue = t["count"];
                if (countValue && countValue.value() > 0) count = countValue.value();

                sol::optional<int> heightValue = t["height"];
                if (heightValue && heightValue.value() > 0) height = heightValue.value();
            }
            return TreeFeatureRegistry::registerTree(modId, treeId, shape, trunk, leaves, biome, count, height);
        });
    }
}

void LuaBindings::bindServerEvents(sol::state& lua) {
    lua.set_function("registerEvent",
        [](const std::string& event_name, sol::function callback) {
            EventBus::Get().registerListener(event_name, sol::protected_function(std::move(callback)));
        }
    );

    lua.new_usertype<CancellableRubyEvent>("CancellableRubyEvent",
        "setCancelled", &CancellableRubyEvent::setCancelled,
        "isCancelled", &CancellableRubyEvent::isCancelled,
        sol::base_classes, sol::bases<RubyEvent>()
    );

    lua.new_usertype<RubyEvent>("RubyEvent",
        "name", &RubyEvent::eventName
    );

    lua.new_usertype<Inventory>("Inventory",
        "setItem", [](Inventory& inv, const int slot, int count, const std::string& identifier, sol::this_state state) {
            IDMapping::MappedItem mapping = IDMapping::get()->getID(identifier);
            if (mapping.id == 0 && identifier != "minecraft:air") {
                RubyUtils::LuaException(state, "Identifier " + identifier + " does not exist");
                return;
            }
            if (Item::items[mapping.id] == nullptr) {
                RubyUtils::LuaException(state, "Item id " + std::to_string(mapping.id) + " is null in Item::items");
                return;
            }
            if (count <= 0) {
                RubyUtils::LuaException(state, "Item count is zero");
                return;
            }

            inv.setItem(slot, std::make_shared<ItemInstance>(mapping.id, count, mapping.aux));
        },
        "getItem", [](Inventory& inv, const int slot) {
            return inv.getItem(slot);
        },
        "clear", [](Inventory& inv) {
            inv.clearInventory(-1, -1);
        }
    );

    lua.new_usertype<ItemInstance>("ItemInstance",
        "getHoverName", [](ItemInstance& item) {
            return item.getHoverName();
        },
        "isStackable", &ItemInstance::isStackable,
        "isEnchanted", &ItemInstance::isEnchanted,
        "isDamaged", &ItemInstance::isDamaged,
        "damageValue", &ItemInstance::getDamageValue,
        "maxDamage", &ItemInstance::getMaxDamage,
        "getAmount", &ItemInstance::count,
        "id", &ItemInstance::id
    );

    lua.new_usertype<ServerPlayerGameMode>("ServerPlayerGameMode",
        "isSurvival", sol::property(&ServerPlayerGameMode::isSurvival),
        "isCreative", sol::property(&ServerPlayerGameMode::isCreative)
    );

    lua["GameMode"] = sol::table(lua, sol::create);
    lua["GameMode"]["NOT_SET"]   = 0;
    lua["GameMode"]["SURVIVAL"]  = 0;
    lua["GameMode"]["CREATIVE"]  = 1;
    lua["GameMode"]["ADVENTURE"] = 2;

    lua.new_usertype<ServerPlayer>("ServerPlayer",
        "getHeldItem", &ServerPlayer::getCarriedItem,
        "setFoodLevel", [](ServerPlayer& player, int food) {
            player.getFoodData()->setFoodLevel(food);
        },
        "feed", [](ServerPlayer& player, int food, double modifier) {
            player.getFoodData()->eat(food,modifier);
        },
        "getHealth", [](ServerPlayer& player) {
            return player.getHealth();
        },
        "setHealth", [](ServerPlayer& player, int health) {
            player.setHealth(health);
        },
        "addEffect", [](ServerPlayer& player, int effectId, int durationTicks, int amplifier) {
            player.addEffect(new MobEffectInstance(effectId, durationTicks, amplifier));
        },
        "pos", sol::property([](ServerPlayer& player) { return LuaVec3(player.x, player.y, player.z); }),
        "teleport", [](ServerPlayer& player, sol::object target, sol::this_state state) {
            if (target.is<LuaVec3>()) {
                auto vec3 = target.as<LuaVec3>();
                player.teleportTo(vec3.x, vec3.y, vec3.z);
            }else {
                RubyUtils::LuaException(state, "Not a valid Vec3 object");
            }
        },
        "setCanFly", [](ServerPlayer& player, bool toggle) {
            // Toggles the PERMISSION to fly
            unsigned int val = toggle == true ? 1 : 0;
            player.setPlayerGamePrivilege(Player::EPlayerGamePrivileges::ePlayerGamePrivilege_CanFly,val);
            player.abilities.mayfly = toggle;
            if (player.connection == nullptr) {
                Loader::_debugPrint("setCanFly: " + RubyPaths::toNarrow(player.name) + " has no connection yet, abilities not sent");
                return;
            }
            std::shared_ptr<PlayerAbilitiesPacket> pkt =
                std::make_shared<PlayerAbilitiesPacket>(&player.abilities);
            player.connection->send(pkt);
        },
        "setFly", [](ServerPlayer& player, bool toggle) {
            // Toggles the flight state (must implicitly toggle the permission if starting flight aswell!)
            if (toggle) player.abilities.mayfly = toggle; // If we are disabling flight; do not remove toggle. That is the job of toggleFlightAllowed
            player.abilities.flying = toggle;
            if (player.connection == nullptr) {
                Loader::_debugPrint("setFly: " + RubyPaths::toNarrow(player.name) + " has no connection yet, abilities not sent");
                return;
            }
            std::shared_ptr<PlayerAbilitiesPacket> pkt =
                std::make_shared<PlayerAbilitiesPacket>(&player.abilities);
            player.connection->send(pkt);
        },
        "canFly", [](ServerPlayer& player) {
            return player.abilities.mayfly;
        },
        "getViewDistance", [](ServerPlayer& player) {
            return player.getViewDistance();
        },
        "displayName", &ServerPlayer::m_displayName,
        "experience",  &ServerPlayer::experienceProgress,
        "flySpeed", sol::property([](ServerPlayer& p) { return p.abilities.getFlyingSpeed(); }),
        "level", &ServerPlayer::experienceLevel,
        "name", &ServerPlayer::name,
        "walkSpeed", sol::property([](ServerPlayer& p) { return p.abilities.getWalkingSpeed(); }),
        "giveExp", [](ServerPlayer& player, float amount) {
            player.experienceProgress += amount;
        },
        "giveExpLevels", [](ServerPlayer& player, int amount) {
            player.experienceLevel += amount;
        },
        "setExp", [](ServerPlayer& player, float amount) {
            player.experienceProgress = amount;
        },
        "setLevel", [](ServerPlayer& player, int amount) {
            player.experienceLevel = amount;
        },
        "gamemode", &ServerPlayer::gameMode,
        "inventory", &ServerPlayer::inventory,
        "setGameMode", [](ServerPlayer& player, int gameTypeId) {
            if (GameType* type = GameType::byId(gameTypeId)) player.setGameMode(type);
        },
        "sendMessage", [](ServerPlayer& p, const std::string& message) {
            std::wstring wmessage(message.begin(), message.end());
            p.sendMessage(wmessage);
        },
        "destroyBlock", [](ServerPlayer& p, sol::object target, sol::this_state state) {
            if (target.is<LuaVec3>()) {
                auto vec3 = target.as<LuaVec3>();
                if (p.level->getTile(vec3.x,vec3.y,vec3.z) == 0) return;
                p.gameMode->destroyBlock(vec3.x, vec3.y, vec3.z);
            }else {
                RubyUtils::LuaException(state, "Not a valid Vec3 object");
            }
        }
    );

    lua.new_usertype<PlayerBlockBreakEvent>("PlayerBlockBreakEvent",
        "player", &PlayerBlockBreakEvent::player,
        "block", &PlayerBlockBreakEvent::block,
        sol::base_classes, sol::bases<CancellableRubyEvent, RubyEvent>()
    );

    lua.new_usertype<PlayerBlockPlaceEvent>("PlayerBlockPlaceEvent",
        "player", &PlayerBlockPlaceEvent::player,
        "block", &PlayerBlockPlaceEvent::block,
        sol::base_classes, sol::bases<CancellableRubyEvent, RubyEvent>()
    );

    lua.new_usertype<ItemCompleteUseEvent>("ItemConsumeEvent",
        "item", &ItemCompleteUseEvent::item,
        "player", &ItemCompleteUseEvent::player,
        "itemId", &ItemCompleteUseEvent::itemId,
        sol::base_classes, sol::bases<RubyEvent>()
    );

    lua.new_usertype<ItemInteractEvent>("ItemInteractEvent",
        "item", &ItemInteractEvent::item,
        //"level", &ItemInteractEvent::level, // We need to implement a usertype for level
        "player", &ItemInteractEvent::player,
        sol::base_classes, sol::bases<RubyEvent>()
    );

    lua.new_usertype<ItemInteractEntityEvent>("ItemInteractEntityEvent",
        "item", &ItemInteractEntityEvent::item,
        sol::base_classes, sol::bases<RubyEvent>()
    );

    lua.new_usertype<PlayerConnectionEvent>("PlayerConnectionEvent",
        "player", &PlayerConnectionEvent::player,
        sol::base_classes, sol::bases<RubyEvent>()
    );

    lua.new_usertype<PlayerJoinEvent>("PlayerJoinEvent",
        "player", &PlayerJoinEvent::player,
        sol::base_classes, sol::bases<RubyEvent>()
    );

    lua.new_usertype<PlayerFlightStartedEvent>("PlayerFlightStartedEvent",
        "player", &PlayerFlightStartedEvent::player,
        sol::base_classes, sol::bases<RubyEvent>()
    );

    lua.new_usertype<PlayerFlightEndedEvent>("PlayerFlightEndedEvent",
        "player", &PlayerFlightEndedEvent::player,
        sol::base_classes, sol::bases<RubyEvent>()
    );
}

void LuaBindings::bindServerFunctions(sol::state& lua, MinecraftServer* server) {
    lua["server"] = server;

    lua.new_usertype<MinecraftServer>("MinecraftServer",
        "getCommandDispatcher", &MinecraftServer::getCommandDispatcher,
        "getPlayers", [](MinecraftServer* server) -> std::vector<std::shared_ptr<ServerPlayer>>& {
            return server->getPlayers()->players;
        }
    );

    lua.new_usertype<CommandSender>("CommandSender", "hasPermission", &CommandSender::hasPermission
    );

    lua.new_usertype<Player>("Player",
        "getPos", &Player::getPos,
        "setPos", sol::resolve<void(double, double, double)>(&Player::setPos),
        "abilities", &Player::abilities,
        "changeDimension", &Player::changeDimension,
        "sendMessage", &Player::sendMessage,
        sol::base_classes, sol::bases<CommandSender>()
    );

    lua.new_usertype<CommandDispatcher>("CommandDispatcher", "performCommand", &CommandDispatcher::performCommand
    );

    lua.new_enum<EGameCommand>("EGameCommand", {
            {"COUNT", eGameCommand_COUNT},
            {"EnchantItem", EGameCommand::eGameCommand_EnchantItem},
            {"Experience", EGameCommand::eGameCommand_Experience},
            {"GameMode", EGameCommand::eGameCommand_GameMode},
            {"Give", EGameCommand::eGameCommand_Give},
            {"Kill", EGameCommand::eGameCommand_Kill},
            {"Teleport", EGameCommand::eGameCommand_Teleport},
            {"Time", EGameCommand::eGameCommand_Time},
            {"ToggleDownfall", EGameCommand::eGameCommand_ToggleDownfall}
        }
    );

    lua.new_usertype<ServerLevel>("ServerLevel",
        "getBlocksAndData", &ServerLevel::getBlocksAndData,
        "setBlocksAndData", &ServerLevel::setBlocksAndData,
        "getServer", &ServerLevel::getServer,
        "isClientside", &ServerLevel::isClientSide,
        "getTile", &ServerLevel::getTile,
        "hasChunkAt", &ServerLevel::hasChunkAt
    );
}

void LuaBindings::bindClientFunctions(sol::state& lua) {
    lua.new_enum<EBaseItem>("EBaseItem", {
        {"Default", EBaseItem::Default},
        {"Food", EBaseItem::Food},
        {"Hoe", EBaseItem::Hoe},
        {"Weapon", EBaseItem::Weapon},
        {"Pickaxe", EBaseItem::Pickaxe},
        {"Hatchet", EBaseItem::Hatchet},
        {"Shovel", EBaseItem::Shovel},
        {"Helmet", EBaseItem::Helmet},
        {"Chestplate", EBaseItem::Chestplate},
        {"Leggings", EBaseItem::Leggings},
        {"Boots", EBaseItem::Boots},
        {"Ingot", EBaseItem::Ingot},
        {"Food_Fruit", EBaseItem::Food_Fruit},
        {"Food_Bread", EBaseItem::Food_Bread}
    });

    lua.new_enum<EArmorMaterial>("EArmorMaterial", {
        {"Cloth", ArmorMaterial_Cloth},
        {"Chain", ArmorMaterial_Chain},
        {"Iron", ArmorMaterial_Iron},
        {"Gold", ArmorMaterial_Gold},
        {"Diamond", ArmorMaterial_Diamond},
        {"Nethanium", ArmorMaterial_Nethanium},
        {"Endorium", ArmorMaterial_Endorium},
        {"Zanite", ArmorMaterial_Zanite},
        {"Gravitite", ArmorMaterial_Gravitite}
    });

    lua.new_enum<EItemTier>("EItemTier", {
        {"Wood", ItemTier_Wood},
        {"Stone", ItemTier_Stone},
        {"Iron", ItemTier_Iron},
        {"Diamond", ItemTier_Diamond},
        {"Gold", ItemTier_Gold},
        {"Nethanium", ItemTier_Nethanium},
        {"Endorium", ItemTier_Endorium},
        {"Zanite", ItemTier_Zanite},
        {"Gravitite", ItemTier_Gravitite},
        {"Vampire", ItemTier_Vampire},
        {"Valkyrie", ItemTier_Valkyrie},
        {"Aphalaf", ItemTier_Aphalaf},
        {"Nusa", ItemTier_Nusa}
    });

    lua.new_enum<EItemMaterial>("EItemMaterial", {
        {"Undefined", ItemMaterial_Undefined},
        {"Wood",      ItemMaterial_Wood},
        {"Stone",     ItemMaterial_Stone},
        {"Iron",      ItemMaterial_Iron},
        {"Gold",      ItemMaterial_Gold},
        {"Diamond",   ItemMaterial_Diamond},
        {"Cloth",     ItemMaterial_Cloth},
        {"Chain",     ItemMaterial_Chain},
        {"Lapis",     ItemMaterial_Lapis},
        {"Redstone",  ItemMaterial_Redstone},
        {"Coal",      ItemMaterial_Coal},
        {"Emerald",   ItemMaterial_Emerald},
        {"Quartz",    ItemMaterial_Quartz},
        {"Glass",     ItemMaterial_Glass},
        {"Sand",      ItemMaterial_Sand},
        {"Brick",     ItemMaterial_Brick},
        {"Clay",      ItemMaterial_Clay},
        {"Snow",      ItemMaterial_Snow},
        {"Ice",       ItemMaterial_Ice},
        {"Glowstone", ItemMaterial_Glowstone},
        {"Stick",     ItemMaterial_Stick},
        {"Paper",     ItemMaterial_Paper},
        {"Apple",     ItemMaterial_Apple},
        {"Nethanium", ItemMaterial_Nethanium},
        {"Endorium",  ItemMaterial_Endorium},
        {"Zanite",    ItemMaterial_Zanite},
        {"Gravitite", ItemMaterial_Gravitite},
        {"Aphal",     ItemMaterial_Aphal},
        {"Nusa",      ItemMaterial_Nusa}
    });

    lua.new_usertype<ItemDefinition>("ItemDefinition",
        sol::constructors<ItemDefinition(sol::table)>(),
        "type", &ItemDefinition::type,
        "nutrition", &ItemDefinition::nutrition,
        "saturationMod", &ItemDefinition::saturationMod,
        "isMeat", &ItemDefinition::isMeat,
        "canAlwaysEat", &ItemDefinition::canAlwaysEat,
        "tier", &ItemDefinition::tier,
        "armorMaterial", &ItemDefinition::armorMaterial,
        "armorSet", &ItemDefinition::armorSet,
        "material", &ItemDefinition::material
    );

    lua.new_usertype<Item::Tier>("Tier",
        sol::constructors<Item::Tier(int,int,double,int,int)>(),
        "getUses", &Item::Tier::getUses,
        "getLevel", &Item::Tier::getLevel
    );

    lua.set_function("registerItem", [](sol::this_environment env,const std::string& id, const std::string& name, const std::string& texturePath, const ItemDefinition& def) {
        sol::environment& modEnv = env;
        std::string envPath = modEnv["pathName"];
        std::string modId = modEnv["modId"];
        std::wstring path = std::wstring(envPath.begin(), envPath.end());
        return ItemRegistry::registerItem(path, id, name, modId, def, texturePath);
    });

    lua.new_enum<EBlockTool>("EBlockTool", {
        {"None", BlockTool_None},
        {"Pickaxe", BlockTool_Pickaxe},
        {"Hatchet", BlockTool_Hatchet},
        {"Shovel", BlockTool_Shovel},
        {"Hoe", BlockTool_Hoe}
    });

    lua.new_usertype<BlockDefinition>("BlockDefinition",
        sol::constructors<BlockDefinition(sol::table)>(),
        "hardness", &BlockDefinition::hardness,
        "resistance", &BlockDefinition::resistance,
        "tool", &BlockDefinition::tool
    );

    lua.set_function("registerBlock", [](sol::this_environment env,const std::string& id, const std::string& name, const std::string& texturePath, sol::optional<sol::object> definition, sol::this_state state) {
        sol::environment& modEnv = env;
        std::string envPath = modEnv["pathName"];
        std::string modId = modEnv["modId"];
        std::wstring path = std::wstring(envPath.begin(), envPath.end());

        BlockDefinition def;
        if (definition && definition->valid()) {
            if (definition->is<BlockDefinition>()) {
                def = definition->as<BlockDefinition>();
            } else if (definition->is<sol::table>()) {
                def = BlockDefinition(definition->as<sol::table>());
            } else if (definition->get_type() != sol::type::nil) {
                RubyUtils::LuaException(state, "registerBlock: the block definition must be a table or a BlockDefinition");
                return -1;
            }
        }

        int registeredBlock = BlockRegistry::registerBlock(path, id, name, modId, texturePath, def);
        if (registeredBlock == -1) {
            RubyUtils::LuaException(state, "The block registry limit has been reached, can't register more than " + std::to_string(BlockRegistry::maxBlockCount()) + " custom blocks");
            return -1;
        }
        return registeredBlock;
    });

    lua.set_function("registerSapling", [](sol::this_environment env, const std::string &saplingId, const std::string &name, const std::string &texturePath, sol::optional<sol::object> options, sol::this_state state) -> int {
        sol::environment& modEnv = env;
        std::string modId = modEnv["modId"];
        std::string envPath = modEnv["pathName"];
        std::wstring path = std::wstring(envPath.begin(), envPath.end());

        std::string shape = "oak", trunk, leaves;
        int height = 0;
        if (options.has_value() && options->valid() && options->is<sol::table>())
        {
            sol::table t = options->as<sol::table>();

            sol::optional<std::string> shapeValue = t["shape"];
            if (shapeValue && !shapeValue.value().empty()) shape = shapeValue.value();

            sol::optional<std::string> trunkValue = t["trunk"];
            if (trunkValue && !trunkValue.value().empty()) trunk = trunkValue.value();

            sol::optional<std::string> leavesValue = t["leaves"];
            if (leavesValue && !leavesValue.value().empty()) leaves = leavesValue.value();

            sol::optional<int> heightValue = t["height"];
            if (heightValue && heightValue.value() > 0) height = heightValue.value();
        }

        int registered = SaplingRegistry::registerSapling(modId, saplingId, name, path, texturePath, shape, trunk, leaves, height);
        if (registered == -1) {
            RubyUtils::LuaException(state, "registerSapling: failed to register '" + saplingId + "' (bad shape, unknown trunk/leaves block, or the block registry limit was reached)");
            return -1;
        }
        return registered;
    });

    lua.new_enum<RecipeRegistry::Group>("ERecipeGroup", {
        {"Structure",  Recipy::eGroupType_Structure},
        {"Tool",       Recipy::eGroupType_Tool},
        {"Food",       Recipy::eGroupType_Food},
        {"Armour",     Recipy::eGroupType_Armour},
        {"Mechanism",  Recipy::eGroupType_Mechanism},
        {"Transport",  Recipy::eGroupType_Transport},
        {"Decoration", Recipy::eGroupType_Decoration}
    });
	lua.set_function("registerShapedRecipe", [](sol::this_environment env, const std::string &recipeId, sol::object pattern, sol::object key, const std::string &result, sol::optional<sol::object> options) -> bool
	{
		std::vector<std::string> patternList;
		std::map<std::string, std::string> keyMap;
		if (!luaStringArrayToVector(pattern, patternList, "registerShapedRecipe: pattern")) return false;
		if (patternList.empty())
		{
			return false;
		}
		if (!luaTableToStringMap(key, keyMap, "registerShapedRecipe: key")) return false;

		sol::environment& modEnv = env;
		std::string modId = modEnv["modId"];

		int count = 1;
		int aux = -1;
		RecipeRegistry::Group group = Recipy::eGroupType_Decoration;
		if (options.has_value()) readRecipeOptions(options.value(), count, aux, group);

		return RecipeRegistry::registerShaped(modId, recipeId, patternList, keyMap, result, count, aux, group);
	});
	lua.set_function("registerShapelessRecipe", [](sol::this_environment env, const std::string &recipeId, sol::object ingredients, const std::string &result, sol::optional<sol::object> options) -> bool
	{
		std::vector<std::string> ingredientList;
		if (!luaStringArrayToVector(ingredients, ingredientList, "registerShapelessRecipe: ingredients")) return false;
		if (ingredientList.empty())
		{
			return false;
		}

		sol::environment& modEnv = env;
		std::string modId = modEnv["modId"];

		int count = 1;
		int aux = -1;
		RecipeRegistry::Group group = Recipy::eGroupType_Decoration;
		if (options.has_value()) readRecipeOptions(options.value(), count, aux, group);

		return RecipeRegistry::registerShapeless(modId, recipeId, ingredientList, result, count, aux, group);
	});

    lua.set_function("registerSmeltingRecipe", [](sol::this_environment env, const std::string &recipeId, const std::string &input, const std::string &result, sol::optional<sol::object> options) -> bool
    {
        sol::environment& modEnv = env;
        std::string modId = modEnv["modId"];

        int count = 1;
        float xp = 0.0f;
        if (options.has_value() && options->valid() && options->is<sol::table>())
        {
            sol::table table = options->as<sol::table>();

            sol::optional<int> countValue = table["count"];
            if (countValue && countValue.value() > 0) count = countValue.value();

            sol::optional<float> xpValue = table["xp"];
            if (xpValue && xpValue.value() >= 0.0f) xp = xpValue.value();
        }
        return RecipeRegistry::registerSmelting(modId, recipeId, input, result, count, xp);
    });

    lua.set_function("registerItemTier", [](sol::this_environment env, const std::string &tierId, sol::optional<sol::object> options) -> int
    {
        sol::environment& modEnv = env;
        std::string modId = modEnv["modId"];
        int level = 1, uses = 250, ench = 14;
        float speed = 6.0f, damage = 2.0f;
        if (options.has_value() && options->valid() && options->is<sol::table>())
        {
            sol::table t = options->as<sol::table>();
            sol::optional<int>   lv = t["level"];    if (lv)  level  = lv.value();
            sol::optional<int>   us = t["uses"];     if (us)  uses   = us.value();
            sol::optional<float> sp = t["speed"];    if (sp)  speed  = sp.value();
            sol::optional<float> dm = t["damage"];   if (dm)  damage = dm.value();
            sol::optional<int>   en = t["ench"];     if (en)  ench   = en.value();
        }
        return TierRegistry::registerTier(modId, tierId, level, uses, speed, damage, ench);
    });

    lua.set_function("registerArmorMaterial", [](sol::this_environment env, const std::string &materialId, sol::optional<sol::object> options) -> int
    {
        sol::environment& modEnv = env;
        std::string modId = modEnv["modId"];
        int durabilityMultiplier = 15, head = 2, torso = 6, legs = 5, feet = 2, ench = 10;
        if (options.has_value() && options->valid() && options->is<sol::table>())
        {
            sol::table t = options->as<sol::table>();
            sol::optional<int> dm = t["durabilityMultiplier"]; if (dm) durabilityMultiplier = dm.value();
            sol::optional<int> hp = t["head"];                 if (hp) head = hp.value();
            sol::optional<int> tp = t["torso"];                if (tp) torso = tp.value();
            sol::optional<int> lp = t["legs"];                 if (lp) legs = lp.value();
            sol::optional<int> fp = t["feet"];                 if (fp) feet = fp.value();
            sol::optional<int> en = t["ench"];                 if (en) ench = en.value();
        }
        return ArmorMaterialRegistry::registerMaterial(modId, materialId, durabilityMultiplier, head, torso, legs, feet, ench);
    });
}