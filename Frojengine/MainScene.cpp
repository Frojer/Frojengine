#include "MainScene.h"
#include "Hero.h"
#include "System.h"
#include "Windmill.h"
#include "Plane.h"
#include "CameraControl.h"

MainScene::MainScene()
{

}

MainScene::~MainScene()
{

}


bool MainScene::Load()
{
	CObject* pCam = new CObject();
	Camera* cam = (Camera*)pCam->AddComponent<Camera>();
	CameraControl* cc = (CameraControl*)pCam->AddComponent<CameraControl>();

	pCam->m_pTransform->SetPositionLocal(VECTOR3(0.0f, 5.0f, -30.0f));
	pCam->m_pTransform->SetRotationDegree(VECTOR3(0.0f, 0.0f, 0.0f));

	//pCam->m_pTransform->SetPositionLocal(VECTOR3(0.0f, 50.0f, 0.0f));
	//pCam->m_pTransform->SetRotationDegree(VECTOR3(90.0f, 0.0f, 0.0f));
	
	CObject* pSystem = new CObject();
	pSystem->AddComponent<System>();

	CObject* pTerrain = FileLoader::ObjectFileLoad(L"./Data/Terrain/terrain.x");
	pTerrain->m_pTransform->m_vPos = VECTOR3(0, -0.0005f, 0);

	list<CObject*> list;
	CObject* temp;
	temp = pTerrain->GetChildren().back()->GetChildren().back();
	temp->m_pRenderer->m_pMaterial->SetShader(CShader::Find(L"Fog"));
	temp->m_pRenderer->m_pMaterial->SetScalar(1, 10.0f);
	temp->m_pRenderer->m_pMaterial->SetScalar(2, 40.0f);
	temp->m_pRenderer->m_pMaterial->SetVector(3, VECTOR4(0.8f, 0.8f, 0.8f, 1.0f));



	CObject* pDwarf = FileLoader::ObjectFileLoad(L"./Data/Dwarf/Dwarf.x");
	pDwarf->m_pTransform->m_vScale = VECTOR3(3, 3, 3);
	pDwarf->AddComponent<Hero>();
	cc->_pHeroTr = pDwarf->m_pTransform;

	// 풍차 생성
	//CObject* pWindmill = FileLoader::ObjectFileLoad(L"./Data/Windmill/Windmill.x");
	//pWindmill->AddComponent<Windmill>();

	// 비행기 생성
	CObject* pPlaneModel = FileLoader::ObjectFileLoad(L"./Data/JN-4/airplane02.x");
	CObject* pPlane = new CObject();
	pPlane->AddComponent<Plane>();
	pPlaneModel->SetParent(pPlane);

	// 상자 생성
	CObject* pBox = FileLoader::ObjectFileLoad(L"./Data/Box/Box.x");
	pBox->m_pTransform->m_vPos = VECTOR3(-3.0f, 1.0f, -25.0f);
	pBox->m_pTransform->m_vScale = VECTOR3(0.2f, 0.2f, 0.2f);

	temp = pBox->GetChildren().back();
	temp->m_pRenderer->m_pMaterial->SetShader(CShader::Find(L"Fog"));
	temp->m_pRenderer->m_pMaterial->SetScalar(1, 10.0f);
	temp->m_pRenderer->m_pMaterial->SetScalar(2, 40.0f);
	temp->m_pRenderer->m_pMaterial->SetVector(3, VECTOR4(0.8f, 0.8f, 0.8f, 1.0f));

	
	CObject* pLight = new CObject;

	//pLight->m_pTransform->m_vRot = VECTOR3(-XM_PI / 2, 0.0f, 0.0f);
	pLight->m_pTransform->m_vRot = VECTOR3(XM_PI, 0.0f, 0.0f);

	Light* light = (Light*)pLight->AddComponent<Light>();
	light->m_diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light->m_ambient = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//light->m_ambient = COLOR(0.2f, 0.2f, 0.2f, 1.0f);
	light->m_lightType = LIGHT_TYPE_DIRECTION;


	CObject* pointLit = new CObject;
	pointLit->AddComponent<Renderer>();
	pointLit->SetParent(pDwarf);
	pointLit->m_pTransform->m_vPos = VECTOR3(3.5f, 3.0f, 0.0f);
	pointLit->m_pTransform->m_vScale = VECTOR3(0.02f, 0.02f, 0.02f);

	Light* plight = (Light*)pointLit->AddComponent<Light>();
	plight->m_diffuse = COLOR(1.0f, 0.40784f, 0.090196f, 1.0f);
	plight->m_ambient = COLOR(0.5f, 0.20392f, 0.045098f, 1.0f);
	plight->m_range = 50.0f;
	plight->m_lightType = LIGHT_TYPE_POINT;

	CMaterial* pMtrl = new CMaterial(CShader::Find(L"Fog"));
	pMtrl->SetScalar(0, 30.0f);
	pMtrl->SetScalar(1, 10.0f);
	pMtrl->SetScalar(2, 40.0f);
	pMtrl->SetVector(0, pMtrl->m_diffuse);
	pMtrl->SetVector(1, XMFLOAT4(pMtrl->m_ambient.x, pMtrl->m_ambient.y, pMtrl->m_ambient.y, 1.0f));
	pMtrl->SetVector(2, XMFLOAT4(pMtrl->m_specular.x, pMtrl->m_specular.y, pMtrl->m_specular.y, 1.0f));
	pMtrl->SetVector(3, VECTOR4(0.8f, 0.8f, 0.8f, 1.0f));
	pMtrl->m_pTexture[0] = CTexture2D::Find(L"woodbox.bmp");
	pointLit->m_pRenderer->ChangeMaterial(pMtrl);
	pointLit->m_pRenderer->ChangeMesh(CMesh::Find(L"Box001"));

	// Clear할 색 설정
	//FJRenderingEngine::SetClearColor(COLOR(0.0f, 0.125f, 0.3f, 1.0f));
	FJRenderingEngine::SetClearColor(COLOR(0.8f, 0.8f, 0.8f, 1.0f));

	return true;
}