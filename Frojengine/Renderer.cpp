#include "Renderer.h"

Renderer::Renderer()
	: _DSState(0), m_stencilRef(0), m_pMesh(nullptr), m_pMaterial(nullptr)
{
	m_name = L"Renderer";
	_type = COMPONENT_TYPE_RENDER;
}

Renderer::~Renderer()
{

}


void Renderer::BufferUpdate()
{
	m_pMaterial->UpdateConstantBuffer(GetMyObject()->m_pTransform->GetWorldMatrix());
}


void Renderer::Render()
{
	if (m_pMesh != nullptr && m_pMaterial != nullptr)
	{
		BufferUpdate();

		m_pMesh->Render();
		m_pMaterial->Render();

		FJRenderingEngine::SetDSState(_DSState, m_stencilRef);

		//±×¸®±â! Render a triangle ¡Ú
		_pDXDC->DrawIndexed(m_pMesh->m_indics.size() * 3, 0, 0);
	}
}


void Renderer::ChangeMesh(CMesh* i_pMesh)
{
	m_pMesh = i_pMesh;
}


void Renderer::ChangeMaterial(CMaterial* i_pMaterial)
{
	m_pMaterial = i_pMaterial;
}


void Renderer::SetDepthEnable(bool enable)
{
	_DSState &= 0x80000000;
	_DSState |= enable ? DS_DEPTH_TEST_ON : DS_DEPTH_TEST_OFF;
}


void Renderer::SetDepthWrite(bool enable)
{
	_DSState &= 0x40000000;
	_DSState |= enable ? DS_DEPTH_WRITE_ON : DS_DEPTH_WRITE_OFF;
}