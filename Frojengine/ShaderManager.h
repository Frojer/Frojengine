#pragma once

#include "Frojengine.h"

class ShaderManager
{
private:
	LPDEVICE _pDevice;

	unordered_map<LPCWSTR, CShader*> _shaderMap;

private:

public:
	ShaderManager(LPDEVICE i_pDevice);
	~ShaderManager();

	bool InsertShader(LPCWSTR i_fileName);
	bool GetShader(LPCWSTR i_fileName);

	void DeleteShader(LPCWSTR i_fileName);
	void Clear();
};