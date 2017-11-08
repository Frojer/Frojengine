#pragma once

#include "Frojengine.h"

class MeshManager
{
private:
	static unordered_map<LPCWSTR, CMesh*> _meshMap;
	
private:
	MeshManager() {}
	MeshManager(const MeshManager& obj) {}
	~MeshManager() {}

	static void Clear();
	static void DeleteMesh(LPCWSTR i_fileName);

public:
	static bool InsertMesh(LPCWSTR i_fileName);
	static CMesh* GetMesh(LPCWSTR i_fileName);

	friend class FJSystemEngine;
};