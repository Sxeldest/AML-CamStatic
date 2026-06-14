#pragma once

struct CamSettings
{
    float camSensX;
    float camSensY;
    float aimSensX;
    float aimSensY;
    float smoothness;
    float camAccel;
};

extern CamSettings g_camSettings;

void LoadCamSettings();
