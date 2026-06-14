#pragma once

#include "common.h"

class CWorld {
public:
    static inline int PlayerInFocus;

    static void Add(CEntityGTA* entity);
    static void Remove(CEntityGTA* entity);
    static float FindGroundZForCoord(float x, float y);
};
