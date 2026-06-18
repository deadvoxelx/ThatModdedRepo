#pragma once
#include "Feature.h"

class OuterEndPlatformFeature : public Feature
{
public:
    OuterEndPlatformFeature(int blockId);

    virtual bool place(Level *level, Random *random, int x, int y, int z);
};