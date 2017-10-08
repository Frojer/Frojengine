#pragma once

#include "Frojengine.h"

class Scene
{
private:
	LPDEVICE	_pDevice;
	LPDXDC		_pDXDC;

public:

private:

public:
	Scene(LPDEVICE i_pDevice, LPDXDC i_pDXDC);
	~Scene();

	virtual void DataLoading() = 0;

	virtual void Update() = 0;
	virtual void Render() = 0;
};