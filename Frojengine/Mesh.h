#pragma once

#include "Frojengine.h"

class CMesh
{
private:
	static LPDEVICE _pDevice;
	static LPDXDC _pDXDC;

protected:
	LPVERTEXBUFFER _pVB;
	LPINDEXBUFFER _pIB;

public:
	vector<VF_Pos>			m_verts;
	vector<unsigned int>	m_indics;

private:
	bool LoadObjFile(LPCWSTR i_fileName);
	bool LoadXFile(LPCWSTR i_fileName);
	void UpdateBuffer(const VECTOR3& pos, const VECTOR3& rot, const VECTOR3& scale);
	void Render();

public:
	CMesh();
	CMesh(LPVERTEXBUFFER i_pVB, LPINDEXBUFFER i_pIB, vector<VF_Pos> i_verts, vector<unsigned int> i_indics);
	~CMesh();

	static CMesh* CreateMesh(LPCWSTR i_fileName);

	friend class CObject;
	friend class FJSystemEngine;
};