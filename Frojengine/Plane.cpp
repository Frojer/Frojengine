#include "Plane.h"

void Plane::Initialize()
{
	plane = GetMyObject()->GetChildren().back();
	plane->m_pTransform->m_vRot = VECTOR3(0.0f, -XM_PI / 2.0f, XM_PI / 8);
	plane->m_pTransform->m_vPos = VECTOR3(0.0f, 10.0f, 10.0f);
	
	list<CObject*> list = plane->GetChildren();

	FOR_STL(list)
	{
		if ((*iter)->m_name == L" propeller")
		{
			proParent = new CObject;
			propeller = (*iter);
			propeller->m_pTransform->SetRotationDegree(VECTOR3(-17.334f, 0.0f, 0.0f));
			propeller->m_pTransform->SetPositionLocal(VECTOR3(-0.031f, 0.7031153f, 3.663438f));
			propeller->SetParent(proParent);
			proParent->SetParent(plane);
			proParent->m_pTransform->SetRotationDegree(VECTOR3(17.334f, 0.0f, 0.0f));
			proParent->m_pTransform->SetPositionLocal(VECTOR3(-0.031f, 0.419f, -3.709f));
		}

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


void Plane::Update()
{
	GetMyObject()->m_pTransform->m_vRot.y += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	proParent->m_pTransform->m_vRot.z += XM_PI * 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	
	
	//if (IsKeyDown('W')) propeller->m_pTransform->m_vRot.x += XM_PI * 0.1f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	//if (IsKeyDown('S')) propeller->m_pTransform->m_vRot.x -= XM_PI * 0.1f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	//if (IsKeyDown('A')) propeller->m_pTransform->m_vRot.y += XM_PI * 0.1f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	//if (IsKeyDown('D')) propeller->m_pTransform->m_vRot.y -= XM_PI * 0.1f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	//
	////if (IsKeyDown('Q')) proParent->m_pTransform->m_vRot.y += XM_PI * 1.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;;
	////if (IsKeyDown('E')) proParent->m_pTransform->m_vRot.y -= XM_PI * 1.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;;



	list<CObject*> list = plane->GetChildren();
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