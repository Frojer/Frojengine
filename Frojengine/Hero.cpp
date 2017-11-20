#include "Hero.h"

Hero::Hero(VECTOR3& pos, VECTOR3& rot, VECTOR3& scale)
	: CObject(pos, rot, scale)
{

}


Hero::~Hero()
{

}


void Hero::Initialize()
{

}


void Hero::Update()
{
#define GRID_SIZE 50
#define GRID_WIDTH 10
	for (int i = 0; i <= GRID_SIZE; i++)
	{
		Debug::DrawLine(VECTOR3(-(GRID_SIZE * GRID_WIDTH / 2), 0, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH)), VECTOR3(GRID_SIZE * GRID_WIDTH / 2, 0, -(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH)), COLOR(1, 0, 0, 1));
		Debug::DrawLine(VECTOR3(-(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), 0, -(GRID_SIZE * GRID_WIDTH / 2)), VECTOR3(-(GRID_SIZE * GRID_WIDTH / 2) + (i * GRID_WIDTH), 0, (GRID_SIZE * GRID_WIDTH / 2)), COLOR(1, 0, 0, 1));
	}
	Debug::DrawLine(m_vPos, VECTOR3(0, 0, 0), COLOR(0, 1, 1, 1));
	if (IsKeyUp(VK_SPACE))
		FJRenderingEngine::SetWireFrame(!FJRenderingEngine::GetWireFrame());

	// 이동
	if (IsKeyDown('W')) m_vPos.z += 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('S')) m_vPos.z -= 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('A')) m_vPos.x -= 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('D')) m_vPos.x += 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('Q')) m_vPos.y += 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('E')) m_vPos.y -= 15.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;

	// 회전
	if (IsKeyDown('I')) m_vRot.x += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('K')) m_vRot.x -= XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('J')) m_vRot.z += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('L')) m_vRot.z -= XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('U')) m_vRot.y += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('O')) m_vRot.y -= XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
}