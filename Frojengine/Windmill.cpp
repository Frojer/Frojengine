#include "Windmill.h"

void Windmill::Initialize()
{
	wing->m_pTransform->SetPositionLocal(VECTOR3(0.0f, 2.0f, -1.0f));
}


void Windmill::Update()
{
	wing->m_pTransform->m_vRot.z += XMConvertToRadians(180) * FJSystemEngine::GetInstance()->m_fDeltaTime;	//180º/sec 씩 회전.(DirectXMath 사용)
}