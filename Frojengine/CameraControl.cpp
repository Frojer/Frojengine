#include "CameraControl.h"
#include "FJMath.h"

void CameraControl::Initialize()
{

}


void CameraControl::Update()
{
	// 이동
	if (IsKeyDown(VK_UP))		_pObj->m_pTransform->SetPositionLocal(_pObj->m_pTransform->GetPositionLocal() + (_pObj->m_pTransform->GetLookAt() * 10.0f * FJSystemEngine::GetInstance()->m_fDeltaTime));
	if (IsKeyDown(VK_DOWN))		_pObj->m_pTransform->SetPositionLocal(_pObj->m_pTransform->GetPositionLocal() + (_pObj->m_pTransform->GetLookAt() * -10.0f * FJSystemEngine::GetInstance()->m_fDeltaTime));
	if (IsKeyDown(VK_LEFT))		_pObj->m_pTransform->SetPositionLocal(_pObj->m_pTransform->GetPositionLocal() + (_pObj->m_pTransform->GetRightVector() * -10.0f * FJSystemEngine::GetInstance()->m_fDeltaTime));
	if (IsKeyDown(VK_RIGHT))	_pObj->m_pTransform->SetPositionLocal(_pObj->m_pTransform->GetPositionLocal() + (_pObj->m_pTransform->GetRightVector() * 10.0f * FJSystemEngine::GetInstance()->m_fDeltaTime));
	
	// 회전
	if (IsKeyDown(VK_NUMPAD8)) _pObj->m_pTransform->m_vRot.x -= XM_PI * 0.25f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown(VK_NUMPAD5)) _pObj->m_pTransform->m_vRot.x += XM_PI * 0.25f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown(VK_NUMPAD4)) _pObj->m_pTransform->m_vRot.y -= XM_PI * 0.25f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown(VK_NUMPAD6)) _pObj->m_pTransform->m_vRot.y += XM_PI * 0.25f * FJSystemEngine::GetInstance()->m_fDeltaTime;
}