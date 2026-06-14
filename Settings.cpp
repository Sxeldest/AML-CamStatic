#include "Settings.h"
#include <mod/amlmod.h>
#include <mod/config.h>

CamSettings g_camSettings;

void LoadCamSettings()
{
    g_camSettings.camSensX = cfg->Bind("CamSensX", 1.0f, "Camera")->GetFloat();
    g_camSettings.camSensY = cfg->Bind("CamSensY", 1.0f, "Camera")->GetFloat();
    g_camSettings.aimSensX = cfg->Bind("AimSensX", 0.7f, "Camera")->GetFloat();
    g_camSettings.aimSensY = cfg->Bind("AimSensY", 0.7f, "Camera")->GetFloat();
    g_camSettings.smoothness = cfg->Bind("Smoothness", 0.5f, "Camera")->GetFloat();
    g_camSettings.camAccel = cfg->Bind("CamAccel", 1.0f, "Camera")->GetFloat();
}
