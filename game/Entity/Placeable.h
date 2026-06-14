#pragma once

#include "../SimpleTransform.h"
#include "../Core/MatrixLink.h"

#pragma pack(push, 1)
class CPlaceable
{
public:
    virtual ~CPlaceable();
    CSimpleTransform    m_placement;
    CMatrixLink         *m_matrix;

    const CVector& GetPosition() const { return m_matrix ? m_matrix->m_pos : m_placement.m_vPosn; }
    CVector& GetPosition() { return m_matrix ? m_matrix->m_pos : m_placement.m_vPosn; }
};
#pragma pack(pop)
