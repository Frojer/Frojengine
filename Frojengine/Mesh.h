#pragma once

#include "Frojengine.h"

class CMesh : public IObject, protected Device
{
private:
	static unordered_map<UINT, CMesh*> _meshMap;

protected:
	LPVERTEXBUFFER _pVB;
	LPINDEXBUFFER _pIB;

public:
	vector<VF_PNT>			m_verts;
	vector<IndexFormat>		m_indics;

private:
	bool CreateBuffer(vector<VF_PNT>& i_vertics, vector<IndexFormat>& i_indics);

	void Render();

	static void ClearMap();

public:
	CMesh();
	CMesh(vector<VF_PNT> i_verts, vector<IndexFormat> i_indics);
	~CMesh();

	static CMesh* Find(UINT id);
	static CMesh* Find(wstring name);

	// void Renderer::Render()
	friend class Renderer;
	// FJSystemEngine::~FJSystemEngine()
	friend class FJSystemEngine;

	friend class FileLoader;
};