#pragma once

#include "Registry/IDs.h"

struct LuaVec3 {
    double x, y, z;
    LuaVec3(double x, double y, double z) : x(x), y(y), z(z) {}
    double distanceTo(const LuaVec3& target) const {
        return std::sqrt(std::pow(x-target.x, 2) + std::pow(y-target.y, 2) + std::pow(z-target.z, 2) );
    }
    std::string toString() const {
        return "Vec3(x: " + std::to_string(x) + ", y: " + std::to_string(y) + ", z: " + std::to_string(z) + ")";
    }
    LuaVec3 addition(const LuaVec3& vec3) const {
        return LuaVec3(vec3.x + x, vec3.y + y, vec3.z + z);
    }
    LuaVec3 subtraction(const LuaVec3& vec3) const {
        return LuaVec3(x - vec3.x, y - vec3.y, z - vec3.z);
    }
    LuaVec3 multiplication(const LuaVec3& vec3) const {
        return LuaVec3(x * vec3.x, y * vec3.y, z * vec3.z);
    }
    LuaVec3 division(const LuaVec3& vec3) const {
        return LuaVec3(x / vec3.x, y / vec3.y, z / vec3.z);
    }
};

struct LuaBlock {
    LuaVec3 pos;
    int id;
    int aux;

    std::string getOID() const {
        std::ostringstream oss;
        oss << id << ':' << aux;
        return oss.str();
    }

    std::string getID() const {
        return IDMapping::get()->getByID(id, aux);
    }
};