#pragma once

#include "common.h"
#include "Core/Vector.h"
#include <mod/amlmod.h>

extern void* pGameHandle;

class CSprite {
public:
    static bool CalcScreenCoors(const CVector& posn, CVector* out, float* width, float* height, bool checkFarClip, bool checkNearClip) {
        static bool bInit = false;
        static bool (*pFunc)(const CVector&, CVector*, float*, float*, bool, bool) = nullptr;

        if (!bInit && pGameHandle) {
            pFunc = (bool (*)(const CVector&, CVector*, float*, float*, bool, bool))aml->GetSym(pGameHandle, "_ZN7CSprite15CalcScreenCoorsERK5RwV3dPS0_PfS4_bb");
            bInit = true;
        }

        if (pFunc) return pFunc(posn, out, width, height, checkFarClip, checkNearClip);
        return false;
    }
};
