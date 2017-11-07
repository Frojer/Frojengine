#include "ShaderManager.h"

unordered_map<LPCWSTR, CShader*> ShaderManager::_shaderMap;

bool ShaderManager::InsertShader(LPCWSTR i_fileName)
{
	CShader* pShader = nullptr;
	
	if (_shaderMap.find(i_fileName) != _shaderMap.end())
		return false;

	pShader = CShader::CreateShader(i_fileName);

	if (pShader == nullptr)
		return false;

	_shaderMap.insert(pair<LPCWSTR, CShader*>(i_fileName, pShader));

	return true;
}


CShader* ShaderManager::GetShader(LPCWSTR i_fileName)
{
	bool result = true;

	if (_shaderMap.find(i_fileName) == _shaderMap.end())
	{
		result = InsertShader(i_fileName);
	}

	return _shaderMap[i_fileName];
}


void ShaderManager::DeleteShader(LPCWSTR i_fileName)
{
	if (_shaderMap.find(i_fileName) != _shaderMap.end())
	{
		delete _shaderMap[i_fileName];
		_shaderMap[i_fileName] = nullptr;

		_shaderMap.erase(i_fileName);
	}
}


void ShaderManager::Clear()
{
	auto i = _shaderMap.begin();

	while (i != _shaderMap.end())
	{
		delete i->second;
		i->second = nullptr;
		_shaderMap.erase(i++);
	}
}