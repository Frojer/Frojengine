#include "Hero.h"

void Hero::Initialize()
{
	auto list = GetMyObject()->GetChildren();
	FOR_STL(list)
	{
		if ((*iter)->m_pRenderer != nullptr)
		{
			(*iter)->m_pRenderer->m_pMaterial->SetShader(CShader::Find(L"Fog"));
			(*iter)->m_pRenderer->m_pMaterial->SetScalar(0, 30.0f);
			(*iter)->m_pRenderer->m_pMaterial->SetScalar(1, 10.0f);
			(*iter)->m_pRenderer->m_pMaterial->SetScalar(2, 40.0f);
			(*iter)->m_pRenderer->m_pMaterial->SetVector(2, VECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
			(*iter)->m_pRenderer->m_pMaterial->SetVector(3, VECTOR4(0.8f, 0.8f, 0.8f, 1.0f));
		}
	}
}


void Hero::Update()
{
	CObject* pObj = GetMyObject();
	Debug::DrawLine(pObj->m_pTransform->m_vPos, VECTOR3(0, 0, 0), COLOR(0, 1, 1, 1));
	if (IsKeyUp(VK_SPACE))
		FJRenderingEngine::SetWireFrame(!FJRenderingEngine::GetWireFrame());

	// 이동
	if (IsKeyDown('W')) pObj->m_pTransform->m_vPos.z += 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('S')) pObj->m_pTransform->m_vPos.z -= 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('A')) pObj->m_pTransform->m_vPos.x -= 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('D')) pObj->m_pTransform->m_vPos.x += 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('Q')) pObj->m_pTransform->m_vPos.y += 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('E')) pObj->m_pTransform->m_vPos.y -= 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;

	// 회전
	if (IsKeyDown('I')) pObj->m_pTransform->m_vRot.x += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('K')) pObj->m_pTransform->m_vRot.x -= XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('J')) pObj->m_pTransform->m_vRot.z += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('L')) pObj->m_pTransform->m_vRot.z -= XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('U')) pObj->m_pTransform->m_vRot.y += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('O')) pObj->m_pTransform->m_vRot.y -= XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
}