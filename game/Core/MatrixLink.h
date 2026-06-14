#pragma once

#include "Matrix.h"

class CPlaceable;

class CMatrixLink : public CMatrix {
public:
    CMatrixLink() : CMatrix() {}

public:
    CPlaceable*  m_pOwner;
    CMatrixLink* m_pPrev;
    CMatrixLink* m_pNext;
};
