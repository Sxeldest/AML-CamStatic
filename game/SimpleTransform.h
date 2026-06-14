#pragma once

#include "Core/Vector.h"

class CSimpleTransform {
public:
    CSimpleTransform() : m_vPosn(), m_fHeading(0.0F) {}

public:
    CVector m_vPosn;
    float m_fHeading;
};

static_assert(sizeof(CSimpleTransform) == 0x10, "Invalid size CSimpleTransform");
