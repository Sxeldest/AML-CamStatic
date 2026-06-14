#pragma once

#include "common.h"

struct CWidget
{
    uintptr_t vtable;
    char padding[68];
    bool bIsTouched;
    char padding2[47];
    int nTouchID;
};
