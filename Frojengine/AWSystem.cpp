#include "AWSystem.h"
#include "FJMath.h"

AWSystem::AWSystem()
{

}

AWSystem::~AWSystem()
{

}

void AWSystem::Initialize()
{

}

void AWSystem::Update()
{
	///*
#define GRID_SIZE 50.0f
#define GRID_WIDTH 1.0f
	for (int i = 0; i <= GRID_SIZE; i++)
	{
		if (i == (int)(GRID_SIZE / 2))
		{
			// x축 그리드
			Debug::DrawLine(VECTOR3(0.0f, -(GRID_SIZE * GRID_WIDTH / 2), 0.0f), VECTOR3(0.0f, 0.0f, 0.0f), COLOR(0.3f, 0.3f, 0.3f, 1));
			Debug::DrawLine(VECTOR3(0.0f, 5.0f, 0.0f), VECTOR3(0.0f, GRID_SIZE * GRID_WIDTH / 2, 0.0f), COLOR(0.3f, 0.3f, 0.3f, 1));
			Debug::DrawLine(VECTOR3(0.0f, 0.0f, -(GRID_SIZE * GRID_WIDTH / 2)), VECTOR3(0.0f, 0.0f, 0.0f), COLOR(0.3f, 0.3f, 0.3f, 1));
			Debug::DrawLine(VECTOR3(0.0f, 0.0f, 5.0f), VECTOR3(0.0f, 0.0f, (GRID_SIZE * GRID_WIDTH / 2)), COLOR(0.3f, 0.3f, 0.3f, 1));

			// y축 그리드
			Debug::DrawLine(VECTOR3(-(GRID_SIZE * GRID_WIDTH / 2), 0.0f, 0.0f), VECTOR3(0.0f, 0.0f, 0.0f), COLOR(0.3f, 0.3f, 0.3f, 1));
			Debug::DrawLine(VECTOR3(5.0f, 0.0f,  0.0f), VECTOR3(GRID_SIZE * GRID_WIDTH / 2, 0.0f, 0.0f), COLOR(0.3f, 0.3f, 0.3f, 1));
			Debug::DrawLine(VECTOR3(0.0f, 0.0f, -(GRID_SIZE * GRID_WIDTH / 2)), VECTOR3(0.0f, 0.0f, 0.0f), COLOR(0.3f, 0.3f, 0.3f, 1));
			Debug::DrawLine(VECTOR3(0.0f, 0.0f, 5.0f), VECTOR3(0.0f, 0.0f, (GRID_SIZE * GRID_WIDTH / 2)), COLOR(0.3f, 0.3f, 0.3f, 1));
		}

		else
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
	}

	Debug::DrawLine(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(5.0f, 0.0f, 0.0f), COLOR(1.0f, 0.0f, 0.0f, 1));
	Debug::DrawLine(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(0.0f, 5.0f, 0.0f), COLOR(0.0f, 1.0f, 0.0f, 1));
	Debug::DrawLine(VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(0.0f, 0.0f, 5.0f), COLOR(0.0f, 0.0f, 1.0f, 1));
	//*/

	if (IsKeyUp(VK_F8))		SceneManager::GetInstance()->LoadScene((UINT)1);

	// 이동
	if (IsKeyDown('W'))		pBoxTr->Translate(VECTOR3(0.0f, 10.0f, 0.0f) * FJSystemEngine::GetInstance()->m_fDeltaTime);
	if (IsKeyDown('S'))		pBoxTr->Translate(VECTOR3(0.0f, -10.0f, 0.0f) * FJSystemEngine::GetInstance()->m_fDeltaTime);
	if (IsKeyDown('A'))		pBoxTr->Translate(VECTOR3(-10.0f, 0.0f, 0.0f) * FJSystemEngine::GetInstance()->m_fDeltaTime);
	if (IsKeyDown('D'))		pBoxTr->Translate(VECTOR3(10.0f, 0.0f, 0.0f) * FJSystemEngine::GetInstance()->m_fDeltaTime);
	if (IsKeyDown('Q'))		pBoxTr->Translate(VECTOR3(0.0f, 0.0f, 10.0f) * FJSystemEngine::GetInstance()->m_fDeltaTime);
	if (IsKeyDown('E'))		pBoxTr->Translate(VECTOR3(0.0f, 0.0f, -10.0f) * FJSystemEngine::GetInstance()->m_fDeltaTime);

	// 회전
	if (IsKeyDown('I')) pBoxTr->Rotate(VECTOR3(45.0f, 0.0f, 0.0f) * FJSystemEngine::GetInstance()->m_fDeltaTime);
	if (IsKeyDown('K')) pBoxTr->Rotate(VECTOR3(-45.0f, 0.0f, 0.0f) * FJSystemEngine::GetInstance()->m_fDeltaTime);
	if (IsKeyDown('J')) pBoxTr->Rotate(VECTOR3(0.0f, -45.0f, 0.0f) * FJSystemEngine::GetInstance()->m_fDeltaTime);
	if (IsKeyDown('L')) pBoxTr->Rotate(VECTOR3(0.0f, 45.0f, 0.0f) * FJSystemEngine::GetInstance()->m_fDeltaTime);
	if (IsKeyDown('U')) pBoxTr->Rotate(VECTOR3(0.0f, 0.0f, -45.0f) * FJSystemEngine::GetInstance()->m_fDeltaTime);
	if (IsKeyDown('O')) pBoxTr->Rotate(VECTOR3(0.0f, 0.0f, 45.0f) * FJSystemEngine::GetInstance()->m_fDeltaTime);
}