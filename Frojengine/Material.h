#pragma once

#include "Frojengine.h"

class CShader;

class CMaterial : public IObject
{
private:
	static unordered_map<UINT, CMaterial*> _mtrlMap;

public:
	CShader* m_pShader;

	VECTOR4 m_diffuse;
	VECTOR3 m_ambient;
	VECTOR3 m_specular;

private:
	static void ClearMap();
	void UpdateConstantBuffer(MATRIXA& mWorld);
	void Render();

public:
	CMaterial(CShader* _pShader);
	~CMaterial();

	static CMaterial* Find(UINT id);
	static CMaterial* Find(LPCWSTR name);

	friend class CObject;
	friend class FJSystemEngine;
};