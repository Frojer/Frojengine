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



void CScene::Update()
{
	auto iter = _listObj.begin();
	while (iter != _listObj.end())
	{
		(*(iter++))->Update();
	}

	iter = _listObj.begin();
	while (iter != _listObj.end())
	{
		(*(iter++))->AfterUpdate();
	}

	iter = _listObj.begin();
	while (iter != _listObj.end())
	{
		ClearWasteBin(iter++);
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

	for (UINT i = 0; i < _vecCam.size(); i++)
	{
		if (_vecCam[i]->m_Enable)
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


void CScene::AddCamera(CCamera* pCam)
{
	_vecCam.push_back(pCam);
}


CCamera* CScene::GetCamera(UINT index)
{
	if (_vecCam.size() <= index)
		return nullptr;

	return _vecCam[index];
}