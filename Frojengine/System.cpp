#include "System.h"

void System::Initialize()
{
	clearColor = COLOR(0.0f, 0.0f, 0.0f, 0.0f);
	seasonCount = 0;
	timeCount = 0;
	cold = 0;
}


void System::Update()
{
	///*
#define GRID_SIZE 50.0f
#define GRID_WIDTH 1.0f
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

	if (IsKeyUp(VK_F6))		if (++timeCount == 4) timeCount = 0;
	if (IsKeyUp(VK_F7))		if (++seasonCount == 3) seasonCount = 0;


	

	if (timeCount == 0 && seasonCount == 2)
	{
		clearColor = WINTER_COLOR;
		// FOGCOLOR
		pBoxMaterial->SetVector(4, WINTER_COLOR);
		pTerrainMtrl->SetVector(4, WINTER_COLOR);
		pTreeMtrl->SetVector(4, WINTER_COLOR);
		pWindmillMtrl->SetVector(4, WINTER_COLOR);
	}

	else
	{
		clearColor = clearColArr[timeCount];
		// FOGCOLOR
		pBoxMaterial->SetVector(4, clearColArr[timeCount]);
		pTerrainMtrl->SetVector(4, clearColArr[timeCount]);
		pTreeMtrl->SetVector(4, clearColArr[timeCount]);
		pWindmillMtrl->SetVector(4, clearColArr[timeCount]);
	}

	FJRenderingEngine::SetClearColor(clearColor);




	if (seasonCount == 2)
	{
		if (cold > 0.0f)
		{
			cold -= 0.25f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		}

		else
		{
			cold = 0.0f;
		}
	}

	else
	{
		if (cold < 1.0f)
		{
			cold += 0.25f * FJSystemEngine::GetInstance()->m_fDeltaTime;
		}

		else
		{
			cold = 1.0f;
		}
	}



	pBoxMaterial->SetScalar(1, seasonCount);
	pTerrainMtrl->SetScalar(1, seasonCount);
	pTreeMtrl->SetScalar(1, seasonCount);
	pWindmillMtrl->SetScalar(1, seasonCount);

	pDirectionalLight->m_diffuse = dirLightColArr[timeCount];
	
	// 회전
	if (IsKeyDown('T')) pDirectionalLight->GetMyObject()->m_pTransform->m_vRot.x += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('G')) pDirectionalLight->GetMyObject()->m_pTransform->m_vRot.x -= XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('F')) pDirectionalLight->GetMyObject()->m_pTransform->m_vRot.y += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('H')) pDirectionalLight->GetMyObject()->m_pTransform->m_vRot.y -= XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
}