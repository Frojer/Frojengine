#include "Object.h"

LPDXDC CObject::_pDXDC = nullptr;

CObject::CObject()
{
	ZeroMemory(&m_vPos, sizeof(VECTOR3));
	ZeroMemory(&m_vRot, sizeof(VECTOR3));
	m_vScale.x = 1.0f;
	m_vScale.y = 1.0f;
	m_vScale.z = 1.0f;
	
	m_pModel = nullptr;
}

CObject::CObject(CModel* pModel = nullptr, VECTOR3& pos, VECTOR3& rot, VECTOR3& scale)
	: m_vPos(pos), m_vRot(rot), m_vScale(scale), m_pModel(pModel)
{
	
}

CObject::CObject(VECTOR3& pos, VECTOR3& rot, VECTOR3& scale)
	: m_vPos(pos), m_vRot(rot), m_vScale(scale), m_pModel(nullptr)
{

}

CObject::~CObject()
{

}


void CObject::BufferUpdate()
{
	if (m_pModel == nullptr)
		return;

	m_pModel->m_pMesh->UpdateBuffer(m_vPos, m_vRot, m_vScale);
}


void CObject::Update()
{

}


void CObject::Render()
{
	m_pModel->m_pMesh->Render();
	m_pModel->m_pMaterial->Render();
	
	//±×¸®±â! Render a triangle ¡Ú
	_pDXDC->Draw(m_pModel->m_pMesh->m_verts.size(), 0);
}


void CObject::ChangeMesh(CMesh* i_pMesh)
{
	m_pModel->m_pMesh = i_pMesh;
}


void CObject::ChangeMaterial(CMaterial* i_pMaterial)
{
	m_pModel->m_pMaterial = i_pMaterial;
}