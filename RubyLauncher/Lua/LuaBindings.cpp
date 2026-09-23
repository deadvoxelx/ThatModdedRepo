#include "LuaBindings.h"
#include "Common/ModNetBus.h"
#include "Common/ModPaths.h"
#include "Common/ModStore.h"
#include "Loader.h"

#include <map>
#include <vector>

#include "ServerPlayerGameMode.h"
#include "ServerPlayer.h"
#include "LivingEntity.h"
#include "PlayerList.h"
#include "MinecraftServer.h"
#include "LevelSettings.h"
#include "CommandDispatcher.h"
#include "Level.h"
#include "PlayerConnection.h"
#include "Tile.h"
#include "Minecraft.h"
#include "ClientConnection.h"
#include "CustomPayloadPacket.h"

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

#include "Common/EventSystem/ClientTickEvent.h"
#include "Common/EventSystem/EventBus.h"
#include "Common/EventSystem/ServerTickEvent.h"
#include "../../Minecraft.Client/ServerPlayer.h"
#include "../../Minecraft.World/ItemInstance.h"
#include "../../Minecraft.World/Level.h"
#include "../../Minecraft.World/MobEffect.h"
#include "../../Minecraft.World/MobEffectInstance.h"
#include "../Registry/IDs.h"
#include "../Server/Events/Item/ItemTickEvent.h"
#include "Common/RubyUtils.h"
#include "LuaStructs.h"
#include "../Registry/IDs.h"

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
Loader *rubyLoader()
	{
		return Loader::getInstance();
	}

	Scheduler &schedulerFor(lua_State *L)
	{
		static Scheduler fallbackScheduler;
		Loader *loader = rubyLoader();
		if (loader == nullptr) return fallbackScheduler;
		if (L == loader->luaClient.lua_state()) return loader->m_clientScheduler;
		return loader->m_serverScheduler;
	}

	sol::object callModByName(sol::this_state state, const std::string &modId, const std::string &fnName, sol::variadic_args args)
	{
		Loader *loader = rubyLoader();
		if (loader == nullptr) return sol::lua_nil_t{};
		RubyMod *mod = loader->findMod(modId);
		if (mod == nullptr)
		{
			Loader::_debugPrint("callMod: unknown mod '" + modId + "'");
			return sol::lua_nil_t{};
		}

		lua_State *L = state.lua_state();
		sol::protected_function fn;
		try
		{
			fn = (L == loader->luaServer.lua_state()) ? mod->getServerFunction(fnName) : mod->getClientFunction(fnName);
		}
		catch (const sol::error &e)
		{
			Loader::_debugPrint("callMod: error resolving '" + modId + "." + fnName + "': " + e.what());
			return sol::lua_nil_t{};
		}

		if (!fn.valid())
		{
			Loader::_debugPrint("callMod: mod '" + modId + "' has no function '" + fnName + "'");
			return sol::lua_nil_t{};
		}

		std::vector<sol::object> argv;
		for (sol::object arg : args) argv.push_back(arg);
		auto result = fn(sol::as_args(argv));
		if (!result.valid())
		{
			Loader::_debugPrint("callMod: error calling '" + modId + "." + fnName + "': " + safeLuaErrorText(result));
			return sol::lua_nil_t{};
		}
		if (result.return_count() == 0) return sol::lua_nil_t{};
		return result.get<sol::object>(0);
	}

	sol::table makeModHandle(lua_State *L, const std::string &modId)
	{
		sol::table handle(L, sol::create);
		handle["modId"] = modId;
		handle["call"] = [modId](sol::this_state state, const std::string &fnName, sol::variadic_args args) -> sol::object {
			Loader *loader = rubyLoader();
			if (loader == nullptr) return sol::lua_nil_t{};
			return callModByName(state, modId, fnName, args);
		};
		return handle;
	}

	std::vector<std::string> variadicArgsToStrings(sol::variadic_args args)
	{
		std::vector<std::string> result;
		for (sol::object arg : args)
		{
			if (arg.is<std::string>()) result.push_back(arg.as<std::string>());
		}
		return result;
	}

	bool rubyNetSendServer(Loader *loader, sol::variadic_args args)
	{
		std::vector<sol::object> argv;
		for (sol::object arg : args) argv.push_back(arg);
		if (argv.size() < 2) return false;
		ServerPlayer *player = nullptr;
		size_t offset = 0;
		if (argv[0].is<ServerPlayer>())
		{
			player = argv[0].as<ServerPlayer *>();
			offset = 1;
		}
		if (argv.size() < offset + 2) return false;
		sol::object channelObj = argv[offset];
		sol::object dataObj = argv[offset + 1];
		if (!channelObj.is<std::string>() || !dataObj.is<std::string>()) return false;
		std::wstring identifier = L"ruby:" + RubyPaths::toWide(channelObj.as<std::string>());
		byteArray payload = stringToByteArray(dataObj.as<std::string>());
		if (player != nullptr)
		{
			if (player->connection != nullptr)
			{
				player->connection->send(std::make_shared<CustomPayloadPacket>(identifier, payload));
			}
			return true;
		}

		if (loader->m_server == nullptr || loader->m_server->getPlayers() == nullptr) return false;
		for (auto &p : loader->m_server->getPlayers()->players)
		{
			if (p != nullptr && p->connection != nullptr)
			{
				p->connection->send(std::make_shared<CustomPayloadPacket>(identifier, payload));
			}
		}
		return true;
	}

	bool rubyNetSendClient(sol::variadic_args args)
	{
		std::vector<sol::object> argv;
		for (sol::object arg : args) argv.push_back(arg);
		if (argv.size() < 2) return false;
		if (!argv[0].is<std::string>() || !argv[1].is<std::string>()) return false;
		Minecraft *mc = Minecraft::GetInstance();
		if (mc == nullptr) return false;
		ClientConnection *conn = mc->getConnection(0);
		if (conn == nullptr) return false;
		std::wstring identifier = L"ruby:" + RubyPaths::toWide(argv[0].as<std::string>());
		conn->send(std::make_shared<CustomPayloadPacket>(identifier, stringToByteArray(argv[1].as<std::string>())));
		return true;
	}

	bool rubyNetSend(lua_State *L, sol::variadic_args args)
	{
		Loader *loader = rubyLoader();
		if (loader == nullptr) return false;
		if (L == loader->luaServer.lua_state()) return rubyNetSendServer(loader, args);
		return rubyNetSendClient(args);
	}
}

void LuaBindings::bindCommonFunctions(const std::vector<sol::state*> &luaStates) {
    for (sol::state* lua : luaStates) {
        sol::table logTable = lua->create_table();
        logTable["info"] = [](const std::string &message) {
            Loader::log(message);
        };
        logTable["warn"] = [](const std::string &message) {
            Loader::log("Warning: " + message);
        };
        logTable["error"] = [](const std::string &message) {
            Loader::log("Error: " + message);
        };
        sol::table logMeta = lua->create_table();
        logMeta[sol::meta_function::call] = [](sol::table, const std::string &message) {
            Loader::log(message);
        };
        logTable[sol::metatable_key] = logMeta;
        (*lua)["log"] = logTable;
        lua->set_function("schedule", [](double delayTicks, sol::protected_function fn, sol::this_state state) {
            return schedulerFor(state.lua_state()).schedule(delayTicks, std::move(fn));
        });

        lua->set_function("scheduleEvery", [](double intervalTicks, sol::protected_function fn, sol::this_state state) {
            return schedulerFor(state.lua_state()).scheduleEvery(intervalTicks, std::move(fn));
        });

        lua->set_function("cancelTask", [](int id, sol::this_state state) {
            schedulerFor(state.lua_state()).cancel(id);
            return true;
        });

        lua->set_function("registerEvent",
            [](const std::string& event_name, sol::function callback, sol::this_state state) {
                Loader *loader = rubyLoader();
                bool serverSide = loader != nullptr && state.lua_state() == loader->luaServer.lua_state();
                EventBus::Get().registerListener(event_name, sol::protected_function(std::move(callback)), serverSide);
            }
        );

        lua->new_usertype<CancellableRubyEvent>("CancellableRubyEvent",
            "setCancelled", &CancellableRubyEvent::setCancelled,
            "isCancelled", &CancellableRubyEvent::isCancelled,
            sol::base_classes, sol::bases<RubyEvent>()
        );

        lua->new_usertype<RubyEvent>("RubyEvent",
            "name", &RubyEvent::eventName
        );

        Loader *loader = rubyLoader();
        if (loader != nullptr && lua->lua_state() == loader->luaServer.lua_state())
        {
            lua->new_usertype<ServerTickEvent>("ServerTickEvent",
                "server", &ServerTickEvent::server,
                "tickCount", &ServerTickEvent::tickCount
            );
        }

        lua->new_usertype<ClientTickEvent>("ClientTickEvent",
            "tickCount", &ClientTickEvent::tickCount
        );

        if (loader != nullptr && lua->lua_state() == loader->luaServer.lua_state())
        {
            lua->new_usertype<ModNetEvent>("ModNetEvent",
                "player", &ModNetEvent::player,
                "channel", &ModNetEvent::channel,
                "data", &ModNetEvent::data
            );
        }
        else
        {
            lua->new_usertype<ModNetEvent>("ModNetEvent",
                "channel", &ModNetEvent::channel,
                "data", &ModNetEvent::data
            );
        }

        lua->set_function("callMod", [](sol::this_state state, const std::string &modId, const std::string &fnName, sol::variadic_args args) -> sol::object {
            return callModByName(state, modId, fnName, args);
        });

        sol::table modsTable = lua->create_table();
        sol::table modsMeta = lua->create_table();
        modsMeta[sol::meta_function::index] = [](sol::this_state state, sol::object key) -> sol::object {
            if (!key.is<std::string>()) return sol::lua_nil_t{};
            Loader *loader = rubyLoader();
            if (loader == nullptr || loader->findMod(key.as<std::string>()) == nullptr) return sol::lua_nil_t{};
            return makeModHandle(state.lua_state(), key.as<std::string>());
        };
        modsTable[sol::metatable_key] = modsMeta;
        (*lua)["mods"] = modsTable;
        sol::table netTable = lua->create_table();
        netTable["listen"] = [](const std::string &channel, sol::protected_function fn, sol::this_state state) -> bool {
            Loader *loader = rubyLoader();
            if (loader == nullptr || channel.empty() || !fn.valid()) return false;
            if (state.lua_state() == loader->luaClient.lua_state())
            {
                ModNetBus::Get().listenClient(channel, std::move(fn));
            }
            else
            {
                ModNetBus::Get().listenServer(channel, std::move(fn));
            }
            return true;
        };
        netTable["send"] = [](sol::this_state state, sol::variadic_args args) -> bool {
            return rubyNetSend(state.lua_state(), args);
        };
        (*lua)["net"] = netTable;
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
        },
        "isInAccessory", [](Inventory& inv, ItemInstance* item) {
            return inv.inAccessory(item);
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
        "setData", [](sol::this_environment env, ServerPlayer& player, const std::string &key, sol::object value, sol::this_state state) -> bool {
            sol::environment& modEnv = env;
            std::string modId = modEnv["modId"];
            if (modId.empty()) return false;
            ModStore::Get().setPlayerData(modId, RubyPaths::toNarrow(player.name), key, value, state);
            return true;
        },
        "getData", [](sol::this_environment env, ServerPlayer& player, const std::string &key, sol::this_state state) -> sol::object {
            sol::environment& modEnv = env;
            std::string modId = modEnv["modId"];
            if (modId.empty()) return sol::lua_nil_t{};
            return ModStore::Get().getPlayerData(modId, RubyPaths::toNarrow(player.name), key, state);
        },
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
        "hasEffect", [](ServerPlayer& player, int effectId) {
            for (MobEffectInstance* effect : *player.getActiveEffects()) {
                if (effect == nullptr) continue;
                if (effect->getId() == effectId) {
                    return true;
                }
            }
            return false;
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

        "destroyBlock", [](ServerPlayer& p, sol::object target, sol::this_state state) {
            if (target.is<LuaVec3>()) {
                auto vec3 = target.as<LuaVec3>();
                if (p.level->getTile(vec3.x,vec3.y,vec3.z) == 0) return;
                p.gameMode->destroyBlock(vec3.x, vec3.y, vec3.z);
            }else {
                RubyUtils::LuaException(state, "Not a valid Vec3 object");
            }
        },
        sol::base_classes, sol::bases<Player, CommandSender, LivingEntity>()
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
        "level", &ItemInteractEvent::level,
        "player", &ItemInteractEvent::player,
        sol::base_classes, sol::bases<RubyEvent>()
    );

    lua.new_usertype<ItemTickEvent>("ItemTickEvent",
        "item", &ItemTickEvent::item,
        "level", &ItemTickEvent::level,
        "player", &ItemTickEvent::player,
        "slot", &ItemTickEvent::slot,
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
    Loader *loader = rubyLoader();
    if (loader != nullptr)
    {
        loader->m_commandRegistry.init(server);
        lua.set_function("registerCommand", [loader](const std::string &name, int permissionLevel, sol::protected_function handler) -> int {
            return loader->m_commandRegistry.registerCommand(name, permissionLevel, std::move(handler));
        });

        lua.set_function("runCommand", [loader](sol::object sender, const std::string &name, sol::variadic_args args) -> bool {
            std::vector<std::string> argv = variadicArgsToStrings(args);
            if (argv.size() != args.size()) return false;
            ServerPlayer *player = sender.is<ServerPlayer>() ? sender.as<ServerPlayer *>() : nullptr;
            return loader->m_commandRegistry.performCommand(name, player, argv);
        });
    }

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
        "sendMessage", [](Player& p, const std::string& message) {
            std::wstring wmessage(message.begin(), message.end());
            p.sendMessage(wmessage);
        },
        sol::base_classes, sol::bases<CommandSender, LivingEntity>()
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

    lua.new_usertype<Level>("Level",
        "setTileAndData", &Level::setTileAndData,
        "getTile", &Level::getTile,
        "setData", &Level::setData,
        "getData", &Level::getData
    );

    lua.set_function("getIdFromString", [](sol::this_environment env, const std::string& id) {
        return IDMapping::get()->getID(id).id;
    });
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
        "material", &ItemDefinition::material,
        "fireImmune", &ItemDefinition::fireImmune,
        "blastImmune", &ItemDefinition::blastImmune
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
