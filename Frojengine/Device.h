#pragma once

#include "Frojengine.h"

class Device
{
protected:
	static LPDEVICE _pDevice;
	static LPDXDC _pDXDC;

	friend class FJRenderingEngine;
};