#pragma once

#include "Frojengine.h"

class CMesh;
class CMaterial;

class CModel
{
private:

public:
	CMesh*		m_mesh;
	CMaterial*	m_material;

private:

public:
	CModel();
	~CModel();
};