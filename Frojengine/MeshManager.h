#pragma once

#include "Frojengine.h"

class MeshManager
{
private:
	static unordered_map<LPCWSTR, CMesh*> _meshMap;
	MeshManager();

public:
	~MeshManager();

	static bool InsertMesh(LPCWSTR i_fileName);
	static CMesh* GetMesh(LPCWSTR i_fileName);

	static void DeleteMesh(LPCWSTR i_fileName);
	static void Clear();
};