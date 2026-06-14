#pragma once

#include "Placeable.h"
#include "../Enums/eEntityType.h"
#include "../Enums/eEntityStatus.h"

struct CEntityGTA : public CPlaceable{
public:
    union {
        struct RwObject* m_pRwObject;
        struct RpClump*  m_pRwClump;
        struct RpAtomic* m_pRwAtomic;
    };

    uint32_t m_nFlags;
    uint32_t flags2;
    uint16_t m_nRandomSeed;
    uint16_t m_nModelIndex;
    uintptr_t m_pReferences;
    uintptr_t m_pStreamingLink;
    uint16_t  m_nScanCode;
    uint8_t   m_nIplIndex;
    uint8_t   m_nAreaCode;
    union {
        int32_t     m_nLodIndex;
        CEntityGTA* m_pLod;
    };
    uint8_t m_nNumLodChildren;
    int8_t  m_nNumLodChildrenRendered;
    eEntityType m_nType : 3;
    eEntityStatus m_nStatus : 5;
    uint8_t pad_0;
};
