#include "MainScene.h"
#include "Hero.h"
#include "System.h"
#include "Windmill.h"

MainScene::MainScene()
{

}

MainScene::~MainScene()
{

}


bool MainScene::Load()
{
	CCamera* cam = new CCamera;

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = g_setting.displayMode.Width;
	vp.Height = g_setting.displayMode.Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	//cam->Create(VECTOR3(0.0f, 200.0f, 0.0f), VECTOR3(0.0f, 0.0f, 1.0f), VECTOR3(0.0f, 0.0f, 0.0f), 45.0f, 1.0f, 1000.0f, g_setting.displayMode.Width, g_setting.displayMode.Height, vp);
	cam->Create(VECTOR3(0.0f, 5.0f, -30.0f), VECTOR3(0.0f, 1.0f, 0.0f), VECTOR3(0.0f, 0.0f, 0.0f), 45.0f, 1.0f, 1000.0f, g_setting.displayMode.Width, g_setting.displayMode.Height, vp);

	CObject* pSystem = new CObject();
	pSystem->AddComponent<System>();

	CObject* pHero = new CObject();
	pHero->AddComponent<Hero>();

	CObject* pTerrain = CObject::CreateModel();
	//pTerrain = *pSystem;
	pTerrain.m_pTransform->m_vPos = VECTOR3(0, -0.0005f, 0);

	// 풍차 생성
	CObject* pWindmill = FileLoader::ObjectFileLoad(L"./Data/Windmill/Windmill.x");
	pWindmill->AddComponent<Windmill>();

	// 상자 생성
	CObject* pBox = FileLoader::ObjectFileLoad(L"./Data/Box/Box.x");
	pBox->m_pTransform->m_vPos = VECTOR3(-3.0f, 1.0f, -25.0f);
	pBox->m_pTransform->m_vScale = VECTOR3(0.2f, 0.2f, 0.2f);

	// Clear할 색 설정
	FJRenderingEngine::SetClearColor(COLOR(0.0f, 0.125f, 0.3f, 1.0f));

	return true;
}