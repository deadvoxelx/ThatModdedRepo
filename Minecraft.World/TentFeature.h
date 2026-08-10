#pragma once
#include "Feature.h"
#include "Material.h"

class TentFeature : public Feature
{
public:
    TentFeature(int blockId);

    virtual bool place(Level *level, Random *random, int x, int y, int z);
};