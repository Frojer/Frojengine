#include "AnimScene.h"
#include "AWSystem.h"
#include "CameraControl.h"

bool AnimScene::Load()
{
	CObject* temp;

	CObject* pCam = new CObject(VECTOR3(0.0f, 0.0f, -25.0f), VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(1.0f, 1.0f, 1.0f));
	pCam->m_name = L"Camera";
	Camera* c = pCam->AddComponent<Camera>();
	CameraControl* cc = pCam->AddComponent<CameraControl>();
	cc->cam = c;

	CObject* system = new CObject;
	system->m_name = L"System";
	system->AddComponent<AWSystem>();

	CObject* pLight = new CObject();
	pLight->m_name = L"Light";
	pLight->m_pTransform->SetRotationDegree(VECTOR3(50.0f, -30.0f, 0.0f));
	Light* dirLight = pLight->AddComponent<Light>();
	dirLight->m_lightType = LIGHT_TYPE_DIRECTION;
	dirLight->m_diffuse = VECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	dirLight->m_ambient = VECTOR4(0.2f, 0.2f, 0.2f, 1.0f);
	
#pragma region 상자 생성
	CObject* pBox = FileLoader::ObjectFileLoad(L"./Data/Box/Box.x");
	pBox->m_name = L"Box";
	pBox->m_pTransform->m_vPos = VECTOR3(0.0f, 0.0f, 0.0f);
	pBox->m_pTransform->m_vScale = VECTOR3(0.2f, 0.2f, 0.2f);

	temp = CObject::CopyObject(pBox);
	temp->m_pTransform->SetPositionLocal(VECTOR3(0.0f, 10.0f, 0.0f));
#pragma endregion

	FJRenderingEngine::SetClearColor(COLOR(0, 0.125f, 0.3f, 1));

	return true;
}