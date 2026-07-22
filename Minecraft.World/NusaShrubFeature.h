#pragma once
#include "Feature.h"

class Level;

class NusaShrubFeature : public Feature
{
public:
    virtual bool place(Level *level, Random *random, int x, int y, int z);
};