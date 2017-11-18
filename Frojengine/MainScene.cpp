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
	CCamera* cam = new CCamera;

	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = g_setting.displayMode.Width;
	vp.Height = g_setting.displayMode.Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;

	cam->Create(VECTOR3(0.0f, 5.0f, -50.0f), VECTOR3(0.0f, 1.0f, 0.0f), VECTOR3(0.0f, 0.0f, 0.0f), 45.0f, 1.0f, 100.0f, g_setting.displayMode.Width, g_setting.displayMode.Height, vp);


	CObject* pPlane = FileLoader::ObjectFileLoad(L"./Data/JN-4/airplane02.x");
	CObject* pBox = FileLoader::ObjectFileLoad(L"./Data/Box/Box.x");
	pBox->m_vPos = VECTOR3(0.0f, -20.0f, 0.0f);
	CObject* pObj = new Hero(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(1.0f, 1.0f, 1.0f));
	pObj->m_name = L"Hero";

	CMaterial* pMtrl = new CMaterial(CShader::Find(L"Line"));

	pBox->GetChildren().back()->m_pMaterial->SetShader(CShader::Find(L"Error"));
	pBox->SetParent(pObj);

	pPlane->SetParent(pObj);
	pPlane->m_vScale = VECTOR3(3.0f, 3.0f, 3.0f);

	// Clear할 색 설정
	FJRenderingEngine::SetClearColor(COLOR(0.0f, 0.125f, 0.3f, 1.0f));

	FOR_STL(_listObj)
	{
		(*iter)->Initialize();
	}

	return true;
}