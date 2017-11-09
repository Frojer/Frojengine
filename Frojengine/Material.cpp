#include "Material.h"

unordered_map<UINT, CMaterial*> CMaterial::_mtrlMap;

CMaterial::CMaterial(CShader* _pShader)
	: m_diffuse(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)), m_ambient(VECTOR3(1.0f, 1.0f, 1.0f)), m_specular(VECTOR3(1.0f, 1.0f, 1.0f)), m_pShader(_pShader)
{
	if (_pShader == nullptr)
	{
		// Default Shader¸¦ »ğÀÔ
	}
	_mtrlMap[GetID()] = this;
}


CMaterial::~CMaterial()
{
	_mtrlMap.erase(GetID());

	m_pShader = nullptr;
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