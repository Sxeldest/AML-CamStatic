#include "Settings.h"
#include <mod/amlmod.h>
#include <mod/config.h>

CamSettings g_camSettings;

void LoadCamSettings()
{
    g_camSettings.camSensX = cfg->Bind("CamSensX", 40.0f, "Camera")->GetFloat();
    g_camSettings.camSensY = cfg->Bind("CamSensY", 25.0f, "Camera")->GetFloat();
    g_camSettings.aimSensX = cfg->Bind("AimSensX", 30.0f, "Camera")->GetFloat();
    g_camSettings.aimSensY = cfg->Bind("AimSensY", 20.0f, "Camera")->GetFloat();
    g_camSettings.smoothness = cfg->Bind("Smoothness", 12.5f, "Camera")->GetFloat();
    g_camSettings.camAccel = cfg->Bind("CamAccel", 1.5f, "Camera")->GetFloat();
}
