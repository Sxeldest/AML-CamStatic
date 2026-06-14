#pragma once
#include <stdint.h>

enum eEntityType : uint8_t {
    ENTITY_TYPE_NOTHING = 0,
    ENTITY_TYPE_BUILDING,
    ENTITY_TYPE_VEHICLE,
    ENTITY_TYPE_PED,
    ENTITY_TYPE_OBJECT,
    ENTITY_TYPE_DUMMY,
    ENTITY_TYPE_NOTINPOOLS
};
