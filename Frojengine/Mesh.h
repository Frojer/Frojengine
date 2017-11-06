#pragma once

#include "Frojengine.h"

class CMesh
{
protected:
	LPVERTEXBUFFER _pVB;
	LPINDEXBUFFER _pIB;

public:
	vector<VF_PNT>			m_verts;
	vector<unsigned int>	m_indics;

private:
	bool LoadObjFile(LPCWSTR i_fileName);
	bool LoadXFile(LPCWSTR i_fileName);

public:
	CMesh();
	CMesh(LPVERTEXBUFFER i_pVB, LPINDEXBUFFER i_pIB, vector<VF_PNT> i_verts, vector<unsigned int> i_indics);
	~CMesh();

	static CMesh* CreateMesh(LPCWSTR i_fileName);

	friend class MeshManager;
};