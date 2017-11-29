#pragma once

#include "Frojengine.h"

class CObject;
class Camera;

enum RenderingLayer
{
	RL_1,
	RL_2,

	RL_NUM
};

class CScene : private Device
{
protected:
	list<CObject*>	_listObj;
	//list<CObject*>	_listManageDrawObj[RL_NUM];
	vector<Camera*> _vecCam;

	static MATRIXA mView;
	static MATRIXA mProj;
	
public:
	LPCWSTR m_Name;

private:
	void ClearWasteBin(list<CObject*>::iterator iter);
	
public:
	CScene();
	virtual ~CScene();

	virtual bool Load() = 0;
	
	void Initialize();

	void Update();
	void Render();

	void Release();

	void AddCamera(Camera* pCam);
	Camera* GetCamera(UINT index);

	// GameObject()
	friend class CObject;
	friend class CMesh;
	friend class Camera;
};