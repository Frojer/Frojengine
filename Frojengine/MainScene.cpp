#include "MainScene.h"
#include "Hero.h"
#include "System.h"
#include "Windmill.h"
#include "Plane.h"
#include "CameraControl.h"
#include "TripleWindmill.h"
#include "TripleWindmill2.h"
#include "MirrorScript.h"

MainScene::MainScene()
{

}

MainScene::~MainScene()
{

}


bool MainScene::Load()
{
#define TREE_MAX_  50
#define AUTUMN_COLOR VECTOR4(1.0f, 0.5f, 0.0f, 1.0f)

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
	CObject* temp;
	
#pragma region 터레인 생성
	CObject* pTerrain = FileLoader::ObjectFileLoad(L"./Data/Terrain/terrain.x");
	pTerrain->m_name = L"Terrain";
	pTerrain->m_pTransform->SetPositionLocal(VECTOR3(0, -0.0005f, 0));
	pTerrain->m_pTransform->m_vScale = VECTOR3(128.0f, 1.0f, 128.0f);

	temp = pTerrain->GetChildren().back()->GetChildren().back();
	temp->m_pRenderer->m_pMaterial->SetShader(CShader::Find(L"Winter"));
	temp->m_pRenderer->m_pMaterial->m_pTexture[1] = CTexture2D::Find(L"mask3.png");
	temp->m_pRenderer->m_pMaterial->SetScalar(1, system->seasonCount);
	temp->m_pRenderer->m_pMaterial->SetScalar(2, FOG_MIN);
	temp->m_pRenderer->m_pMaterial->SetScalar(3, FOG_MAX);
	temp->m_pRenderer->m_pMaterial->SetVector(3, AUTUMN_COLOR);
	temp->m_pRenderer->SetDepthWrite(false);
	system->pTerrainMtrl = temp->m_pRenderer->m_pMaterial;
#pragma endregion


	CObject* pLake = new CObject();
	pLake->m_name = L"Lake";
	pLake->AddComponent<Renderer>();
	pLake->m_pRenderer->ChangeMesh(pTerrain->GetChildren().back()->GetChildren().back()->m_pRenderer->m_pMesh);
	pLake->m_pRenderer->ChangeMaterial(new CMaterial(CShader::Find(L"StencilWrite")));
	pLake->m_pRenderer->m_pMaterial->m_pTexture[0] = CTexture2D::Find(L"Lake_m.dds");
	pLake->m_pRenderer->SetStencilEnable(true);
	pLake->m_pRenderer->SetStencilFuncFront(COMPARISON_ALWAYS);
	pLake->m_pRenderer->SetStencilPassOpFront(STENCIL_OP_REPLACE);
	pLake->m_pRenderer->SetStencilFuncBack(COMPARISON_ALWAYS);
	pLake->m_pRenderer->SetStencilPassOpBack(STENCIL_OP_REPLACE);
	pLake->m_pRenderer->m_stencilRef = 1;
	pLake->m_pRenderer->SetDepthWrite(false);
	pLake->m_pTransform->SetPositionWorld(VECTOR3(5.0f, 0.001f, -15.0f));
	pLake->m_pTransform->m_vScale = VECTOR3(16.0f, 1.0f, 16.0f);

	MirrorScript* m;
	
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

		CObject* tree = CObject::CopyObject(pTree, pos);

		temp = CObject::CopyObject(tree);
		m = temp->AddComponent<MirrorScript>();
		m->pModelTr = tree->m_pTransform;
		m->pSystem = system;
	}
	
	temp = CObject::CopyObject(pTree);
	m = temp->AddComponent<MirrorScript>();
	m->pModelTr = pTree->m_pTransform;
	m->pSystem = system;
#pragma endregion
	


#pragma region 풍차 생성
	CObject* pWindmill = new CObject;
	Windmill* wind = pWindmill->AddComponent<Windmill>();
	pWindmill->m_pTransform->SetPositionLocal(VECTOR3(5.0f, -0.0002f, -10.0f));

	CObject* pWindmillBody = FileLoader::ObjectFileLoad(L"./Data/Windmill/windmill_body.x");
	pWindmillBody->m_name = L"Windmill Body";
	CObject* pWindmillWing = FileLoader::ObjectFileLoad(L"./Data/Windmill/windmill_wing.x");
	pWindmillWing->m_name = L"Windmill Wing";
	pWindmillBody->SetParent(pWindmill);
	pWindmillWing->SetParent(pWindmill);
	wind->body = pWindmillBody;
	wind->wing = pWindmillWing;
	wind->system = system;
	wind->isMirror = false;

	temp = CObject::CopyObject(pWindmill);
	m = temp->AddComponent<MirrorScript>();
	m->pModelTr = pWindmill->m_pTransform;
	m->pSystem = system;
	wind = (Windmill*)temp->GetComponent(typeid(Windmill));
	wind->isMirror = true;

	auto list = temp->GetChildren();
	FOR_STL(list)
	{
		if ((*iter)->m_name == L"Windmill Body")
		{
			wind->body = (*iter);
		}

		else if ((*iter)->m_name == L"Windmill Wing")
		{
			wind->wing = (*iter);
		}
	}
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
	tripWind->isMirror = false;
	
	temp = CObject::CopyObject(pTripleWindmill);
	m = temp->AddComponent<MirrorScript>();
	m->pModelTr = pTripleWindmill->m_pTransform;
	m->pSystem = system;
	tripWind = (TripleWindmill*)temp->GetComponent(typeid(TripleWindmill));
	pWindmillWing = CObject::CopyObject(pWindmillWing);
	tripWind->wing[0] = pWindmillWing;
	pWindmillWing = CObject::CopyObject(pWindmillWing);
	tripWind->wing[1] = pWindmillWing;
	pWindmillWing = CObject::CopyObject(pWindmillWing);
	tripWind->wing[2] = pWindmillWing;
	tripWind->isMirror = true;
	list = temp->GetChildren();
	FOR_STL(list)
	{
		if ((*iter)->m_name == L"Windmill Body")
		{
			tripWind->body = (*iter);
		}
	}
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
	tripWind2->isMirror = false;


	temp = CObject::CopyObject(pTripleWindmill2);
	m = temp->AddComponent<MirrorScript>();
	m->pModelTr = pTripleWindmill2->m_pTransform;
	m->pSystem = system;
	tripWind2 = (TripleWindmill2*)temp->GetComponent(typeid(TripleWindmill2));
	pWindmillWing = CObject::CopyObject(pWindmillWing);
	tripWind2->wing[0] = pWindmillWing;
	pWindmillWing = CObject::CopyObject(pWindmillWing);
	tripWind2->wing[1] = pWindmillWing;
	pWindmillWing = CObject::CopyObject(pWindmillWing);
	tripWind2->wing[2] = pWindmillWing;
	tripWind2->isMirror = true;
	list = temp->GetChildren();
	FOR_STL(list)
	{
		if ((*iter)->m_name == L"Windmill Body")
		{
			tripWind->body = (*iter);
		}
	}
#pragma endregion



#pragma region 드워프 생성
	CObject* pDwarf = FileLoader::ObjectFileLoad(L"./Data/Dwarf/Dwarf.x");
	pDwarf->m_name = L"Dwarf";
	pDwarf->m_pTransform->m_vScale = VECTOR3(3, 3, 3);

	Hero* hero = pDwarf->AddComponent<Hero>();
	hero->state = 1;
	hero->pSystem = system;
	cc->_pHeroTr = pDwarf->m_pTransform;

	CObject* pMirrorDwarf = CObject::CopyObject(pDwarf);
	m = pMirrorDwarf->AddComponent<MirrorScript>();
	m->pModelTr = pDwarf->m_pTransform;
	m->pSystem = system;
#pragma endregion



#pragma region 상자 생성
	CObject* pBox = FileLoader::ObjectFileLoad(L"./Data/Box/Box.x");
	pBox->m_name = L"Box";
	pBox->m_pTransform->SetPositionLocal(VECTOR3(-3.0f, 1.0f, -25.0f));
	pBox->m_pTransform->m_vScale = VECTOR3(0.2f, 0.2f, 0.2f);

	temp = pBox->GetChildren().back();
	temp->m_pRenderer->m_pMaterial->SetShader(CShader::Find(L"Box"));
	temp->m_pRenderer->m_pMaterial->SetScalar(1, system->seasonCount);
	temp->m_pRenderer->m_pMaterial->SetScalar(2, FOG_MIN);
	temp->m_pRenderer->m_pMaterial->SetScalar(3, FOG_MAX);
	temp->m_pRenderer->m_pMaterial->SetVector(3, AUTUMN_COLOR);
	temp->m_pRenderer->m_pMaterial->m_pTexture[1] = CTexture2D::Find(L"Grass.jpg");
	temp->m_pRenderer->m_pMaterial->m_pTexture[2] = CTexture2D::Find(L"Mask2.bmp");
	temp->m_pRenderer->m_pMaterial->m_pTexture[3] = CTexture2D::Find(L"snow_mask2.png");
	system->pBoxMaterial = temp->m_pRenderer->m_pMaterial;

	temp = CObject::CopyObject(pBox);
	m = temp->AddComponent<MirrorScript>();
	m->pModelTr = pBox->m_pTransform;
	m->pSystem = system;
#pragma endregion


	
#pragma region 라이트 생성
	CObject* pLight = new CObject;
	pLight->m_name = L"Direction Light";

	//pLight->m_pTransform->m_vRot = VECTOR3(-XM_PI / 2, 0.0f, 0.0f);
	pLight->m_pTransform->SetRotationDegree(VECTOR3(50.0f, -30.0f, 0.0f));
	//pLight->m_pTransform->SetRotationDegree(VECTOR3(0.0f, 0.0f, 0.0f));

	Light* light = (Light*)pLight->AddComponent<Light>();
	light->m_diffuse = COLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light->m_ambient = COLOR(0.2f, 0.2f, 0.2f, 1.0f);
	light->m_lightType = LIGHT_TYPE_DIRECTION;

	system->pDirectionalLight = light;


	CObject* pointLit = new CObject;
	pointLit->m_name = L"Point Light";
	pointLit->AddComponent<Renderer>();
	pointLit->SetParent(pDwarf);
	pointLit->m_pTransform->SetPositionLocal(VECTOR3(3.5f, 3.0f, 0.0f));
	pointLit->m_pTransform->m_vScale = VECTOR3(0.02f, 0.02f, 0.02f);

	Light* plight = (Light*)pointLit->AddComponent<Light>();
	plight->m_diffuse = COLOR(1.0f, 0.40784f, 0.090196f, 1.0f);
	plight->m_ambient = COLOR(0.5f, 0.20392f, 0.045098f, 1.0f);
	plight->m_range = 5.0f;
	plight->m_lightType = LIGHT_TYPE_POINT;

	CMaterial* pMtrl = new CMaterial(CShader::Find(L"Standard"));
	pMtrl->m_pTexture[0] = CTexture2D::Find(L"woodbox.bmp");
	pointLit->m_pRenderer->ChangeMaterial(pMtrl);
	pointLit->m_pRenderer->ChangeMesh(CMesh::Find(L"Box001"));

	temp = CObject::CopyObject(pointLit);
	m = temp->AddComponent<MirrorScript>();
	m->pModelTr = pointLit->m_pTransform;
	m->pSystem = system;
#pragma endregion
	

	

#pragma region 비행기 생성
	CObject* pPlaneModel = FileLoader::ObjectFileLoad(L"./Data/JN-4/airplane02.x");
	pPlaneModel->m_name = L"Plane";
	CObject* pPlane = new CObject();
	pPlane->AddComponent<Plane>();
	((Plane*)pPlane->GetComponent(typeid(Plane)))->pSystem = system;
	pPlaneModel->SetParent(pPlane);
	temp = CObject::CopyObject(pPlaneModel);
	m = temp->AddComponent<MirrorScript>();
	m->pModelTr = pPlaneModel->m_pTransform;
	m->pSystem = system;
#pragma endregion


	CObject* pLakeCover = CObject::CopyObject(pLake);
	pLakeCover->m_pRenderer->ChangeMaterial(new CMaterial(CShader::Find(L"Lake")));
	pLakeCover->m_pRenderer->m_pMaterial->m_pTexture[0] = CTexture2D::Find(L"Water.jpg");
	pLakeCover->m_pRenderer->m_pMaterial->m_pTexture[1] = CTexture2D::Find(L"Lake_w.jpg");
	pLakeCover->m_pRenderer->m_pMaterial->m_pTexture[2] = CTexture2D::Find(L"Lake_m.dds");
	pLakeCover->m_pRenderer->m_pMaterial->SetVector(0, VECTOR4(1.0f, 1.0f, 1.0f, 0.8f));
	pLakeCover->m_pRenderer->m_pMaterial->SetVector(1, VECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
	pLakeCover->m_pRenderer->m_pMaterial->SetVector(2, VECTOR4(0.8f, 0.8f, 0.8f, 0.8f));
	pLakeCover->m_pRenderer->m_pMaterial->SetScalar(0, 30.0f);
	pLakeCover->m_pRenderer->m_pMaterial->SetScalar(2, FOG_MIN);
	pLakeCover->m_pRenderer->m_pMaterial->SetScalar(3, FOG_MAX);
	system->pLakeMtrl = pLakeCover->m_pRenderer->m_pMaterial;


	// Clear할 색 설정
	//FJRenderingEngine::SetClearColor(COLOR(0.0f, 0.125f, 0.3f, 1.0f));
	FJRenderingEngine::SetClearColor(COLOR(0.8f, 0.8f, 0.8f, 1.0f));

	return true;
}