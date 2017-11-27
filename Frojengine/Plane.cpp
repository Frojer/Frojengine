#include "Plane.h"

void Plane::Initialize()
{
	plane = _pObj->GetChildren().back();
	plane->m_pTransform->m_vRot = VECTOR3(0.0f, -XM_PI / 2.0f, XM_PI / 8);
	plane->m_pTransform->m_vPos = VECTOR3(0.0f, 10.0f, 10.0f);
	

	//proParent = new CObject();

	//propeller = CObject::Find(48);
	//propeller->m_pRenderer->m_pMaterial->SetShader(CShader::Find(L"Error"));
	//propeller->m_pTransform->m_vPos = VECTOR3(0.0f, 0.35f, 3.8f);
	//propeller->m_pTransform->m_vRot = VECTOR3(-0.3f, -0.35f, 0.0f);
	//propeller->SetParent(proParent);

	////proParent = new CObject(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(-XM_PI, 0.0f, 0.0f), VECTOR3(1.0f, 1.0f, 1.0f));
	////propeller->m_pTransform->m_vRot = VECTOR3(XM_PI / 10, 0.0f, 0.0f);
	////propeller->SetParent(nullptr);
	////proParent->SetParent(plane);

	//_pObj->m_pTransform->m_vPos.y -= 15.0f;
}


void Plane::Update()
{
	_pObj->m_pTransform->m_vRot.y += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	
	
	//if (IsKeyDown('W')) propeller->m_pTransform->m_vRot.x += XM_PI * 0.1f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	//if (IsKeyDown('S')) propeller->m_pTransform->m_vRot.x -= XM_PI * 0.1f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	//if (IsKeyDown('A')) propeller->m_pTransform->m_vRot.y += XM_PI * 0.1f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	//if (IsKeyDown('D')) propeller->m_pTransform->m_vRot.y -= XM_PI * 0.1f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	//
	////if (IsKeyDown('Q')) proParent->m_pTransform->m_vRot.y += XM_PI * 1.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;;
	////if (IsKeyDown('E')) proParent->m_pTransform->m_vRot.y -= XM_PI * 1.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;;
	////proParent->m_pTransform->m_vRot.z += XM_PI * 1.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
}