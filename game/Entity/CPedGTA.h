#pragma once

#include "CPhysical.h"
#include "../Enums/ePedState.h"
#include "../Enums/eMoveState.h"
#include "../Enums/ePedType.h"
#include "../Enums/eWeaponType.h"
#include "../Enums/AnimationEnums.h"

struct CPedGTA : public CPhysical {
    // 0x13C is end of CPhysical
    uint8_t             m_PedAudioEntity[0x15C];      // 0x13C
    uint8_t             m_PedSpeechAudioEntity[0x100]; // 0x298
    uint8_t             m_PedWeaponAudioEntity[0xA8];  // 0x398

    uintptr_t           m_pIntelligence; // 0x440
    uintptr_t           m_pPlayerData;   // 0x444
    uint8_t             m_nCreatedBy;    // 0x448
    uint8_t             pad6[3];
    ePedState           m_nPedState;     // 0x44C
    eMoveState          m_nMoveState;    // 0x450

    uint8_t             m_storedCollPoly[0x4C]; // 0x454
    float               m_distTravelledSinceLastHeightCheck; // 0x4A0
    uint8_t             m_nPedFlags[0x10]; // 0x4A4

    uintptr_t           m_apBones[19]; // 0x4B4
    AssocGroupId        m_nAnimGroup;  // 0x500
    uint8_t             pad_anim[0x3C]; // 0x504

    float               m_fHealth;    // 0x540
    float               m_fMaxHealth; // 0x544
    float               m_fArmour;    // 0x548
    uint8_t             pad_54C[0x10]; // 0x54C

    float               m_fCurrentHeading; // 0x55C
    float               m_fTargetHeading;  // 0x560
    float               m_fHeadingRate;    // 0x564
    float               field_568;         // 0x568

    uint8_t             pad_56C[0x24];     // 0x56C
    struct CVehicleGTA* pVehicle;          // 0x590
    uint8_t             pad_594[0x8];      // 0x594
    ePedType            m_nPedType;        // 0x59C

    // CWeapon m_aWeapons[13];             // 0x5A0 (Size 13 * 0x1C = 0x16C)
    uint8_t             m_aWeaponsData[0x16C]; // 0x5A0

    uint8_t             pad_70C[0x10];     // 0x70C
    int8_t              m_nActiveWeaponSlot; // 0x71C
    uint8_t             pad_71D[0x3];
};
