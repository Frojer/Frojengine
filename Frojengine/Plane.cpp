#include "Plane.h"

void Plane::Initialize()
{
	plane = GetMyObject()->GetChildren().back();
	//plane->m_pTransform->m_vRot = VECTOR3(0.0f, -XM_PI / 2.0f, XM_PI / 8);
	//plane->m_pTransform->m_vPos = VECTOR3(0.0f, 10.0f, 10.0f);
	plane->m_pTransform->m_vRot = VECTOR3(0.0f, 0.0f, 0.0f);
	plane->m_pTransform->m_vPos = VECTOR3(0.0f, 0.0f, 0.0f);
	
	list<CObject*> list = plane->GetChildren();

	FOR_STL(list)
	{
		if ((*iter)->m_name == L" propeller")
		{
			proParent = new CObject;
			propeller = (*iter);
			propeller->m_pTransform->SetRotationDegree(VECTOR3(-17.345f, 0.0f, 0.0f));
			propeller->m_pTransform->SetPositionWorld(VECTOR3(0.0f, 0.7f, 3.62f));
			propeller->SetParent(proParent);
			proParent->SetParent(plane);
			propeller->m_pTransform->SetRotationDegree(VECTOR3(17.345f, 0.0f, 0.0f));
			proParent->m_pTransform->SetPositionLocal(VECTOR3(0.0f, 0.45f, -3.69f));
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

	///*
#define GRID_SIZE 50
#define GRID_WIDTH 1
	for (int i = 0; i <= GRID_SIZE; i++)
	{
		// x축 그리드
		Debug::DrawLine(VECTOR3(0, -(GRID_SIZE * GRID_WIDTH / 2), -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH)), VECTOR3(0, GRID_SIZE * GRID_WIDTH / 2, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH)), COLOR(0.3f, 0.3f, 0.3f, 1));
		Debug::DrawLine(VECTOR3(0, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), -(GRID_SIZE * GRID_WIDTH / 2)), VECTOR3(0, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), (GRID_SIZE * GRID_WIDTH / 2)), COLOR(0.3f, 0.3f, 0.3f, 1));

		// y축 그리드
		Debug::DrawLine(VECTOR3(-(GRID_SIZE * GRID_WIDTH / 2), 0, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH)), VECTOR3(GRID_SIZE * GRID_WIDTH / 2, 0, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH)), COLOR(0.3f, 0.3f, 0.3f, 1));
		Debug::DrawLine(VECTOR3(-(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), 0, -(GRID_SIZE * GRID_WIDTH / 2)), VECTOR3(-(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), 0, (GRID_SIZE * GRID_WIDTH / 2)), COLOR(0.3f, 0.3f, 0.3f, 1));

		// z축 그리드
		Debug::DrawLine(VECTOR3(-(GRID_SIZE * GRID_WIDTH / 2), -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), 0), VECTOR3(GRID_SIZE * GRID_WIDTH / 2, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), 0), COLOR(0.3f, 0.3f, 0.3f, 1));
		Debug::DrawLine(VECTOR3(-(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), -(GRID_SIZE * GRID_WIDTH / 2), 0), VECTOR3(-(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), (GRID_SIZE * GRID_WIDTH / 2), 0), COLOR(0.3f, 0.3f, 0.3f, 1));
	}
	//*/


	//GetMyObject()->m_pTransform->m_vRot.y += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	
	
	//if (IsKeyDown('W')) propeller->m_pTransform->m_vRot.x += XM_PI * 0.1f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	//if (IsKeyDown('S')) propeller->m_pTransform->m_vRot.x -= XM_PI * 0.1f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	//if (IsKeyDown('A')) propeller->m_pTransform->m_vRot.y += XM_PI * 0.1f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	//if (IsKeyDown('D')) propeller->m_pTransform->m_vRot.y -= XM_PI * 0.1f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	//
	////if (IsKeyDown('Q')) proParent->m_pTransform->m_vRot.y += XM_PI * 1.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;;
	////if (IsKeyDown('E')) proParent->m_pTransform->m_vRot.y -= XM_PI * 1.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;;
	////proParent->m_pTransform->m_vRot.z += XM_PI * 1.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;



	/*list<CObject*> list = plane->GetChildren();
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
	}*/
}