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
	if (IsKeyUp(VK_SPACE))
		FJRenderingEngine::SetWireFrame(!FJRenderingEngine::GetWireFrame());

	// 이동
	if (IsKeyDown('W')) m_vPos.z += 5.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('S')) m_vPos.z -= 5.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('A')) m_vPos.x -= 5.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('D')) m_vPos.x += 5.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('Q')) m_vPos.y += 5.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('E')) m_vPos.y -= 5.0f * FJSystemEngine::GetInstance()->m_fDeltaTime;

	// 회전
	if (IsKeyDown('I')) m_vRot.x += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('K')) m_vRot.x -= XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('J')) m_vRot.z += XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('L')) m_vRot.z -= XM_PI * 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
}