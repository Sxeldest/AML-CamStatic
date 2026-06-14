#pragma once

#include "rwlpcore.h"
#include "rwcore.h"

typedef struct RsGlobalType RsGlobalType;
struct RsGlobalType
{
    const RwChar *appName;
    RwInt32 maximumWidth;
    RwInt32 maximumHeight;
};

#define RWRGBALONG(r,g,b,a) ((unsigned int) (((a) << 24) | ((r) << 16) | ((g) << 8) | (b)))

extern RwImage* (*RtPNGImageWrite)(RwImage* image, const RwChar* imageName);
extern RwImage* (*RtPNGImageRead)(const RwChar* imageName);

void InitRenderWareFunctions();
extern RsGlobalType* RsGlobal;

typedef struct RwMatrix RwMatrix;
struct RwMatrix
{
    RwV3d right;
    unsigned int flags;
    RwV3d up;
    unsigned int pad1;
    RwV3d at;
    unsigned int pad2;
    RwV3d pos;
    unsigned int pad3;
};
