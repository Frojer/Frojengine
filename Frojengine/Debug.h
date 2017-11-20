#pragma once

#include "Frojengine.h"

class CMaterial;

class Debug : protected Device
{
private:
	static LPVERTEXBUFFER _pVB;
	static VECTOR3 _linePos[2];
	static CMaterial* _pLineMtrl;
public:
	Debug() {}
	~Debug() {}

	static void DrawLine(VECTOR3 start, VECTOR3 end, COLOR col);

	friend class FJRenderingEngine;
};