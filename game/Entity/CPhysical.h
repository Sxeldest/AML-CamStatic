#pragma once

#include "CEntityGTA.h"

struct CPhysical : public CEntityGTA {
    float       m_fPrevDistFromCam;
    uint32_t    m_LastCollisionTime;
    uint32_t    m_nPhysicalFlags;
    CVector     m_vecMoveSpeed;
    CVector     m_vecTurnSpeed;
    CVector     m_vecFrictionMoveSpeed;
    CVector     m_vecFrictionTurnSpeed;
    CVector     m_vecForce;
    CVector     m_vecTorque;
    float       m_fMass;
    float       m_fTurnMass;
    float       m_fVelocityFrequency;
    float       m_fAirResistance;
    float       m_fElasticity;
    float       m_fBuoyancyConstant;
    CVector     m_vecCentreOfMass;
    uintptr_t   m_pCollisionList;
    uintptr_t   m_pMovingList;

    uint8_t         m_nFakePhysics;
    uint8_t         m_nNumEntitiesCollided;
    uint8_t         m_nContactSurface;
    uint8_t         _pad4;

    CEntityGTA*     m_apCollidedEntities[6];
    float           m_fMovingSpeed;
    float           m_fDamageIntensity;
    CEntityGTA*     m_pDamageEntity;
    CVector         m_vecLastCollisionImpactVelocity;
    CVector         m_vecLastCollisionPosn;
    uint16_t        m_nPieceType;
    uint8_t         _pad5[2];
    CEntityGTA*     m_pAttachedTo;
    CVector         m_vecAttachPosnOffset;
    CVector         m_vecAttachTiltOffset;
    float           m_qAttachedEntityRotation[4];
    CEntityGTA*     m_pEntityIgnoredCollision;
    float           m_fContactSurfaceBrightness;
    float           m_fDynamicLighting;
    uintptr_t       m_pShadowData;
};
