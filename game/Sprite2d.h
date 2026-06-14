#pragma once

#include "common.h"
#include "rgba.h"
#include "Core/Rect.h"

class CSprite2d
{
public:
	RwTexture* m_pTexture;
    RwTexture* m_pRwTexture;

public:
	~CSprite2d();
	void Draw(float x, float y, float width, float height, CRGBA color);
	void Draw(const CRect& posn, const CRGBA& color);
	void SetTexture(const char *name);
};
