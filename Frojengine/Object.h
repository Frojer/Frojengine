#pragma once

#include "Frojengine.h"

class Component;

class CObject : public IObject
{
private:
	bool _bDead;

protected:
	CObject* _pParent;
	list<CObject*> _childList;
	list<Component*> _components;

public:
	VECTOR3 m_vPos;
	VECTOR3 m_vRot;
	VECTOR3 m_vScale;

private:
	MATRIXA GetWorldMatrix();
	void Render();

public:
	CObject();
	CObject(VECTOR3& pos, VECTOR3& rot, VECTOR3& scale);
	~CObject();

	virtual void Initialize();

	virtual void Update();

	void Destroy();
	void Destroy(float time);

	void SetParent(CObject* i_pParent);
	CObject* GetParent();
	list<CObject*> GetChildren();

	bool AddComponent(wstring name);
	Component* GetComponent(wstring name);
	list<Component*> GetComponents(wstring name);

	static CObject* Find(unsigned int id);

	friend class FJRenderingEngine;
	friend class CScene;
	// Renderer::BufferUpdate()
	friend class Renderer;
};