#pragma once

#include "Frojengine.h"

class Component;
class Transform;
class Renderer;

class CObject : public IObject
{
private:
	bool _bDead;

protected:
	CObject* _pParent;
	list<CObject*> _childList;
	list<Component*> _components;

public:
	Transform* m_pTransform;
	Renderer* m_pRenderer;

protected:
	virtual void Update();
	void AfterUpdate();
	void Render();

public:
	CObject();
	CObject(VECTOR3& pos, VECTOR3& rot, VECTOR3& scale);
	~CObject();

	virtual void Initialize();

	void Destroy();
	void Destroy(float time);

	void SetParent(CObject* i_pParent);
	CObject* GetParent();
	list<CObject*> GetChildren();

	template <typename T>
	Component* AddComponent()
	{
		Component* pCom = nullptr;

		if (!is_base_of<Component, T>::value)
			return nullptr;

		if (is_base_of<Transform, T>::value)
		{
			if (m_pTransform == nullptr)
			{
				pCom = new T;
				m_pTransform = (Transform*)pCom;
			}
			else
				return nullptr;
		}

		else if (is_base_of<Renderer, T>::value)
		{
			if (m_pRenderer == nullptr)
			{
				pCom = new T;
				m_pRenderer = (Renderer*)pCom;
			}
			else
				return nullptr;
		}

		else
		{
			pCom = new T;
		}

		pCom->_pObj = this;
		_components.push_back(pCom);

		return pCom;
	}

	Component* GetComponent(wstring name);
	list<Component*> GetComponents(wstring name);

	static CObject* Find(unsigned int id);

	friend class FJRenderingEngine;
	friend class CScene;
};