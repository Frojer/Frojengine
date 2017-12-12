#pragma once

#include "Frojengine.h"

class CMaterial;

class Debug : protected Device
{
private:
	static LPVERTEXBUFFER _pVB;
	static VECTOR _linePos[2];
	static CMaterial* _pLineMtrl;
public:
	Debug() {}
	~Debug() {}

	static void DrawLine(VECTOR3 start, VECTOR3 end, COLOR col);
	static void DrawLine(VECTOR start, VECTOR end, COLOR col);
	static void DrawNormal(const CObject* obj, COLOR col);

	friend class FJRenderingEngine;
};