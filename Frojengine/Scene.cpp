#include "Scene.h"


CScene::CScene(LPDEVICE i_pDevice, LPDXDC i_pDXDC)
{
	_pDevice = i_pDevice;
	_pDXDC = i_pDXDC;
}



CScene::~CScene()
{
	Release();
}


void CScene::Update()
{
	auto iter = _listObj.begin();
	while (iter != _listObj.end())
	{
		(*(iter++))->Update();
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

	auto iter = _listObj.begin();
	while (iter != _listObj.end())
	{
		(*(iter++))->Render();
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