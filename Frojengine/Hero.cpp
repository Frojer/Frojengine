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
			(*iter)->m_pRenderer->m_pMaterial->SetScalar(1, FOG_MIN);
			(*iter)->m_pRenderer->m_pMaterial->SetScalar(2, FOG_MAX);
			(*iter)->m_pRenderer->m_pMaterial->SetVector(2, VECTOR4(1.0f, 1.0f, 1.0f, 1.0f));
			(*iter)->m_pRenderer->m_pMaterial->SetVector(3, VECTOR4(0.8f, 0.8f, 0.8f, 1.0f));
		}
	}
}


void Hero::Update()
{
	CObject* pObj = GetMyObject();
	//Debug::DrawLine(pObj->m_pTransform->m_vPos, VECTOR3(0, 0, 0), COLOR(0, 1, 1, 1));
	
	if (state)
	{
		// �̵�
		if (IsKeyDown('W')) pObj->m_pTransform->m_vPos.z += 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		if (IsKeyDown('S')) pObj->m_pTransform->m_vPos.z -= 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		if (IsKeyDown('A')) pObj->m_pTransform->m_vPos.x -= 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		if (IsKeyDown('D')) pObj->m_pTransform->m_vPos.x += 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		if (IsKeyDown('Q')) pObj->m_pTransform->m_vPos.y += 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		if (IsKeyDown('E')) pObj->m_pTransform->m_vPos.y -= 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;

		if (IsKeyDown('Z')) pObj->m_pTransform->m_vRot.y = 0;
		if (IsKeyDown('X')) pObj->m_pTransform->m_vRot.y = XM_PI / 2;
		if (IsKeyDown('C')) pObj->m_pTransform->m_vRot.x = 0;
		if (IsKeyDown('V')) pObj->m_pTransform->m_vRot.x = XM_PI / 2;
		// ȸ��
		if (IsKeyDown('I')) pObj->m_pTransform->m_vRot.x += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		if (IsKeyDown('K')) pObj->m_pTransform->m_vRot.x -= XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		if (IsKeyDown('J')) pObj->m_pTransform->m_vRot.z += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		if (IsKeyDown('L')) pObj->m_pTransform->m_vRot.z -= XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		if (IsKeyDown('U')) pObj->m_pTransform->m_vRot.y += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		if (IsKeyDown('O')) pObj->m_pTransform->m_vRot.y -= XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	}

	else
	{
		// �̵�
		if (IsKeyDown('I')) pObj->m_pTransform->m_vPos.z += 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		if (IsKeyDown('K')) pObj->m_pTransform->m_vPos.z -= 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		if (IsKeyDown('J')) pObj->m_pTransform->m_vPos.x -= 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		if (IsKeyDown('L')) pObj->m_pTransform->m_vPos.x += 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		if (IsKeyDown('U')) pObj->m_pTransform->m_vPos.y += 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		if (IsKeyDown('O')) pObj->m_pTransform->m_vPos.y -= 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;

		//// ȸ��
		//if (IsKeyDown('I')) pObj->m_pTransform->m_vRot.x += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		//if (IsKeyDown('K')) pObj->m_pTransform->m_vRot.x -= XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		//if (IsKeyDown('J')) pObj->m_pTransform->m_vRot.z += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		//if (IsKeyDown('L')) pObj->m_pTransform->m_vRot.z -= XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		//if (IsKeyDown('U')) pObj->m_pTransform->m_vRot.y += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		//if (IsKeyDown('O')) pObj->m_pTransform->m_vRot.y -= XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	}


	auto list = GetMyObject()->GetChildren();
	if (pSystem->timeCount == 0 && pSystem->seasonCount == 2)
	{
		FOR_STL(list)
		{
			if ((*iter)->m_pRenderer != nullptr)
			{
				(*iter)->m_pRenderer->m_pMaterial->SetVector(3, WINTER_COLOR);
			}
		}
	}

	else
	{
		FOR_STL(list)
		{
			if ((*iter)->m_pRenderer != nullptr)
			{
				(*iter)->m_pRenderer->m_pMaterial->SetVector(3, pSystem->clearColArr[pSystem->timeCount]);
			}
		}
	}
}