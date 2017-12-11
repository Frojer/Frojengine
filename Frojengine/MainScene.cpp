#include "MainScene.h"
#include "Hero.h"
#include "System.h"
#include "Windmill.h"
#include "Plane.h"
#include "CameraControl.h"
#include "TripleWindmill.h"
#include "TripleWindmill2.h"

MainScene::MainScene()
{

}

MainScene::~MainScene()
{

}


bool MainScene::Load()
{
#define TREE_MAX_  100
#define AUTUMN_COLOR VECTOR4(1.0f, 0.5f, 0.0f, 1.0f)
#define FOG_MIN 20.0f
#define FOG_MAX 40.0f

	// 카메라 생성
	CObject* pCam = new CObject();
	pCam->m_name = L"Camera";
	Camera* cam = pCam->AddComponent<Camera>();
	CameraControl* cc = pCam->AddComponent<CameraControl>();
	cc->cam = cam;

	pCam->m_pTransform->SetPositionLocal(VECTOR3(0.0f, 5.0f, -30.0f));
	pCam->m_pTransform->SetRotationDegree(VECTOR3(0.0f, 0.0f, 0.0f));


	//pCam->m_pTransform->SetPositionLocal(VECTOR3(0.0f, 50.0f, 0.0f));
	//pCam->m_pTransform->SetRotationDegree(VECTOR3(90.0f, 0.0f, 0.0f));
	
	// 시스템 오브젝트
	CObject* pSystem = new CObject();
	pSystem->m_name = L"System";
	System* system = pSystem->AddComponent<System>();


#pragma region 터레인 생성
	CObject* pTerrain = FileLoader::ObjectFileLoad(L"./Data/Terrain/terrain.x");
	pTerrain->m_name = L"Terrain";
	pTerrain->m_pTransform->m_vPos = VECTOR3(0, -0.0005f, 0);
	pTerrain->m_pTransform->m_vScale = VECTOR3(128.0f, 0, 128.0f);

	CObject* temp;
	temp = pTerrain->GetChildren().back()->GetChildren().back();
	temp->m_pRenderer->m_pMaterial->SetShader(CShader::Find(L"Winter"));
	temp->m_pRenderer->m_pMaterial->m_pTexture[1] = CTexture2D::Find(L"mask3.png");
	temp->m_pRenderer->m_pMaterial->SetScalar(1, system->seasonCount);
	temp->m_pRenderer->m_pMaterial->SetScalar(2, FOG_MIN);
	temp->m_pRenderer->m_pMaterial->SetScalar(3, FOG_MAX);
	temp->m_pRenderer->m_pMaterial->SetVector(3, AUTUMN_COLOR);
	system->pTerrainMtrl = temp->m_pRenderer->m_pMaterial;
#pragma endregion


#pragma region 나무 생성
	CObject* pTree = FileLoader::ObjectFileLoad(L"./Data/Tree/tree.x");
	temp = pTree->GetChildren().back()->GetChildren().back();
	temp->m_pRenderer->m_pMaterial->SetShader(CShader::Find(L"Winter"));
	temp->m_pRenderer->m_pMaterial->m_pTexture[1] = CTexture2D::Find(L"tree_mask3.png");
	temp->m_pRenderer->m_pMaterial->SetScalar(1, system->seasonCount);
	temp->m_pRenderer->m_pMaterial->SetScalar(2, FOG_MIN);
	temp->m_pRenderer->m_pMaterial->SetScalar(3, FOG_MAX);
	temp->m_pRenderer->m_pMaterial->SetVector(3, AUTUMN_COLOR);
	system->pTreeMtrl = temp->m_pRenderer->m_pMaterial;

	srand(::GetTickCount());

	for (int i = 0; i < TREE_MAX_; i++)
	{
		TCHAR name[256] = L"";
		_stprintf(name, L"Tree-%03d", i);			//이름 지정.

		VECTOR3 pos;
		pos.x = (float)(rand() % 128 - 64);			//위치는 랜덤.
		pos.y = 0.0f;
		pos.z = (float)(rand() % 128 - 64);

		CObject::CopyObject(pTree, pos);
	}
#pragma endregion


#pragma region 풍차 생성
	CObject* pWindmill = new CObject;
	Windmill* wind = pWindmill->AddComponent<Windmill>();
	pWindmill->m_pTransform->SetPositionLocal(VECTOR3(5.0f, -0.0002f, -10.0f));

	CObject* pWindmillBody = FileLoader::ObjectFileLoad(L"./Data/Windmill/windmill_body.x");
	CObject* pWindmillWing = FileLoader::ObjectFileLoad(L"./Data/Windmill/windmill_wing.x");
	pWindmillBody->SetParent(pWindmill);
	pWindmillWing->SetParent(pWindmill);
	wind->body = pWindmillBody;
	wind->wing = pWindmillWing;
	wind->system = system;
#pragma endregion


#pragma region 삼단풍차1 생성
	CObject* pTripleWindmill = new CObject;
	TripleWindmill* tripWind = pTripleWindmill->AddComponent<TripleWindmill>();
	pTripleWindmill->m_pTransform->SetPositionLocal(VECTOR3(-5.0f, -0.0001f, -10.0f));
	pWindmillBody = CObject::CopyObject(pWindmillBody);
	pWindmillBody->SetParent(pTripleWindmill);
	tripWind->body = pWindmillBody;
	pWindmillWing = CObject::CopyObject(pWindmillWing);
	tripWind->wing[0] = pWindmillWing;
	pWindmillWing = CObject::CopyObject(pWindmillWing);
	tripWind->wing[1] = pWindmillWing;
	pWindmillWing = CObject::CopyObject(pWindmillWing);
	tripWind->wing[2] = pWindmillWing;
	tripWind->system = system;
#pragma endregion


#pragma region 삼단풍차2 생성
	CObject* pTripleWindmill2 = new CObject;
	TripleWindmill2* tripWind2 = pTripleWindmill2->AddComponent<TripleWindmill2>();
	pTripleWindmill2->m_pTransform->SetPositionLocal(VECTOR3(0.0f, 0.0f, -10.0f));
	pWindmillBody = CObject::CopyObject(pWindmillBody);
	pWindmillBody->SetParent(pTripleWindmill2);
	tripWind2->body = pWindmillBody;
	pWindmillWing = CObject::CopyObject(pWindmillWing);
	tripWind2->wing[0] = pWindmillWing;
	pWindmillWing = CObject::CopyObject(pWindmillWing);
	tripWind2->wing[1] = pWindmillWing;
	pWindmillWing = CObject::CopyObject(pWindmillWing);
	tripWind2->wing[2] = pWindmillWing;
	tripWind2->system = system;
#pragma endregion


	// 드워프 생성
#pragma region a
	CObject* pDwarf = FileLoader::ObjectFileLoad(L"./Data/Dwarf/Dwarf.x");
	pDwarf->m_name = L"Dwarf";
	pDwarf->m_pTransform->m_vScale = VECTOR3(3, 3, 3);
	pDwarf->AddComponent<Hero>()->state = 0;
	cc->_pHeroTr = pDwarf->m_pTransform;
#pragma endregion a

	// 비행기 생성
	CObject* pPlaneModel = FileLoader::ObjectFileLoad(L"./Data/JN-4/airplane02.x");
	pPlaneModel->m_name = L"Plane";
	CObject* pPlane = new CObject();
	pPlane->AddComponent<Plane>();
	pPlaneModel->SetParent(pPlane);

	
	CObject* pLight = new CObject;
	pLight->m_name = L"Direction Light";

	//pLight->m_pTransform->m_vRot = VECTOR3(-XM_PI / 2, 0.0f, 0.0f);
	pLight->m_pTransform->m_vRot = VECTOR3(XM_PI, 0.0f, 0.0f);

	Light* light = (Light*)pLight->AddComponent<Light>();
	light->m_diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light->m_ambient = COLOR(0.2f, 0.2f, 0.2f, 1.0f);
	light->m_lightType = LIGHT_TYPE_DIRECTION;

	system->pDirectionalLight = light;


	CObject* pointLit = new CObject;
	pointLit->m_name = L"Point Light";
	pointLit->AddComponent<Renderer>();
	pointLit->SetParent(pDwarf);
	pointLit->m_pTransform->m_vPos = VECTOR3(3.5f, 3.0f, 0.0f);
	pointLit->m_pTransform->m_vScale = VECTOR3(0.02f, 0.02f, 0.02f);

	Light* plight = (Light*)pointLit->AddComponent<Light>();
	plight->m_diffuse = COLOR(1.0f, 0.40784f, 0.090196f, 1.0f);
	plight->m_ambient = COLOR(0.5f, 0.20392f, 0.045098f, 1.0f);
	plight->m_range = 5.0f;
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


	((Hero*)CObject::CopyObject(pDwarf)->GetComponent(typeid(Hero)))->state = 1;




	// 상자 생성
	CObject* pBox = FileLoader::ObjectFileLoad(L"./Data/Box/Box.x");
	pBox->m_name = L"Box";
	pBox->m_pTransform->m_vPos = VECTOR3(-3.0f, 1.0f, -25.0f);
	pBox->m_pTransform->m_vScale = VECTOR3(0.2f, 0.2f, 0.2f);

	temp = pBox->GetChildren().back();
	temp->m_pRenderer->m_pMaterial->SetShader(CShader::Find(L"Box"));
	temp->m_pRenderer->m_pMaterial->SetScalar(1, system->seasonCount);
	temp->m_pRenderer->m_pMaterial->SetVector(3, AUTUMN_COLOR);
	temp->m_pRenderer->m_pMaterial->m_pTexture[1] = CTexture2D::Find(L"Grass.jpg");
	temp->m_pRenderer->m_pMaterial->m_pTexture[2] = CTexture2D::Find(L"Mask2.bmp");
	temp->m_pRenderer->m_pMaterial->m_pTexture[3] = CTexture2D::Find(L"snow_mask2.png");
	system->pBoxMaterial = temp->m_pRenderer->m_pMaterial;

	// Clear할 색 설정
	//FJRenderingEngine::SetClearColor(COLOR(0.0f, 0.125f, 0.3f, 1.0f));
	FJRenderingEngine::SetClearColor(COLOR(0.8f, 0.8f, 0.8f, 1.0f));

	return true;
}