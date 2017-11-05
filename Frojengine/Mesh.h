#pragma once

#include "Frojengine.h"

class CMesh
{
protected:
	LPVERTEXBUFFER _pVB;
	LPINDEXBUFFER _pIB;

public:
	vector<FVF_PNT>			m_verts;
	vector<unsigned int>	m_indics;

private:
	bool LoadObjFile(LPCWSTR i_fileName);
	bool LoadXFile(LPCWSTR i_fileName);

public:
	CMesh();
	CMesh(LPVERTEXBUFFER i_pVB, LPINDEXBUFFER i_pIB, vector<FVF_PNT> i_verts, vector<unsigned int> i_indics);
	~CMesh();

	friend class MeshManager;
};