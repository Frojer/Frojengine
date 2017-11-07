#pragma once

#include "Frojengine.h"

class CObject;

enum RenderingLayer
{
	RL_1,
	RL_2,

	RL_NUM
};

class CScene
{
protected:
	LPDEVICE	_pDevice;
	LPDXDC		_pDXDC;

	list<CObject*>	_listObj;
	//list<CObject*>	_listManageDrawObj[RL_NUM];
	
public:
	LPCWSTR m_Name;

private:
	
public:
	CScene(LPDEVICE i_pDevice, LPDXDC i_pDXDC);
	~CScene();

	virtual bool Load() = 0;

	void Update();
	void Render();

	void Release();
};