#include "Object.h"

#include <typeinfo>
#include "CustomComponent.h"

list<CObject*> CObject::_dataList;

CObject::CObject(bool isData)
	: IObject(isData), _state(0x60), _pParent(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr)
{
	AddComponent<Transform>();
}

CObject::CObject()
	: _state(0x60), _pParent(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr)
{
	AddComponent<Transform>();

	SceneManager::pCurrentScene->_listObj.push_back(this);
}

CObject::CObject(VECTOR3& pos, VECTOR3& rot, VECTOR3& scale)
	: _state(0x60), _pParent(nullptr), m_pTransform(nullptr), m_pRenderer(nullptr)
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

	auto iter2 = _components.begin();
	while (iter2 != _components.end())
	{
		delete (*iter2);
		(*iter2) = nullptr;
		_components.erase(iter2++);
	}
}


void CObject::StateUpdate()
{
	// Enable State Check
	_state &= 0xBF;
	_state |= ((_state & 0x20) == 0x20) ? 0x40 : 0x00;

	FOR_STL(_childList)
	{
		(*iter)->Initialize();
	}
}


void CObject::Initialize()
{
	if (!GetEnable())
		return;
	
	FOR_STL(_components)
	{
		if (!IsInitialize((*iter)->_check))
		{
			(*iter)->Initialize();
			(*iter)->_check |= 0x80;
		}
	}

	FOR_STL(_childList)
	{
		(*iter)->Initialize();
	}
}



void CObject::Update()
{
	if (!GetEnable())
		return;

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
	if (!GetEnable())
		return;

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
	if (!GetEnable())
		return;

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
	_state |= 0x80;
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

		if (i_pParent == nullptr && !_isData)
		{
			SceneManager::pCurrentScene->_listObj.push_back(this);
		}
	}

	_pParent = i_pParent;

	if (_pParent != nullptr)
	{
		_pParent->_childList.push_back(this);

		if (!_isData)
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


Component* CObject::GetComponent(const type_info& type)
{
	FOR_STL(_components)
	{
		if (typeid(*(*iter)) == type)
			return (*iter);
	}

	return nullptr;
}



list<Component*> CObject::GetComponents()
{
	return _components;
}


void CObject::SetEnable(bool enable)
{
	_state &= 0xDF;	
	_state |= (enable ? 0x20 : 0x00);
}


bool CObject::GetEnable()
{
	return ((_state & 0x40) == 0x40);
}


CObject* CObject::Find(unsigned int id)
{
	CObject* obj = nullptr;

	FOR_STL(SceneManager::pCurrentScene->_listObj)
	{
		if ((*iter)->GetID() == id)
			return (*iter);

		obj = FindChildList(id, (*iter)->_childList);

		if (obj != nullptr)
			return obj;
	}

	return nullptr;
}
CObject* CObject::FindChildList(unsigned int id, list<CObject*> childList)
{
	CObject* obj = nullptr;

	FOR_STL(childList)
	{
		if ((*iter)->GetID() == id)
			return (*iter);

		obj = FindChildList(id, (*iter)->_childList);

		if (obj != nullptr)
			return obj;
	}

	return nullptr;
}

CObject* CObject::FindModel(wstring name)
{
	FOR_STL(_dataList)
	{
		if ((*iter)->m_name == name)
			return (*iter);
	}

	return nullptr;
}


CObject* CObject::CopyObject(const CObject* origin, VECTOR3 pos)
{
	CObject* copy = CopyObject(origin);
	copy->m_pTransform->SetPositionLocal(pos);

	return copy;
}


//////////////////////////////////////////////////////////////
//
//  현재 새로운 컴포넌트는 이곳에 수동적으로 추가해줘야합니다
//
//////////////////////////////////////////////////////////////
CObject* CObject::CopyObject(const CObject* origin)
{
	CObject* obj = new CObject();
	obj->m_name = origin->m_name;

	Component* cp;

	FOR_STL(origin->_components)
	{
#define COPY_SIZE(custom) sizeof(custom) - sizeof(Component)
		const type_info& ti = typeid(*(*iter));

		if (ti == typeid(Transform))
		{
			cp = obj->m_pTransform;
			memcpy_s((char*)cp + sizeof(Component), COPY_SIZE(Transform), (char*)(*iter) + sizeof(Component), COPY_SIZE(Transform));
		}

		else if (ti == typeid(Renderer))
		{
			cp = obj->AddComponent<Renderer>();
			memcpy_s((char*)cp + sizeof(Component), COPY_SIZE(Renderer), (char*)(*iter) + sizeof(Component), COPY_SIZE(Renderer));
		}

		else if (ti == typeid(Light))
		{
			cp = obj->AddComponent<Light>();
			memcpy_s((char*)cp + sizeof(Component), COPY_SIZE(Light), (char*)(*iter) + sizeof(Component), COPY_SIZE(Light));
		}

		else if (ti == typeid(Camera))
		{
			cp = obj->AddComponent<Camera>();
			memcpy_s((char*)cp + sizeof(Component), COPY_SIZE(Camera), (char*)(*iter) + sizeof(Component), COPY_SIZE(Camera));
		}

		else if (ti == typeid(CameraControl))
		{
			cp = obj->AddComponent<CameraControl>();
			memcpy_s((char*)cp + sizeof(Component), COPY_SIZE(CameraControl), (char*)(*iter) + sizeof(Component), COPY_SIZE(CameraControl));
		}

		else if (ti == typeid(Hero))
		{
			cp = obj->AddComponent<Hero>();
			memcpy_s((char*)cp + sizeof(Component), COPY_SIZE(Hero), (char*)(*iter) + sizeof(Component), COPY_SIZE(Hero));
		}

		else if (ti == typeid(Plane))
		{
			cp = obj->AddComponent<Plane>();
			memcpy_s((char*)cp + sizeof(Component), COPY_SIZE(Plane), (char*)(*iter) + sizeof(Component), COPY_SIZE(Plane));
		}

		else if (ti == typeid(System))
		{
			cp = obj->AddComponent<System>();
			memcpy_s((char*)cp + sizeof(Component), COPY_SIZE(System), (char*)(*iter) + sizeof(Component), COPY_SIZE(System));
		}

		else if (ti == typeid(Windmill))
		{
			cp = obj->AddComponent<Windmill>();
			memcpy_s((char*)cp + sizeof(Component), COPY_SIZE(Windmill), (char*)(*iter) + sizeof(Component), COPY_SIZE(Windmill));
		}

		else if (ti == typeid(TripleWindmill))
		{
			cp = obj->AddComponent<TripleWindmill>();
			memcpy_s((char*)cp + sizeof(Component), COPY_SIZE(TripleWindmill), (char*)(*iter) + sizeof(Component), COPY_SIZE(TripleWindmill));
		}

		else if (ti == typeid(TripleWindmill2))
		{
			cp = obj->AddComponent<TripleWindmill2>();
			memcpy_s((char*)cp + sizeof(Component), COPY_SIZE(TripleWindmill2), (char*)(*iter) + sizeof(Component), COPY_SIZE(TripleWindmill2));
		}

		else if (ti == typeid(MirrorScript))
		{
			cp = obj->AddComponent<MirrorScript>();
			memcpy_s((char*)cp + sizeof(Component), COPY_SIZE(MirrorScript), (char*)(*iter) + sizeof(Component), COPY_SIZE(MirrorScript));
		}

		else if (ti == typeid(AWSystem))
		{
			cp = obj->AddComponent<AWSystem>();
			memcpy_s((char*)cp + sizeof(Component), COPY_SIZE(AWSystem), (char*)(*iter) + sizeof(Component), COPY_SIZE(AWSystem));
		}
	}

	FOR_STL(origin->_childList)
	{
		CopyObject((*iter))->SetParent(obj);
	}

	return obj;
}