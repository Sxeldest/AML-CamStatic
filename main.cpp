#include <mod/amlmod.h>
#include <mod/logger.h>
#include <mod/config.h>
#include "CameraPatch.h"
#include "Settings.h"
#include "game/RW/RenderWare.h"

MYMODCFG(net.dexsocy.camstatic, CamStatic, 1.0, Dexsociety)

uintptr_t g_gtasa = 0;
void* pGameHandle = nullptr;

// Addresses
uintptr_t addrIsTouched = 0x2B0CBC;

// Hook declarations
DECL_HOOK(bool, InitRenderware);
DECL_HOOKv(Render2DStuff);
DECL_HOOKv(OnTouchEvent, int type, int fingerId, int x, int y);
DECL_HOOKi(IsTouched, int widgetId, void* a2, int a3);

// Hook implementations
bool HookOf_InitRenderware()
{
    if (!InitRenderware()) return false;
    InitRenderWareFunctions();
    CameraPatchOnInitRenderware();
    return true;
}

void HookOf_Render2DStuff()
{
    Render2DStuff();
    CameraPatchOnRender2D();
}

void HookOf_OnTouchEvent(int type, int fingerId, int x, int y)
{
    if (fingerId < 0 || fingerId >= 15) return;
    CameraPatchOnTouchEvent(type, fingerId, x, y);

    // Call original for first few fingers to not break game
    if (fingerId < 4)
    {
        OnTouchEvent(type, fingerId, x, y);
    }
}

int HookOf_IsTouched(int widgetId, void* a2, int a3)
{
    int result = IsTouched(widgetId, a2, a3);
    CameraPatchOnIsTouched(widgetId, result);
    return result;
}

extern "C" void OnModPreLoad()
{
    g_gtasa = aml->GetLib("libGTASA.so");
    if (g_gtasa)
    {
        pGameHandle = aml->GetLibHandle("libGTASA.so");
        CameraPatchPreload(pGameHandle);
    }
}

extern "C" void OnModLoad()
{
    logger->SetTag("CamStatic");

    g_gtasa = aml->GetLib("libGTASA.so");
    if(!g_gtasa)
    {
        logger->Error("libGTASA.so not found!");
        return;
    }

    LoadCamSettings();
    CameraPatchLoad(pGameHandle, g_gtasa);

    HOOK(InitRenderware, aml->GetSym(pGameHandle, "_Z14InitRenderwarev"));
    HOOK(Render2DStuff, aml->GetSym(pGameHandle, "_Z13Render2dStuffv"));
    HOOKPLT(OnTouchEvent, g_gtasa + 0x675DE4);
    HOOK(IsTouched, g_gtasa + addrIsTouched + 1);

    logger->Info("CamStatic loaded!");
}
