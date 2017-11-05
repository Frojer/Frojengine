#pragma once

#include "Frojengine.h"

class CMaterial
{
private:

public:
	VECTOR4 m_diffuse;
	VECTOR3 m_ambient;
	VECTOR3 m_specular;

private:

public:
	CMaterial();
	~CMaterial();
};