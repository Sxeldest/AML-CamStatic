#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>
#include <math.h>

#include "game/Camera.h"
#include "game/RW/RenderWare.h"

MYMODCFG(net.dexsocy.camstatic, CamStatic, 1.0, Dexsociety)

// --- Settings ---
struct CamSettings
{
    float camSensX;
    float camSensY;
    float aimSensX;
    float aimSensY;
    float smoothness;
    float camAccel;
};
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

// --- Camera Logic ---
void* pGameHandle = nullptr;
static CCamera* s_theCamera = nullptr;
static float* s_timeStep = nullptr;
static int* s_menuOpened = nullptr;
static bool* s_userPause = nullptr;

static bool s_firstInit = true;
static float s_smoothH = 0.0f;
static float s_smoothV = 0.0f;
static float s_velH = 0.0f;
static float s_velV = 0.0f;
static float s_stopTimer = 1.0f; // Start stopped
static eCamMode s_prevMode = MODE_NONE;
static int s_transitionFrames = 0;

static const float kStopThreshold = 0.05f;

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

static float NormalizeAngle(float angle)
{
    while (angle > M_PI) angle -= (2.0f * M_PI);
    while (angle < -M_PI) angle += (2.0f * M_PI);
    return angle;
}

static float SmoothDampAngle(float current, float target, float& currentVelocity, float smoothTime, float deltaTime)
{
    float diff = target - current;
    diff = atan2f(sinf(diff), cosf(diff));
    float targetSanitized = current + diff;
    smoothTime = fmaxf(0.0001f, smoothTime);
    float omega = 2.0f / smoothTime;
    float x = omega * deltaTime;
    float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
    float change = current - targetSanitized;
    float originalTo = targetSanitized;
    float maxChange = 10.0f * smoothTime;
    change = fminf(fmaxf(change, -maxChange), maxChange);
    float temp = (currentVelocity + omega * change) * deltaTime;
    currentVelocity = (currentVelocity - omega * temp) * exp;
    float result = targetSanitized + (change + temp) * exp;
    if ((originalTo - current > 0.0f) == (result > originalTo))
    {
        result = originalTo;
        currentVelocity = 0.0f;
    }
    return result;
}

static bool IsAimMode(eCamMode mode)
{
    return (mode == MODE_AIMING || mode == MODE_SNIPER || mode == MODE_ROCKETLAUNCHER ||
            mode == MODE_1STPERSON || mode == MODE_AIMWEAPON || mode == MODE_AIMWEAPON_ATTACHED);
}

void ApplyNoopPatches(uintptr_t gtasa)
{
    unsigned char nop[4] = {0x00, 0xBF, 0x00, 0xBF};
    aml->Write(gtasa + 0x3C39B8, (uintptr_t)nop, 4);
    aml->Write(gtasa + 0x3C4090, (uintptr_t)nop, 4);
    aml->Write(gtasa + 0x3C1A72, (uintptr_t)nop, 4);
    aml->Write(gtasa + 0x3C1778, (uintptr_t)nop, 4);
}

void ProcessCameraLogic()
{
    if (!s_theCamera || !s_timeStep || !s_menuOpened || !s_userPause) return;
    if (*s_menuOpened != 0 || *s_userPause) return;

    uint8_t activeIdx = s_theCamera->m_nActiveCam;
    if (activeIdx >= 3) return;

    CCam& cam = s_theCamera->m_aCams[activeIdx];
    s_theCamera->WhoIsInControlOfTheCamera = 1;
    cam.AlphaSpeed = 0.0f;
    cam.BetaSpeed = 0.0f;

    if (s_firstInit)
    {
        s_smoothH = cam.m_fHorizontalAngle;
        s_smoothV = cam.Alpha;
        s_firstInit = false;
        s_prevMode = cam.m_nMode;
    }

    if (IsAimMode(cam.m_nMode) != IsAimMode(s_prevMode)) s_transitionFrames = 5;
    s_prevMode = cam.m_nMode;

    if (s_transitionFrames > 0)
    {
        cam.m_fHorizontalAngle = s_smoothH;
        cam.Alpha = s_smoothV;
        s_transitionFrames--;
    }

    float dt = *s_timeStep * 0.02f;
    if (dt <= 0.0f) return;

    if (s_stopTimer >= kStopThreshold)
    {
        if (s_transitionFrames == 0)
        {
            s_smoothH = cam.m_fHorizontalAngle;
            s_smoothV = cam.Alpha;
        }
        s_velH = 0.0f;
        s_velV = 0.0f;
    }
    else
    {
        float dynamicSmoothTime = 0.4f / fmaxf(0.1f, g_camSettings.smoothness);
        cam.m_fHorizontalAngle = NormalizeAngle(SmoothDampAngle(cam.m_fHorizontalAngle, s_smoothH, s_velH, dynamicSmoothTime, dt));
        cam.Alpha = SmoothDampAngle(cam.Alpha, s_smoothV, s_velV, dynamicSmoothTime, dt);
        s_stopTimer += dt;
    }

    if (cam.Alpha > 1.5f) cam.Alpha = 1.5f;
    if (cam.Alpha < -1.1f) cam.Alpha = -1.1f;
}

extern "C" void ApplyCustomCameraLook(float dx, float dy)
{
    if (!s_theCamera || !s_timeStep) return;
    uint8_t activeIdx = s_theCamera->m_nActiveCam;
    if (activeIdx >= 3) return;

    CCam& cam = s_theCamera->m_aCams[activeIdx];
    float dt = *s_timeStep * 0.02f;

    float sensMultiplier = 0.00025f;
    float sensX = (IsAimMode(cam.m_nMode) ? g_camSettings.aimSensX : g_camSettings.camSensX) * sensMultiplier;
    float sensY = (IsAimMode(cam.m_nMode) ? g_camSettings.aimSensY : g_camSettings.camSensY) * sensMultiplier;

    if (g_camSettings.camAccel > 1.0f && dt > 0.0f)
    {
        float velocity = sqrtf(dx * dx + dy * dy) / dt;
        float accelFactor = 1.0f + (velocity * 0.00002f * (g_camSettings.camAccel - 1.0f));
        if (accelFactor > 5.0f) accelFactor = 5.0f;
        sensX *= accelFactor;
        sensY *= accelFactor;
    }

    s_smoothH = NormalizeAngle(s_smoothH - (dx * sensX));
    s_smoothV = s_smoothV - (dy * sensY);

    if (s_smoothV > 1.5f) s_smoothV = 1.5f;
    if (s_smoothV < -1.1f) s_smoothV = -1.1f;

    s_stopTimer = 0.0f;
}

// --- Hooks ---
DECL_HOOK(bool, InitRenderware);
DECL_HOOKv(Render2DStuff);

bool HookOf_InitRenderware()
{
    bool res = InitRenderware();
    if (res) InitRenderWareFunctions();
    return res;
}

void HookOf_Render2DStuff()
{
    Render2DStuff();
    ProcessCameraLogic();
}

extern "C" void OnModPreLoad()
{
    pGameHandle = aml->GetLibHandle("libGTASA.so");
    if (pGameHandle)
    {
        s_theCamera = (CCamera*)aml->GetSym(pGameHandle, "TheCamera");
        s_timeStep = (float*)aml->GetSym(pGameHandle, "_ZN6CTimer12ms_fTimeStepE");
    }
}

extern "C" void OnModLoad()
{
    logger->SetTag("CamStatic");
    uintptr_t g_gtasa = aml->GetLib("libGTASA.so");
    if(!g_gtasa) return;
    pGameHandle = aml->GetLibHandle("libGTASA.so");

    LoadCamSettings();
    ApplyNoopPatches(g_gtasa);

    s_menuOpened = (int*)(g_gtasa + 0x6E0098);
    s_userPause = (bool*)aml->GetSym(pGameHandle, "_ZN6CTimer11m_UserPauseE");

    HOOK(InitRenderware, aml->GetSym(pGameHandle, "_Z14InitRenderwarev"));
    HOOK(Render2DStuff, aml->GetSym(pGameHandle, "_Z13Render2dStuffv"));

    logger->Info("CamStatic Consolidated (No Touch) loaded!");
}
