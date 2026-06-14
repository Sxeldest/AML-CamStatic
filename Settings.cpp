#include "Settings.h"
#include <mod/amlmod.h>
#include <mod/config.h>

CamSettings g_camSettings;

void LoadCamSettings()
{
    g_camSettings.enableCameraPatch = cfg->Bind("EnableCameraPatch", true, "Camera")->GetBool();
    g_camSettings.CameraSensX = cfg->Bind("CameraSensX", 40.0f, "Camera")->GetFloat();
    g_camSettings.CameraSensY = cfg->Bind("CameraSensY", 25.0f, "Camera")->GetFloat();
    g_camSettings.AimingSensX = cfg->Bind("AimngSensX", 30.0f, "Camera")->GetFloat();
    g_camSettings.AimingSensY = cfg->Bind("AimngSensY", 20.0f, "Camera")->GetFloat();
    g_camSettings.CamSmoothnes = cfg->Bind("CamSmothnes", 12.5f, "Camera")->GetFloat();
    g_camSettings.CamAcceleration = cfg->Bind("CamAcceleration", 1.5f, "Camera")->GetFloat();
}
