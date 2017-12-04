#include "Renderer.h"

Renderer::Renderer()
	: m_pMesh(nullptr), m_pMaterial(nullptr)
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