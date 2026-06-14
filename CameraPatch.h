#pragma once

#include <stdint.h>

void CameraPatchPreload(void* gameHandle);
void CameraPatchLoad(void* gameHandle, uintptr_t gtasa);
void CameraPatchOnInitRenderware();
void CameraPatchOnRender2D();
void CameraPatchOnTouchEvent(int type, int fingerId, int x, int y);
void ApplyCustomCameraLook(float dx, float dy);
void CameraPatchOnIsTouched(int widgetId, int result);
void SetCustomLookActive(bool active, int fingerId = -1);
bool IsCameraInAimMode();
