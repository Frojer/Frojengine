#pragma once

#include "Frojengine.h"

class CMaterial;

class Debug
{
private:
	static LPDEVICE _pDevice;
	static LPDXDC _pDXDC;

	static LPVERTEXBUFFER _pVB;
	static VECTOR3 _linePos[2];
	static CMaterial* _pLineMtrl;
public:
	Debug() {}
	~Debug() {}

	static void DrawLine(VECTOR3 start, VECTOR3 end, COLOR col);

	friend class FJRenderingEngine;
};