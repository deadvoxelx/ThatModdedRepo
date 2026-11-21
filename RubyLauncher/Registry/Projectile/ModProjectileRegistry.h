#pragma once

#include <string>
#include <vector>

#include "../../../Minecraft.World/ParticleTypes.h"

struct ModProjectileSpec
{
    std::string id;
    std::string particle = "snowballpoof";
    std::string trail;
    int iconItemId = -1;
    int damage = 0;
    float gravity = 0.03f;
    float throwPower = 1.5f;
    float explosionPower = 0.0f;
    int particleType = eParticleType_snowballpoof;
    int trailParticleType = -1;
};

namespace ModProjectileRegistry
{
    inline std::vector<ModProjectileSpec> &all()
    {
        static std::vector<ModProjectileSpec> specs;
        return specs;
    }

    inline int add(const ModProjectileSpec &spec)
    {
        all().push_back(spec);
        return static_cast<int>(all().size()) - 1;
    }

    inline const ModProjectileSpec *get(int specId)
    {
        if (specId < 0 || specId >= static_cast<int>(all().size())) return nullptr;
        return &all()[specId];
    }

    inline int particleTypeFor(const std::string &name)
    {
        static const struct
        {
            const char *name;
            int type;
        }
        particles[] = {
            {"bubble", eParticleType_bubble},
            {"smoke", eParticleType_smoke},
            {"note", eParticleType_note},
            {"explode", eParticleType_explode},
            {"flame", eParticleType_flame},
            {"lava", eParticleType_lava},
            {"footstep", eParticleType_footstep},
            {"splash", eParticleType_splash},
            {"largesmoke", eParticleType_largesmoke},
            {"reddust", eParticleType_reddust},
            {"snowballpoof", eParticleType_snowballpoof},
            {"snowshovel", eParticleType_snowshovel},
            {"slime", eParticleType_slime},
            {"heart", eParticleType_heart},
            {"crit", eParticleType_crit},
            {"hugeexplosion", eParticleType_hugeexplosion},
            {"largeexplode", eParticleType_largeexplode},
            {"townaura", eParticleType_townaura},
            {"spell", eParticleType_spell},
            {"witchMagic", eParticleType_witchMagic},
            {"mobSpell", eParticleType_mobSpell},
            {"magicCrit", eParticleType_magicCrit},
            {"dripWater", eParticleType_dripWater},
            {"dripLava", eParticleType_dripLava},
            {"enchantmenttable", eParticleType_enchantmenttable},
            {"dragonbreath", eParticleType_dragonbreath},
            {"ender", eParticleType_ender},
            {"angryVillager", eParticleType_angryVillager},
            {"happyVillager", eParticleType_happyVillager},
            {"fireworksspark", eParticleType_fireworksspark},
        };

        for (const auto &entry : particles)
        {
            if (name == entry.name) return entry.type;
        }
        return -1;
    }
}
