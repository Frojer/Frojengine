#pragma once

#include "Frojengine.h"

class CMesh;
class CMaterial;

class CModel
{
private:

public:
	CMesh*		m_pMesh;
	CMaterial*	m_pMaterial;

private:

public:
	CModel();
	CModel(CMesh* i_pMesh, CMaterial* i_pMaterial);
	~CModel();
};