#include "Scene.h"

MATRIXA CScene::mView;
MATRIXA CScene::mProj;

CScene::CScene()
{

}



CScene::~CScene()
{
	Release();
}



void CScene::ClearWasteBin(list<CObject*>::iterator i_iter)
{
	if ((*i_iter)->_bDead)
	{
		delete (*i_iter);
		(*i_iter) = nullptr;
		_listObj.erase(i_iter);
	}

	else
	{
		FOR_STL((*i_iter)->_childList)
		{
			ClearWasteBin(iter);
		}
	}
}



void CScene::Initialize()
{
	_listObjBk = _listObj;

	auto iter = _listObjBk.begin();
	while (iter != _listObjBk.end())
	{
		(*(iter++))->Initialize();
	}

	_listObjBk = _listObj;
}



void CScene::Update()
{
	FOR_STL(_listObjBk)
	{
		(*(iter))->Update();
	}

	_listObjBk = _listObj;

	FOR_STL(_listObjBk)
	{
		(*(iter))->AfterUpdate();
	}

	_listObjBk = _listObj;

	FOR_STL(_listObj)
	{
		ClearWasteBin(iter);
	}
}


void CScene::Render()
{
	//for (int i = 0; i < RL_NUM; i++)
	//{
	//	// 정렬
	//	// ...

	//	// 드로우
	//	for (list<CObject*>::iterator iter = _listManageDrawObj[i].begin(); iter != _listManageDrawObj[i].end(); iter++)
	//	{
	//		(*iter)->Render();
	//	}
	//}

	// Light Buffer Setting
	CMaterial::UpdateLightData();

	for (UINT i = 0; i < _vecCam.size(); i++)
	{
		if (_vecCam[i]->GetEnable())
		{
			_vecCam[i]->UseCamera();
			CMaterial::_WVPData.mView = mView;
			CMaterial::_WVPData.mProj = mProj;

			FOR_STL(_listObj)
			{
				(*(iter))->Render();
			}
		}
	}
}


void CScene::Release()
{
	auto iter = _listObj.begin();
	while (iter != _listObj.end())
	{
		delete (*iter);
		(*iter) = nullptr;
		_listObj.erase(iter++);
	}
}


void CScene::AddCamera(Camera* pCam)
{
	_vecCam.push_back(pCam);
}


Camera* CScene::GetCamera(UINT index)
{
	if (_vecCam.size() <= index)
		return nullptr;

	return _vecCam[index];
}