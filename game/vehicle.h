#pragma once

#include <cstring>
#include "Entity/CVehicleGTA.h"

class CVehicle
{
public:
	CVehicle(int iType, float fX, float fY, float fZ, float fRotation, bool bPreloaded, bool bSiren);
	virtual ~CVehicle();

    CVehicleGTA* m_pVehicle;
	CVehicle* m_pTrailer;
    uintptr_t 	m_dwGTAId;
	bool m_bPreloaded;
	int m_iEngineState;
	int m_iLightState;
	bool m_bIsInvulnerable;
	bool m_bIsLocked;
	bool m_bDoorsLocked;
	uint8_t m_byteObjectiveVehicle;
	bool m_bSpecialMarkerEnabled;
	uint8_t m_byteColor1;
	uint8_t m_byteColor2;
	bool m_bHaveColor;
	bool m_bSiren;
	char m_szPlateText[32];
	uint32_t m_dwMarkerID;
};
