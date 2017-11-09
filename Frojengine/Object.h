#pragma once

#include "Frojengine.h"

class CModel;
class CMesh;
class CMaterial;

class CObject : public IObject
{
private:
	static list<CObject*> _objList;
	static LPDXDC _pDXDC;

protected:
	CObject* _parent;
	list<CObject*> _childList;

public:
	VECTOR3 m_vPos;
	VECTOR3 m_vRot;
	VECTOR3 m_vScale;

	CModel* m_pModel;

private:
	void BufferUpdate();
	void Render();

public:
	CObject();
	CObject(CModel* pModel, VECTOR3& pos = VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3& rot = VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3& scale = VECTOR3(0.0f, 0.0f, 0.0f));
	CObject(VECTOR3& pos, VECTOR3& rot, VECTOR3& scale);
	~CObject();

	virtual void Initialize() = 0;

	virtual void Update() = 0;

	void ChangeMesh(CMesh* i_pMesh);
	void ChangeMaterial(CMaterial* i_pMaterial);

	static CObject* Find(unsigned int id);

	friend class FJSystemEngine;
	friend class CScene;
};