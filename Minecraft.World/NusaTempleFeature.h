#pragma once
#include "Feature.h"
#include "Material.h"

class NusaTempleFeature : public Feature
{
public:
    NusaTempleFeature(int blockId);

    virtual bool place(Level *level, Random *random, int x, int y, int z);
};