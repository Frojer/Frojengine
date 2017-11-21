#include "Object.h"

CObject::CObject()
	: _bDead(false), _pParent(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr)
{
	AddComponent<Transform>();

	SceneManager::pCurrentScene->_listObj.push_back(this);
}

CObject::CObject(VECTOR3& pos, VECTOR3& rot, VECTOR3& scale)
	: _bDead(false), _pParent(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr)
{
	AddComponent<Transform>();

	m_pTransform->m_vPos = pos;
	m_pTransform->m_vRot = rot;
	m_pTransform->m_vScale = scale;
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


void CObject::Initialize()
{

}



void CObject::Update()
{
	FOR_STL(_components)
	{
		if (CheckComponentType((*iter)->_type, COMPONENT_TYPE_UPDATE))
		{
			(*iter)->Update();
		}
	}

	FOR_STL(_childList)
	{
		(*iter)->Update();
	}
}



void CObject::AfterUpdate()
{
	FOR_STL(_components)
	{
		if (CheckComponentType((*iter)->_type, COMPONENT_TYPE_AFTERUPDATE))
		{
			(*iter)->AfterUpdate();
		}
	}

	FOR_STL(_childList)
	{
		(*iter)->AfterUpdate();
	}
}


void CObject::Render()
{
	FOR_STL(_components)
	{
		if (CheckComponentType((*iter)->_type, COMPONENT_TYPE_RENDER))
		{
			(*iter)->Render();
		}
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


Component* CObject::GetComponent(wstring name)
{
	FOR_STL(_components)
	{
		if ((*iter)->m_name == name)
			return (*iter);
	}

	return nullptr;
}



list<Component*> CObject::GetComponents(wstring name)
{
	return _components;
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