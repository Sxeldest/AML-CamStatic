#pragma once

struct CamSettings
{
    bool enableCameraPatch;
    float camSensX;
    float camSensY;
    float aimSensX;
    float aimSensY;
    float smoothness;
    float camAccel;
};

extern CamSettings g_camSettings;

void LoadCamSettings();
