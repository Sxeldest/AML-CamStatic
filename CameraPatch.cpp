#include "game/Camera.h"
#include "game/Widget.h"
#include "Settings.h"
#include <mod/amlmod.h>
#include "game/RW/RenderWare.h"
#include <math.h>

static void* s_gameHandle = nullptr;
static CCamera* s_theCamera = nullptr;
static float* s_timeStep = nullptr;
static uintptr_t* s_widgets = nullptr;
static int* s_menuOpened = nullptr;
static bool* s_userPause = nullptr;
static int s_displayX = 0;
static int s_displayY = 0;
static bool s_lookWidgetTouched = false;
static bool s_customLookActive = false;
static int s_customLookFinger = -1;
static bool s_firstInit = true;
static float s_smoothH = 0.0f;
static float s_smoothV = 0.0f;
static float s_velH = 0.0f;
static float s_velV = 0.0f;
static float s_stopTimer = 0.0f;
static float s_fingerDeltaX[15] = {0};
static float s_fingerDeltaY[15] = {0};
static float s_lastTouchX[15] = {0};
static float s_lastTouchY[15] = {0};
static bool s_fingerDown[15] = {false};
static int s_activeCameraFinger = -1;
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

static void ResetTouchDeltas()
{
    for (int i = 0; i < 15; ++i)
    {
        s_fingerDeltaX[i] = 0.0f;
        s_fingerDeltaY[i] = 0.0f;
    }
}

static void ApplyNoopPatches(uintptr_t gtasa)
{
    unsigned char nop[4] = {0x00, 0xBF, 0x00, 0xBF};
    aml->Write(gtasa + 0x3C39B8, (uintptr_t)nop, 4);
    aml->Write(gtasa + 0x3C4090, (uintptr_t)nop, 4);
    aml->Write(gtasa + 0x3C1A72, (uintptr_t)nop, 4);
    aml->Write(gtasa + 0x3C1778, (uintptr_t)nop, 4);
}

void CameraPatchPreload(void* gameHandle)
{
    s_gameHandle = gameHandle;
    if (!s_gameHandle) return;

    s_theCamera = (CCamera*)aml->GetSym(s_gameHandle, "TheCamera");
    s_timeStep = (float*)aml->GetSym(s_gameHandle, "_ZN6CTimer12ms_fTimeStepE");
}

void CameraPatchLoad(void* gameHandle, uintptr_t gtasa)
{
    s_gameHandle = gameHandle;
    if (!s_gameHandle) return;

    ApplyNoopPatches(gtasa);
    s_widgets = (uintptr_t*)aml->GetSym(s_gameHandle, "_ZN15CTouchInterface10m_pWidgetsE");
    s_menuOpened = (int*)(gtasa + 0x6E0098);
    s_userPause = (bool*)aml->GetSym(s_gameHandle, "_ZN6CTimer11m_UserPauseE");
}

void CameraPatchOnInitRenderware()
{
    if (RsGlobal)
    {
        s_displayX = RsGlobal->maximumWidth;
        s_displayY = RsGlobal->maximumHeight;
    }
}

void CameraPatchOnRender2D()
{
    if (!s_theCamera || !s_timeStep || !s_widgets || !s_menuOpened || !s_userPause)
    {
        ResetTouchDeltas();
        return;
    }

    if (*s_menuOpened != 0 || *s_userPause)
    {
        ResetTouchDeltas();
        return;
    }

    uint8_t activeIdx = s_theCamera->m_nActiveCam;
    if (activeIdx >= 3) return;

    CCam& cam = s_theCamera->m_aCams[activeIdx];

    CWidget* lookWidget = (CWidget*)s_widgets[175];
    bool isCurrentlyTouched = s_lookWidgetTouched || s_customLookActive;
    if (lookWidget && lookWidget->bIsTouched) isCurrentlyTouched = true;

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

    if (IsAimMode(cam.m_nMode) != IsAimMode(s_prevMode))
    {
        s_transitionFrames = 5;
    }
    s_prevMode = cam.m_nMode;

    if (s_transitionFrames > 0)
    {
        cam.m_fHorizontalAngle = s_smoothH;
        cam.Alpha = s_smoothV;
        s_transitionFrames--;
    }

    float dt = *s_timeStep * 0.02f;
    if (dt <= 0.0f) return;

    bool isMoving = false;
    if (s_displayX <= 0 && RsGlobal)
    {
        s_displayX = RsGlobal->maximumWidth;
        s_displayY = RsGlobal->maximumHeight;
    }

    if (isCurrentlyTouched)
    {
        // Reset active finger if it's no longer down
        if (s_activeCameraFinger != -1 && !s_fingerDown[s_activeCameraFinger])
        {
            s_activeCameraFinger = -1;
        }

        if (s_activeCameraFinger == -1)
        {
            int candidateId = -1;
            if (s_customLookActive && s_customLookFinger != -1 && s_fingerDown[s_customLookFinger])
            {
                candidateId = s_customLookFinger;
            }
            else if (lookWidget && lookWidget->bIsTouched && lookWidget->nTouchID >= 0 && lookWidget->nTouchID < 15 && s_fingerDown[lookWidget->nTouchID])
            {
                candidateId = lookWidget->nTouchID;
            }

            float rightSideThreshold = (s_displayX > 0) ? (float)s_displayX * 0.45f : 500.0f;

            // If we have a candidate from widget/custom, verify it's on the right side (unless custom)
            if (candidateId != -1)
            {
                if (s_customLookActive || s_lastTouchX[candidateId] > rightSideThreshold)
                {
                    s_activeCameraFinger = candidateId;
                }
            }

            // If still no active finger, look for any moving finger on the right
            if (s_activeCameraFinger == -1)
            {
                for (int i = 0; i < 15; ++i)
                {
                    if (s_fingerDown[i] && s_lastTouchX[i] > rightSideThreshold && (fabsf(s_fingerDeltaX[i]) > 0.01f || fabsf(s_fingerDeltaY[i]) > 0.01f))
                    {
                        s_activeCameraFinger = i;
                        break;
                    }
                }
            }
        }

        int fingerId = s_activeCameraFinger;
        if (fingerId >= 0 && fingerId < 15)
        {
            float dx = s_fingerDeltaX[fingerId];
            float dy = s_fingerDeltaY[fingerId];

            if (fabsf(dx) > 0.0f || fabsf(dy) > 0.0f)
            {
                isMoving = true;

                    float sensMultiplier = 0.00025f;
                    float sensX = (IsAimMode(cam.m_nMode) ? g_camSettings.aimSensX : g_camSettings.camSensX) * sensMultiplier;
                    float sensY = (IsAimMode(cam.m_nMode) ? g_camSettings.aimSensY : g_camSettings.camSensY) * sensMultiplier;

                    if (g_camSettings.camAccel > 1.0f)
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
                }
            }
        }

    if (isMoving && isCurrentlyTouched)
    {
        s_stopTimer = 0.0f;
    }
    else
    {
        s_stopTimer += dt;
    }

    if (!isCurrentlyTouched || s_stopTimer >= kStopThreshold)
    {
        if (s_transitionFrames == 0)
        {
            s_smoothH = cam.m_fHorizontalAngle;
            s_smoothV = cam.Alpha;
        }
        s_velH = 0.0f;
        s_velV = 0.0f;
        s_activeCameraFinger = -1;
    }
    else
    {
        float dynamicSmoothTime = 0.4f / fmaxf(0.1f, g_camSettings.smoothness);
        cam.m_fHorizontalAngle = NormalizeAngle(SmoothDampAngle(cam.m_fHorizontalAngle, s_smoothH, s_velH, dynamicSmoothTime, dt));
        cam.Alpha = SmoothDampAngle(cam.Alpha, s_smoothV, s_velV, dynamicSmoothTime, dt);
    }

    if (cam.Alpha > 1.5f) cam.Alpha = 1.5f;
    if (cam.Alpha < -1.1f) cam.Alpha = -1.1f;

    ResetTouchDeltas();
}

void CameraPatchOnTouchEvent(int type, int fingerId, int x, int y)
{
    if (fingerId < 0 || fingerId >= 15) return;

    if (type == 2) // DOWN
    {
        s_fingerDown[fingerId] = true;
        s_lastTouchX[fingerId] = (float)x;
        s_lastTouchY[fingerId] = (float)y;
        s_fingerDeltaX[fingerId] = 0.0f;
        s_fingerDeltaY[fingerId] = 0.0f;
    }
    else if (type == 3) // MOVE
    {
        s_fingerDeltaX[fingerId] += (float)(x - s_lastTouchX[fingerId]);
        s_fingerDeltaY[fingerId] += (float)(y - s_lastTouchY[fingerId]);
        s_lastTouchX[fingerId] = (float)x;
        s_lastTouchY[fingerId] = (float)y;
    }
    else if (type == 1) // UP
    {
        s_fingerDown[fingerId] = false;
        s_fingerDeltaX[fingerId] = 0.0f;
        s_fingerDeltaY[fingerId] = 0.0f;
    }
}

void ApplyCustomCameraLook(float dx, float dy)
{
    if (!s_theCamera) return;
    uint8_t activeIdx = s_theCamera->m_nActiveCam;
    if (activeIdx >= 3) return;

    CCam& cam = s_theCamera->m_aCams[activeIdx];

    float sensMultiplier = 0.00025f;
    float sensX = (IsAimMode(cam.m_nMode) ? g_camSettings.aimSensX : g_camSettings.camSensX) * sensMultiplier;
    float sensY = (IsAimMode(cam.m_nMode) ? g_camSettings.aimSensY : g_camSettings.camSensY) * sensMultiplier;

    s_smoothH = NormalizeAngle(s_smoothH - (dx * sensX));
    s_smoothV = s_smoothV - (dy * sensY);

    if (s_smoothV > 1.5f) s_smoothV = 1.5f;
    if (s_smoothV < -1.1f) s_smoothV = -1.1f;
}

void CameraPatchOnIsTouched(int widgetId, int result)
{
    if (widgetId == 175)
    {
        s_lookWidgetTouched = (result != 0);
    }
}

void SetCustomLookActive(bool active, int fingerId)
{
    s_customLookActive = active;
    s_customLookFinger = fingerId;
}

bool IsCameraInAimMode()
{
    if (!s_theCamera) return false;
    uint8_t activeIdx = s_theCamera->m_nActiveCam;
    if (activeIdx >= 3) return false;
    return IsAimMode(s_theCamera->m_aCams[activeIdx].m_nMode);
}
