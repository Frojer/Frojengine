#pragma once

#include "Frojengine.h"

class Component;
class Transform;
class Renderer;

class CObject : public IObject
{
private:
	// Dead Enable EnableCheck
	// 0    0      0           00000
	unsigned char _state;

	static list<CObject*> _dataList;

protected:
	CObject* _pParent;
	list<CObject*> _childList;
	list<Component*> _components;

public:
	Transform* m_pTransform;
	Renderer* m_pRenderer;

private:
	CObject(const CObject& obj) {}
	CObject& operator= (const CObject& obj) = delete;

	static CObject* FindChildList(unsigned int id, list<CObject*> childList);

	void StateUpdate();

protected:
	void Update();
	void AfterUpdate();
	void Render();
	CObject(bool isData);

public:
	CObject();
	CObject(VECTOR3& pos, VECTOR3& rot, VECTOR3& scale);
	virtual ~CObject();

	void Initialize();

	void Destroy();
	void Destroy(float time);

	void SetParent(CObject* i_pParent);
	CObject* GetParent();
	list<CObject*> GetChildren();

	template <typename T>
	T* AddComponent()
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

		return (T*)pCom;
	}

	Component* GetComponent(const type_info& type);
	list<Component*> GetComponents();

	void SetEnable(bool enable);
	bool GetEnable();

	static CObject* Find(unsigned int id);
	static CObject* FindModel(wstring name);
	static CObject* CopyObject(const CObject* origin, VECTOR3 pos);
	static CObject* CopyObject(const CObject* origin);

	friend class FJRenderingEngine;
	friend class FileLoader;
	friend class CScene;
};