#pragma once

struct CamSettings
{
    bool enableCameraPatch;
    float CameraSensX;
    float CameraSensY;
    float AimingSensX;
    float AimingSensY;
    float CamSmoothnes;
    float CamAcceleration;
};

extern CamSettings g_camSettings;

void LoadCamSettings();
