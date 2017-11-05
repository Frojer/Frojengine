#include "Scene.h"


CScene::CScene(LPDEVICE i_pDevice, LPDXDC i_pDXDC)
{
	_pDevice = i_pDevice;
	_pDXDC = i_pDXDC;
}



CScene::~CScene()
{

}


void CScene::Update()
{
	for (list<CObject*>::iterator iter = _listObj.begin(); iter != _listObj.end(); iter++)
	{
		(*iter)->Update();
	}
}


void CScene::Render()
{
	for (int i = 0; i < RL_NUM; i++)
	{
		// 정렬
		SortRenderingObject((RenderingLayer)i);


		// 드로우
		for (list<CObject*>::iterator iter = _listManageDrawObj[i].begin(); iter != _listManageDrawObj[i].end(); iter++)
		{
			(*iter)->Render();
		}
	}
}



void CScene::SortRenderingObject(RenderingLayer i_Layer)
{
	
}