#include "Plane.h"

void Plane::Initialize()
{
	plane = _pObj->GetChildren().back();
	//plane->m_pTransform->m_vRot = VECTOR3(0.0f, -XM_PI / 2.0f, XM_PI / 8);
	//plane->m_pTransform->m_vPos = VECTOR3(0.0f, 10.0f, 10.0f);
	propeller = CObject::Find(48);
	propeller->m_pRenderer->m_pMaterial->SetShader(CShader::Find(L"Error"));

	proParent = new CObject();
	//VECTOR3(-0.031f, 0.419f, -3.709f)
	//proParent = new CObject(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(-XM_PI, 0.0f, 0.0f), VECTOR3(1.0f, 1.0f, 1.0f));
	propeller->m_pTransform->m_vPos = VECTOR3(0.031f, -1.5f, 3.663538f);
	propeller->m_pTransform->m_vRot = VECTOR3(XM_PI / 10, 0.0f, 0.0f);
	propeller->SetParent(proParent);
	proParent->SetParent(plane);
}


void Plane::Update()
{
	if (IsKeyDown('W')) _pObj->m_pTransform->m_vPos.y += 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('S')) _pObj->m_pTransform->m_vPos.y -= 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('A')) proParent->m_pTransform->m_vRot.z += XM_PI * 1.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;;
	if (IsKeyDown('D')) proParent->m_pTransform->m_vRot.z -= XM_PI * 1.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;;
	//_pObj->m_pTransform->m_vRot.y += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	//proParent->m_pTransform->m_vRot.z += XM_PI * 1.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
}