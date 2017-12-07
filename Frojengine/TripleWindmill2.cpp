#include "TripleWindmill2.h"

void TripleWindmill2::Initialize()
{
	body->m_pTransform->m_vScale = VECTOR3(1.0f, 2.0f, 1.0f);

	wing[0]->m_pTransform->m_vPos = VECTOR3(0.0f, 2.0f, -1.0f);
	wing[0]->m_pTransform->m_vScale = VECTOR3(1.5f, 1.5f, 1.5f);
	wing[0]->SetParent(GetMyObject());
	wing[1]->m_pTransform->m_vPos = VECTOR3(1.0f, 4.0f, 0.0f);
	wing[1]->m_pTransform->m_vScale = VECTOR3(1.0f, 1.0f, 1.0f);
	wing[1]->SetParent(GetMyObject());
	wing[2]->m_pTransform->m_vPos = VECTOR3(-0.5f, 6.0f, 0.5f);
	wing[2]->m_pTransform->m_vScale = VECTOR3(0.8f, 0.8f, 0.8f);
	wing[2]->SetParent(GetMyObject());

	wing[1]->m_pTransform->m_vRot.y = -XM_PI * 0.5f;
	wing[2]->m_pTransform->m_vRot.y = XM_PI * 0.75f;
}


void TripleWindmill2::Update()
{
	wing[0]->m_pTransform->m_vRot.z += -XM_PI * 0.25f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	wing[1]->m_pTransform->m_vRot.z += XM_PI * FJSystemEngine::GetInstance()->m_fDeltaTime;
	wing[2]->m_pTransform->m_vRot.z += -XM_PI * 2.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
}