#include "MainScene.h"
#include "Hero.h"

MainScene::MainScene(LPDEVICE i_pDevice, LPDXDC i_pDXDC)
	: CScene(i_pDevice, i_pDXDC)
{

}

MainScene::~MainScene()
{

}


bool MainScene::Load()
{
	ShaderManager::InsertShader(L"./fx/Demo.fx");
	MeshManager::InsertMesh(L"temp.obj");

	CMaterial* pMaterial = new CMaterial(ShaderManager::GetShader(L"./fx/Demo.fx"));

	CModel* pModel = new CModel(MeshManager::GetMesh(L"temp.obj"), pMaterial);

	CObject* pObj = new Hero(pModel, VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(1.0f, 1.0f, 1.0f));

	_listObj.push_back(pObj);

	// Clear할 색 설정
	FJRenderingEngine::SetClearColor(COLOR(0.0f, 0.125f, 0.3f, 1.0f));

	return true;
}