#pragma once

#include "Frojengine.h"

class CShader;

class ShaderManager
{
private:
	static unordered_map<LPCWSTR, CShader*> _shaderMap;
	ShaderManager() {}
	ShaderManager(const ShaderManager& obj) {}
	~ShaderManager() {}

private:
	static void Clear();
	static bool InsertShader(LPCWSTR i_fileName);
	static void DeleteShader(LPCWSTR i_fileName);

public:
	static CShader* GetShader(LPCWSTR i_fileName);

	friend class FJSystemEngine;
};