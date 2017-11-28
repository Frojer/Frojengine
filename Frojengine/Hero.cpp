#include "Hero.h"

void Hero::Initialize()
{
	auto list = _pObj->GetChildren();
	FOR_STL(list)
	{
		if ((*iter)->m_pRenderer != nullptr)
		{
			(*iter)->m_pRenderer->m_pMaterial->SetScalar(0, 30.0f);
			(*iter)->m_pRenderer->m_pMaterial->SetVector(2, VECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}


void Hero::Update()
{
	Debug::DrawLine(_pObj->m_pTransform->m_vPos, VECTOR3(0, 0, 0), COLOR(0, 1, 1, 1));
	if (IsKeyUp(VK_SPACE))
		FJRenderingEngine::SetWireFrame(!FJRenderingEngine::GetWireFrame());

	// 이동
	if (IsKeyDown('W')) _pObj->m_pTransform->m_vPos.z += 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('S')) _pObj->m_pTransform->m_vPos.z -= 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('A')) _pObj->m_pTransform->m_vPos.x -= 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('D')) _pObj->m_pTransform->m_vPos.x += 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('Q')) _pObj->m_pTransform->m_vPos.y += 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('E')) _pObj->m_pTransform->m_vPos.y -= 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;

	// 회전
	if (IsKeyDown('I')) _pObj->m_pTransform->m_vRot.x += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('K')) _pObj->m_pTransform->m_vRot.x -= XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('J')) _pObj->m_pTransform->m_vRot.z += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('L')) _pObj->m_pTransform->m_vRot.z -= XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('U')) _pObj->m_pTransform->m_vRot.y += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('O')) _pObj->m_pTransform->m_vRot.y -= XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
}