#include "Material.h"

CMaterial::CMaterial(LPCWSTR i_name)
	: m_diffuse(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)), m_ambient(VECTOR3(1.0f, 1.0f, 1.0f)), m_specular(VECTOR3(1.0f, 1.0f, 1.0f)), m_pShader(nullptr)
{

}


CMaterial::CMaterial(CShader* _pShader)
	: m_diffuse(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)), m_ambient(VECTOR3(1.0f, 1.0f, 1.0f)), m_specular(VECTOR3(1.0f, 1.0f, 1.0f)), m_pShader(_pShader)
{

}


CMaterial::~CMaterial()
{
	m_pShader = nullptr;
}


void CMaterial::Render()
{
	if (m_pShader == nullptr)
		return;

	m_pShader->Render();
}