#pragma once

#include "Vector.h"

class CMatrix {
public:
    CVector     m_right;        // 0x0  // RW: Right
    uint32_t    flags;          // 0xC
    CVector     m_forward;      // 0x10 // RW: Up
    uint32_t    pad1;           // 0x1C
    CVector     m_up;           // 0x20 // RW: At
    uint32_t    pad2;           // 0x2C
    CVector     m_pos;          // 0x30
    uint32_t    pad3;           // 0x3C

    RwMatrix*   m_pAttachMatrix;
    uint32_t    m_bOwnsAttachedMatrix;

    inline CMatrix() {
        m_right = CVector(1, 0, 0);
        m_forward = CVector(0, 1, 0);
        m_up = CVector(0, 0, 1);
        m_pos = CVector(0, 0, 0);
        flags = 0;
        pad1 = 0; pad2 = 0; pad3 = 0;
        m_pAttachMatrix = nullptr;
        m_bOwnsAttachedMatrix = 0;
    }

    static CMatrix Identity() {
        return CMatrix();
    }
};
