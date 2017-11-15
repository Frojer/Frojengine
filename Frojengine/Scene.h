#pragma once

#include "Frojengine.h"

class CObject;
class CCamera;

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
	vector<CCamera*> _vecCam;

	static MATRIXA mView;
	static MATRIXA mProj;
	
public:
	LPCWSTR m_Name;

private:
	void ClearWasteBin(list<CObject*>::iterator iter);
	
public:
	CScene(LPDEVICE i_pDevice, LPDXDC i_pDXDC);
	~CScene();

	virtual bool Load() = 0;

	void Update();
	void Render();

	void Release();

	void AddCamera(CCamera* pCam);
	CCamera* GetCamera(UINT index);

	friend class CObject;
	friend class CMesh;
	friend class CCamera;
};