#include "PlaneShadow.h"
#include "FJMath.h"

void PlaneShadow::Initialize()
{
	Renderer* r = GetMyObject()->m_pRenderer;

	if (r != nullptr)
	{
		r->useShadow = true;
		r->m_pMaterial = new CMaterial(CShader::Find(L"PlaneShadow"));
		r->m_pMaterial->m_pTexture[0] = tex;
		r->m_pMaterial->SetVector(0, WINTER_COLOR);
		r->m_pMaterial->SetScalar(0, FOG_MIN + 15.0f);
		r->m_pMaterial->SetScalar(1, FOG_MAX - 50.0f);
		r->SetStencilEnable(true);
		r->SetStencilFuncFront(COMPARISON_EQUAL);
		r->SetStencilPassOpFront(STENCIL_OP_INCR);
		r->SetStencilFailOpFront(STENCIL_OP_KEEP);
		r->SetStencilDepthFailOpFront(STENCIL_OP_KEEP);
		r->SetStencilFuncBack(COMPARISON_EQUAL);
		r->SetStencilPassOpBack(STENCIL_OP_INCR);
		r->SetStencilFailOpBack(STENCIL_OP_KEEP);
		r->SetStencilDepthFailOpBack(STENCIL_OP_KEEP);
	}

	InitRenderer(GetMyObject()->GetChildren());
}


void PlaneShadow::Update()
{
	Transform* tr = GetMyObject()->m_pTransform;
	tr->SetPositionWorld(m_pModelTr->GetPositionWorld());
	tr->SetRotationDegree(m_pModelTr->GetRotationDegree());
	tr->m_vScale = m_pModelTr->m_vScale;

	Renderer* r = GetMyObject()->m_pRenderer;
	if (r != nullptr)
	{
		if (sys->timeCount == 0 && sys->seasonCount == 2)
			r->m_pMaterial->SetVector(0, WINTER_COLOR);
		else
			r->m_pMaterial->SetVector(0, sys->clearColArr[sys->timeCount]);
	}

	UpdateRenderer(GetMyObject()->GetChildren());
}

void PlaneShadow::InitRenderer(list<CObject*> children)
{
	FOR_STL(children)
	{
		if ((*iter)->m_pRenderer != nullptr)
		{
			(*iter)->m_pRenderer->useShadow = true;
			(*iter)->m_pRenderer->m_pMaterial = new CMaterial(CShader::Find(L"PlaneShadow"));
			(*iter)->m_pRenderer->m_pMaterial->m_pTexture[0] = tex;
			(*iter)->m_pRenderer->m_pMaterial->SetVector(0, WINTER_COLOR);
			(*iter)->m_pRenderer->m_pMaterial->SetScalar(0, FOG_MIN + 15.0f);
			(*iter)->m_pRenderer->m_pMaterial->SetScalar(1, FOG_MAX - 20.0f);
			(*iter)->m_pRenderer->SetStencilEnable(true);
			(*iter)->m_pRenderer->SetStencilFuncFront(COMPARISON_EQUAL);
			(*iter)->m_pRenderer->SetStencilPassOpFront(STENCIL_OP_INCR);
			(*iter)->m_pRenderer->SetStencilFailOpFront(STENCIL_OP_KEEP);
			(*iter)->m_pRenderer->SetStencilDepthFailOpFront(STENCIL_OP_KEEP);
			(*iter)->m_pRenderer->SetStencilFuncBack(COMPARISON_EQUAL);
			(*iter)->m_pRenderer->SetStencilPassOpBack(STENCIL_OP_INCR);
			(*iter)->m_pRenderer->SetStencilFailOpBack(STENCIL_OP_KEEP);
			(*iter)->m_pRenderer->SetStencilDepthFailOpBack(STENCIL_OP_KEEP);
		}

		InitRenderer((*iter)->GetChildren());
	}
}


void PlaneShadow::UpdateRenderer(list<CObject*> children)
{
	Renderer* r;
	FOR_STL(children)
	{
		r = (*iter)->m_pRenderer;

		if (r != nullptr)
		{
			if (sys->timeCount == 0 && sys->seasonCount == 2)
				r->m_pMaterial->SetVector(0, WINTER_COLOR);
			else
				r->m_pMaterial->SetVector(0, sys->clearColArr[sys->timeCount]);
		}

		UpdateRenderer((*iter)->GetChildren());
	}
}