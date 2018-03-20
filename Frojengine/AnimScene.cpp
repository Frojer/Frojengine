#include "AnimScene.h"
#include "AWSystem.h"
#include "CameraControl.h"

bool AnimScene::Load()
{
	CObject *temp, *temp2;

	CObject* pCam = new CObject(VECTOR3(0.0f, 0.0f, -25.0f), VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(1.0f, 1.0f, 1.0f));
	pCam->m_name = L"Camera";
	Camera* c = pCam->AddComponent<Camera>();
	CameraControl* cc = pCam->AddComponent<CameraControl>();
	cc->cam = c;

	CObject* system = new CObject;
	system->m_name = L"System";
	AWSystem* AW = system->AddComponent<AWSystem>();

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

	temp = CObject::CopyObject(pBox);
	temp->SetParent(pBox);

	temp2 = CObject::CopyObject(temp);
	temp2->SetParent(temp);

	pBox->m_pTransform->SetPositionLocal(VECTOR3(5.0f, 0.0f, 0.0f));
	pBox->m_pTransform->SetRotationDegree(VECTOR3(0.0f, 0.0f, 90.0f));
	pBox->m_pTransform->m_vScale = VECTOR3(0.02f, 0.02f, 0.02f);

	temp->m_pTransform->SetPositionLocal(VECTOR3(0.0f, 5.0f, 0.0f));
	temp->m_pTransform->SetRotationDegree(VECTOR3(0.0f, 0.0f, 0.0f));
	temp->m_pTransform->m_vScale = VECTOR3(1.0f, 1.0f, 1.0f);

	temp2->m_pTransform->SetPositionWorld(VECTOR3(2.5f, 2.5f, 0.0f));
	temp2->m_pTransform->SetRotationDegree(VECTOR3(0.0f, 0.0f, 0.0f));
	temp2->m_pTransform->m_vScale = VECTOR3(2.0f, 2.0f, 2.0f);

	AW->pBoxTr = temp2->m_pTransform;

	VECTOR3 world = temp2->m_pTransform->GetPositionWorld();
#pragma endregion

	FJRenderingEngine::SetClearColor(COLOR(0, 0.125f, 0.3f, 1));

	return true;
}