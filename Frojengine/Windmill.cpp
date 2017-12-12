#include "Windmill.h"
#include "System.h"

void Windmill::Initialize()
{
#define AUTUMN_COLOR VECTOR4(0.666666f, 0.666666f, 0.666666f, 0.666666f)
	CObject* temp = wing->GetChildren().back()->GetChildren().back();
	temp->m_pRenderer->m_pMaterial->SetShader(CShader::Find(L"Winter"));
	temp->m_pRenderer->m_pMaterial->m_pTexture[1] = CTexture2D::Find(L"windmill_mask3.png");
	temp->m_pRenderer->m_pMaterial->SetScalar(1, system->seasonCount);
	temp->m_pRenderer->m_pMaterial->SetScalar(2, FOG_MIN);
	temp->m_pRenderer->m_pMaterial->SetScalar(3, FOG_MAX);
	temp->m_pRenderer->m_pMaterial->SetVector(3, AUTUMN_COLOR);
	system->pWindmillMtrl = temp->m_pRenderer->m_pMaterial;

	temp = body->GetChildren().back()->GetChildren().back();
	temp->m_pRenderer->ChangeMaterial(wing->GetChildren().back()->GetChildren().back()->m_pRenderer->m_pMaterial);

	wing->m_pTransform->SetPositionLocal(VECTOR3(0.0f, 2.0f, -1.0f));
}


void Windmill::Update()
{
	wing->m_pTransform->m_vRot.z += XMConvertToRadians(180) * FJSystemEngine::GetInstance()->m_fDeltaTime * system->cold;	//180º/sec 씩 회전.(DirectXMath 사용)
	
	Debug::DrawNormal(wing->GetChildren().back()->GetChildren().back(), COLOR(0.0f, 1.0f, 0.0f, 1.0f));
	Debug::DrawNormal(body->GetChildren().back()->GetChildren().back(), COLOR(0.0f, 1.0f, 0.0f, 1.0f));
}