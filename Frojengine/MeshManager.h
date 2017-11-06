#pragma once

#include "Frojengine.h"

class MeshManager
{
private:
	unordered_map<LPCWSTR, CMesh*> _meshMap;

public:
	MeshManager();
	~MeshManager();

	bool InsertMesh(LPCWSTR i_fileName);
	CMesh* GetMesh(LPCWSTR i_fileName);

	void DeleteMesh(LPCWSTR i_fileName);
	void Clear();
};