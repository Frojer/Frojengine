#include "ShaderManager.h"

ShaderManager::ShaderManager(LPDEVICE i_pDevice)
	: _pDevice(i_pDevice)
{

}


ShaderManager::~ShaderManager()
{
	Clear();
}


bool ShaderManager::InsertShader(LPCWSTR i_fileName)
{
	bool result;
	CShader* pShader = new CShader;

	if (pShader == nullptr)
	{
		ErrMsgBox(L"Failed to Memory Allocate");
		return false;
	}

	result = pShader->CreateShader(i_fileName, _pDevice);

	if (!result)
	{
		delete pShader;
		pShader = nullptr;

		return false;
	}

	_shaderMap.insert(pair<LPCWSTR, CShader*>(i_fileName, pShader));

	return true;
}


bool ShaderManager::GetShader(LPCWSTR i_fileName)
{
	bool result = true;

	if (_shaderMap.find(i_fileName) == _shaderMap.end())
	{
		result = InsertShader(i_fileName);
	}

	return result;
}


void ShaderManager::DeleteShader(LPCWSTR i_fileName)
{
	delete _shaderMap[i_fileName];
	_shaderMap[i_fileName] = nullptr;

	_shaderMap.erase(i_fileName);
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