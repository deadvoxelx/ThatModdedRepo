#include "RubyMod.h"

#include <string_view>
#include "sol/sol.hpp"

std::string_view RubyMod::getName() const {
    return metadata.at("name");
};

std::string_view RubyMod::getModID() const {
    return metadata.at("modId");
};

sol::protected_function RubyMod::getServerFunction(std::string n) {
    return getServerEnv()[n];
};

sol::protected_function RubyMod::getClientFunction(std::string n) {
    return getClientEnv()[n];
};

std::string_view RubyMod::getServerEntry() const {
    auto it = metadata.find("serverMain");
    return it == metadata.end() ? std::string_view{} : std::string_view{it->second};
};

std::string_view RubyMod::getClientEntry() const {
    auto it = metadata.find("clientMain");
    return it == metadata.end() ? std::string_view{} : std::string_view{it->second};
};

std::string_view RubyMod::getPathName() const {
    return pathName;
}

void RubyMod::setPathName(std::string name) {
    pathName = name;
}

void RubyMod::setServerEnv(sol::environment env) {
    serverEnv = std::move(env);
};

void RubyMod::setClientEnv(sol::environment env) {
    clientEnv = std::move(env);
};

sol::environment& RubyMod::getServerEnv() {
    return serverEnv;
};

sol::environment& RubyMod::getClientEnv() {
    return clientEnv;
};

const std::vector<std::string>& RubyMod::getDependencies() const {
    return dependencies;
};

const std::vector<std::string>& RubyMod::getLoadAfter() const {
    return loadAfter;
};
