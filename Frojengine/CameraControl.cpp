#include "CameraControl.h"
#include "FJMath.h"

void CameraControl::Initialize()
{

}


void CameraControl::Update()
{
	CObject* pObj = GetMyObject();

	// 이동
	if (IsKeyDown(VK_UP))		pObj->m_pTransform->SetPositionLocal(pObj->m_pTransform->GetPositionLocal() + (pObj->m_pTransform->GetLookAt() * 10.0f * FJSystemEngine::GetInstance()->m_fDeltaTime));
	if (IsKeyDown(VK_DOWN))		pObj->m_pTransform->SetPositionLocal(pObj->m_pTransform->GetPositionLocal() + (pObj->m_pTransform->GetLookAt() * -10.0f * FJSystemEngine::GetInstance()->m_fDeltaTime));
	if (IsKeyDown(VK_LEFT))		pObj->m_pTransform->SetPositionLocal(pObj->m_pTransform->GetPositionLocal() + (pObj->m_pTransform->GetRightVector() * -10.0f * FJSystemEngine::GetInstance()->m_fDeltaTime));
	if (IsKeyDown(VK_RIGHT))	pObj->m_pTransform->SetPositionLocal(pObj->m_pTransform->GetPositionLocal() + (pObj->m_pTransform->GetRightVector() * 10.0f * FJSystemEngine::GetInstance()->m_fDeltaTime));
	
	// 회전
	//if (IsKeyDown(VK_NUMPAD8)) pObj->m_pTransform->m_vRot.x -= XM_PI * 0.25f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	//if (IsKeyDown(VK_NUMPAD5)) pObj->m_pTransform->m_vRot.x += XM_PI * 0.25f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	//if (IsKeyDown(VK_NUMPAD4)) pObj->m_pTransform->m_vRot.y -= XM_PI * 0.25f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	//if (IsKeyDown(VK_NUMPAD6)) pObj->m_pTransform->m_vRot.y += XM_PI * 0.25f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('W')) pObj->m_pTransform->m_vRot.x -= XM_PI * 0.25f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('S')) pObj->m_pTransform->m_vRot.x += XM_PI * 0.25f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('A')) pObj->m_pTransform->m_vRot.y -= XM_PI * 0.25f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyDown('D')) pObj->m_pTransform->m_vRot.y += XM_PI * 0.25f * FJSystemEngine::GetInstance()->m_fDeltaTime;
}