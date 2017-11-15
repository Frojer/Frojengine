#include "Material.h"

unordered_map<UINT, CMaterial*> CMaterial::_mtrlMap;

CMaterial::CMaterial(CShader* _pShader)
	: m_diffuse(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)), m_ambient(VECTOR3(1.0f, 1.0f, 1.0f)), m_specular(VECTOR3(1.0f, 1.0f, 1.0f)), m_pShader(_pShader)
{
	if (m_pShader == nullptr)
	{
		// Default Shader¸¦ »ðÀÔ
		m_pShader = CShader::Find(L"Default");
	}
	_mtrlMap[GetID()] = this;
}


CMaterial::~CMaterial()
{
	_mtrlMap.erase(GetID());

	m_pShader = nullptr;
}


void CMaterial::ClearMap()
{
	auto i = _mtrlMap.begin();

	while (i != _mtrlMap.end())
		delete (i++)->second;
}


void CMaterial::UpdateConstantBuffer(MATRIXA& mWorld)
{
	m_pShader->UpdateDefaultBuffer(mWorld);
}


void CMaterial::Render()
{
	if (m_pShader == nullptr)
		return;

	m_pShader->Render();
}


CMaterial* CMaterial::Find(UINT id)
{
	if (_mtrlMap.find(id) == _mtrlMap.end())
		return nullptr;

	return _mtrlMap[id];
}


CMaterial* CMaterial::Find(LPCWSTR name)
{
	FOR_STL(_mtrlMap)
	{
		if (iter->second->m_name == name)
			return iter->second;
	}

	return nullptr;
}