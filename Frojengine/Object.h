#pragma once

#include "Frojengine.h"

class CModel;
class CMesh;
class CMaterial;

class CObject : public IObject
{
private:
	static LPDXDC _pDXDC;
	bool _bDead;

protected:
	CObject* _pParent;
	list<CObject*> _childList;

public:
	VECTOR3 m_vPos;
	VECTOR3 m_vRot;
	VECTOR3 m_vScale;

	CMesh*		m_pMesh;
	CMaterial*	m_pMaterial;

private:
	void BufferUpdate();
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

	void ChangeMesh(CMesh* i_pMesh);
	void ChangeMaterial(CMaterial* i_pMaterial);

	static CObject* Find(unsigned int id);

	friend class FJRenderingEngine;
	friend class CScene;
};