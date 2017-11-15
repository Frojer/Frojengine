#pragma once

#include "Frojengine.h"

class CObject;

class CMesh : public IObject
{
private:
	static unordered_map<UINT, CMesh*> _meshMap;

	static LPDEVICE _pDevice;
	static LPDXDC _pDXDC;

protected:
	LPVERTEXBUFFER _pVB;
	LPINDEXBUFFER _pIB;

public:
	vector<VF_PNT>			m_verts;
	vector<IndexFormat>		m_indics;

private:
	bool CreateBuffer(vector<VF_PNT>& i_vertics, vector<IndexFormat>& i_indics);

	MATRIXA GetWorldMatrix(const VECTOR3& pos, const VECTOR3& rot, const VECTOR3& scale);
	void Render();

	static void ClearMap();

public:
	CMesh();
	CMesh(vector<VF_PNT> i_verts, vector<IndexFormat> i_indics);
	~CMesh();

	static CMesh* Find(UINT id);
	static CMesh* Find(wstring name);

	friend class CObject;
	friend class FJRenderingEngine;
	friend class FJSystemEngine;
	friend class FileLoader;
};