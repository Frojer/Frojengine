#include "Object.h"

LPDXDC CObject::_pDXDC = nullptr;

CObject::CObject()
{
	ZeroMemory(&m_vPos, sizeof(VECTOR3));
	ZeroMemory(&m_vRot, sizeof(VECTOR3));
	m_vScale.x = 1.0f;
	m_vScale.y = 1.0f;
	m_vScale.z = 1.0f;
	
	_bDead = false;

	_pParent = nullptr;

	m_pMesh = nullptr;
	m_pMaterial = nullptr;

	SceneManager::pCurrentScene->_listObj.push_back(this);
}

CObject::CObject(VECTOR3& pos, VECTOR3& rot, VECTOR3& scale)
	: _bDead(false), _pParent(nullptr), m_vPos(pos), m_vRot(rot), m_vScale(scale), m_pMesh(nullptr), m_pMaterial(nullptr)
{
	SceneManager::pCurrentScene->_listObj.push_back(this);
}

CObject::~CObject()
{
	auto iter = _childList.begin();
	while (iter != _childList.end())
	{
		delete (*iter);
		(*iter) = nullptr;
		_childList.erase(iter++);
	}
}


void CObject::BufferUpdate()
{
	m_pMaterial->UpdateConstantBuffer(GetWorldMatrix());
}


MATRIXA CObject::GetWorldMatrix()
{
	MATRIXA mPos, mRot, mScale;
	MATRIXA mWorld;

	mPos = DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&m_vPos));
	mRot = DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_vRot));
	mScale = DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&m_vScale));

	mWorld = mScale * mRot * mPos;

	if (_pParent != nullptr)
		mWorld *= _pParent->GetWorldMatrix();

	return mWorld;
}


void CObject::Initialize()
{

}



void CObject::Update()
{
	FOR_STL(_childList)
	{
		(*iter)->Update();
	}
}


void CObject::Render()
{
	if (m_pMesh != nullptr && m_pMaterial != nullptr)
	{
		BufferUpdate();

		m_pMesh->Render();
		m_pMaterial->Render();

		//±×¸®±â! Render a triangle ¡Ú
		_pDXDC->DrawIndexed(m_pMesh->m_indics.size() * 3, 0, 0);
	}

	FOR_STL(_childList)
	{
		(*iter)->Render();
	}
}


void CObject::Destroy()
{
	_bDead = true;
}


void CObject::Destroy(float time)
{

}


void CObject::ChangeMesh(CMesh* i_pMesh)
{
	m_pMesh = i_pMesh;
}


void CObject::ChangeMaterial(CMaterial* i_pMaterial)
{
	m_pMaterial = i_pMaterial;
}


void CObject::SetParent(CObject* i_pParent)
{
	if (_pParent != nullptr)
	{
		_pParent->_childList.remove(this);
		_pParent = nullptr;

		if (i_pParent == nullptr)
		{
			SceneManager::pCurrentScene->_listObj.push_back(this);
		}
	}

	_pParent = i_pParent;

	if (_pParent != nullptr)
	{
		_pParent->_childList.push_back(this);
		SceneManager::pCurrentScene->_listObj.remove(this);
	}
}


CObject* CObject::GetParent()
{
	return _pParent;
}

list<CObject*> CObject::GetChildren()
{
	return _childList;
}


CObject* CObject::Find(unsigned int id)
{
	FOR_STL(SceneManager::pCurrentScene->_listObj)
	{
		if ((*iter)->GetID() == id)
			return (*iter);
	}

	return nullptr;
}