#include "Object.h"

LPDXDC CObject::_pDXDC = nullptr;
list<CObject*> CObject::_objList;

CObject::CObject()
{
	ZeroMemory(&m_vPos, sizeof(VECTOR3));
	ZeroMemory(&m_vRot, sizeof(VECTOR3));
	m_vScale.x = 1.0f;
	m_vScale.y = 1.0f;
	m_vScale.z = 1.0f;
	
	m_pModel = nullptr;

	_objList.push_back(this);
}

CObject::CObject(CModel* pModel = nullptr, VECTOR3& pos, VECTOR3& rot, VECTOR3& scale)
	: m_vPos(pos), m_vRot(rot), m_vScale(scale), m_pModel(pModel)
{
	_objList.push_back(this);
}

CObject::CObject(VECTOR3& pos, VECTOR3& rot, VECTOR3& scale)
	: m_vPos(pos), m_vRot(rot), m_vScale(scale), m_pModel(nullptr)
{
	_objList.push_back(this);
}

CObject::~CObject()
{
	FOR_LIST(_objList) { if ((*iter) == this) _objList.erase(iter); break; }
}


void CObject::BufferUpdate()
{
	if (m_pModel == nullptr)
		return;

	m_pModel->m_pMesh->UpdateBuffer(m_vPos, m_vRot, m_vScale);
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


CObject* CObject::Find(unsigned int id)
{
	FOR_LIST(_objList)
	{
		if ((*iter)->GetID() == id)
			return (*iter);
	}

	return nullptr;
}