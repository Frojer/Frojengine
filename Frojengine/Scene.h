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
private:
	LPDEVICE	_pDevice;
	LPDXDC		_pDXDC;

	list<CObject*>	_listObj;
	list<CObject*>	_listManageDrawObj[RL_NUM];
	list<CObject*>	_listWasteBin;

public:

private:

public:
	CScene(LPDEVICE i_pDevice, LPDXDC i_pDXDC);
	~CScene();

	virtual bool DataLoading() = 0;

	void Update();
	void Render();

	virtual void Release() = 0;

	void SortRenderingObject(RenderingLayer i_Layer);

	void ChangeRenderingLayer(CObject& io_obj, RenderingLayer i_Layer);
};