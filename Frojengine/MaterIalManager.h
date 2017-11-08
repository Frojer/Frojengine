#pragma once

#include "Frojengine.h"

class MaterialManager
{
private:
	static unordered_map<LPCWSTR, CMaterial*> _mtrlMap;

private:
	MaterialManager() {}
	MaterialManager(const MaterialManager& obj) {}
	~MaterialManager() {}

	static void Clear();
	static bool InsertMaterial(CMaterial* i_pMtrl);
	static void DeleteMaterial(LPCWSTR i_fileName);

public:
	static CMaterial* GetMaterial(LPCWSTR i_fileName);

	friend class FJSystemEngine;
};