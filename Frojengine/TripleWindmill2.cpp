#include "TripleWindmill2.h"
#include "System.h"

void TripleWindmill2::Initialize()
{
#define AUTUMN_COLOR VECTOR4(0.66666f, 0.666666f, 0.666666f, 0.666666f)
	CObject* temp = wing[0]->GetChildren().back()->GetChildren().back();
	temp->m_pRenderer->m_pMaterial->SetShader(CShader::Find(L"Winter"));
	temp->m_pRenderer->m_pMaterial->m_pTexture[1] = CTexture2D::Find(L"windmill_mask3.png");
	temp->m_pRenderer->m_pMaterial->SetScalar(1, system->seasonCount);
	temp->m_pRenderer->m_pMaterial->SetScalar(2, FOG_MIN);
	temp->m_pRenderer->m_pMaterial->SetScalar(3, FOG_MAX);
	temp->m_pRenderer->m_pMaterial->SetVector(3, AUTUMN_COLOR);
	system->pWindmillMtrl = temp->m_pRenderer->m_pMaterial;

	temp = body->GetChildren().back()->GetChildren().back();
	temp->m_pRenderer->ChangeMaterial(wing[0]->GetChildren().back()->GetChildren().back()->m_pRenderer->m_pMaterial);

	body->m_pTransform->m_vScale = VECTOR3(1.0f, 2.0f, 1.0f);

	if (isMirror)
	{
		wing[0]->m_pTransform->m_vPos = VECTOR3(0.0f, 2.0f, -1.0f);
		wing[0]->m_pTransform->m_vScale = VECTOR3(1.5f, 1.5f, 1.5f);
		wing[0]->SetParent(GetMyObject());
		wing[1]->m_pTransform->m_vPos = VECTOR3(-1.0f, 4.0f, 0.0f);
		wing[1]->m_pTransform->m_vScale = VECTOR3(1.0f, 1.0f, 1.0f);
		wing[1]->SetParent(GetMyObject());
		wing[2]->m_pTransform->m_vPos = VECTOR3(0.5f, 6.0f, 0.5f);
		wing[2]->m_pTransform->m_vScale = VECTOR3(0.8f, 0.8f, 0.8f);
		wing[2]->SetParent(GetMyObject());

		wing[1]->m_pTransform->m_vRot.y = -XM_PI * 0.5f;
		wing[2]->m_pTransform->m_vRot.y = XM_PI * 0.75f;
	}

	else
	{
		wing[0]->m_pTransform->m_vPos = VECTOR3(0.0f, 2.0f, -1.0f);
		wing[0]->m_pTransform->m_vScale = VECTOR3(1.5f, 1.5f, 1.5f);
		wing[0]->SetParent(GetMyObject());
		wing[1]->m_pTransform->m_vPos = VECTOR3(1.0f, 4.0f, 0.0f);
		wing[1]->m_pTransform->m_vScale = VECTOR3(1.0f, 1.0f, 1.0f);
		wing[1]->SetParent(GetMyObject());
		wing[2]->m_pTransform->m_vPos = VECTOR3(-0.5f, 6.0f, 0.5f);
		wing[2]->m_pTransform->m_vScale = VECTOR3(0.8f, 0.8f, 0.8f);
		wing[2]->SetParent(GetMyObject());

		wing[1]->m_pTransform->m_vRot.y = -XM_PI * 0.5f;
		wing[2]->m_pTransform->m_vRot.y = XM_PI * 0.75f;
	}
}


void TripleWindmill2::Update()
{
	if (isMirror)
	{
		wing[0]->m_pTransform->m_vRot.z -= -XM_PI * 0.25f * FJSystemEngine::GetInstance()->m_fDeltaTime * system->cold;
		wing[1]->m_pTransform->m_vRot.z -= XM_PI * FJSystemEngine::GetInstance()->m_fDeltaTime * system->cold;
		wing[2]->m_pTransform->m_vRot.z -= -XM_PI * 2.0f * FJSystemEngine::GetInstance()->m_fDeltaTime * system->cold;
	}

	else
	{
		wing[0]->m_pTransform->m_vRot.z += -XM_PI * 0.25f * FJSystemEngine::GetInstance()->m_fDeltaTime * system->cold;
		wing[1]->m_pTransform->m_vRot.z += XM_PI * FJSystemEngine::GetInstance()->m_fDeltaTime * system->cold;
		wing[2]->m_pTransform->m_vRot.z += -XM_PI * 2.0f * FJSystemEngine::GetInstance()->m_fDeltaTime * system->cold;
	}

	Debug::DrawNormal(wing[0]->GetChildren().front()->GetChildren().front(), COLOR(0.0f, 1.0f, 0.0f, 1.0f));
	Debug::DrawNormal(wing[1]->GetChildren().front()->GetChildren().front(), COLOR(0.0f, 1.0f, 0.0f, 1.0f));
	Debug::DrawNormal(wing[2]->GetChildren().front()->GetChildren().front(), COLOR(0.0f, 1.0f, 0.0f, 1.0f));
	Debug::DrawNormal(body->GetChildren().back()->GetChildren().back(), COLOR(0.0f, 1.0f, 0.0f, 1.0f));
}