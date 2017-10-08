#pragma once

#include "Frojengine.h"
#include <DirectXTK.h>

class FJFontEngine
{
private:
	LPDEVICE	_pDevice;
	LPDXDC		_pDXDC;

	// DXTK SpriteFont
	SpriteFont*		_pFont;
	SpriteBatch*	_pFontBatch;

public:

private:

public:
	FJFontEngine(LPDEVICE i_pDevice, LPDXDC i_pDXDC);
	~FJFontEngine();

	bool FontCreate(LPCWSTR i_fileName);
	void FontRelease();

	void TextDraw(int x, int y, COLOR col, TCHAR* msg, ...);
};