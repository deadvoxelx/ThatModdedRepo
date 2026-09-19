#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <type_traits>

#include "RubyEvent.h"
#include "sol/sol.hpp"

inline std::string safeLuaErrorText(const sol::protected_function_result& result)
{
	lua_State* L = result.lua_state();
	if (L == nullptr) return "(no lua state)";

	const int idx = result.stack_index();
	if (idx == 0) return "(no error value)";
	if (result.return_count() <= 0) return "(no error value)";

	const char* text = lua_tolstring(L, idx, nullptr);
	if (text != nullptr) return std::string(text);

	return "(non-string error of type " + std::string(lua_typename(L, lua_type(L, idx))) + ")";
}

class EventBus {
public:
    using Listener = sol::protected_function;

    static EventBus& Get() {
        static EventBus instance;
        return instance;
    }

    void registerListener(const std::string& eventName, Listener listener) {
        listeners_[eventName].push_back(std::move(listener));
    }

    void clearListeners() {
        listeners_.clear();
    }

    template<typename E>
    bool fire(E& event) {
        auto it = listeners_.find(event.eventName);
        if (it == listeners_.end()) return false;

        bool wasCanceled = false;
        for (auto& fn : it->second) {
            if (!fn.valid()) continue;
            auto result = fn(&event);

            if (!result.valid()) {
                fprintf(stderr, "[Lua] Error in '%s' listener: %s\n", event.eventName.c_str(), safeLuaErrorText(result).c_str());
                continue;
            }

            if (result.valid() && result.get_type() == sol::type::boolean && result.template get<bool>()) {
                if constexpr (std::is_base_of_v<CancellableRubyEvent, E>) {
                    event.setCancelled(true);
                }
            }
            if constexpr (std::is_base_of_v<CancellableRubyEvent, E>) {
                if (event.isCancelled()) wasCanceled = true;
            }
        }
        return wasCanceled;
    }

private:
    EventBus() = default;
    std::unordered_map<std::string, std::vector<Listener>> listeners_;
};
