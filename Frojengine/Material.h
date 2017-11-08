#pragma once

#include "Frojengine.h"

class CShader;

class CMaterial
{
private:

public:
	CShader* m_pShader;

	VECTOR4 m_diffuse;
	VECTOR3 m_ambient;
	VECTOR3 m_specular;

private:
	void Render();

public:
	CMaterial(LPCWSTR i_name);
	CMaterial(CShader* _pShader);
	~CMaterial();

	friend class CObject;
};