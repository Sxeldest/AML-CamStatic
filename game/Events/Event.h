#pragma once

#include "../common.h"

class CEvent {
public:
    int32 m_nTimeActive;
    bool  m_bValid;
    bool  field_9;
    bool  field_A;
    bool  field_B;

    virtual ~CEvent();
};
