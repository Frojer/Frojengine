#include "Material.h"

unordered_map<UINT, CMaterial*> CMaterial::_mtrlMap;

CMaterial::CMaterial(CShader* _pShader)
	: m_diffuse(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)), m_ambient(VECTOR3(1.0f, 1.0f, 1.0f)), m_specular(VECTOR3(1.0f, 1.0f, 1.0f)), m_pShader(_pShader)
{
	ZeroMemory(m_pTexture, sizeof(m_pTexture));

	if (m_pShader == nullptr)
	{
		// Default Shader를 삽입
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
	m_pShader->_cbDefault.mtrlDiffuse = XMLoadFloat4(&m_diffuse);
	m_pShader->_cbDefault.mWorld = mWorld;
	m_pShader->_cbDefault.mWV = mWorld * m_pShader->_cbDefault.mView;

	m_pShader->UpdateDefaultBuffer();
}


void CMaterial::Render()
{
	if (m_pShader == nullptr)
		return;

	if (m_pTexture[0] != nullptr)
	{
		auto sampler = CTexture2D::GetSampler(m_pTexture[0]->m_AddressFilter);

		//셈플러 설정
		m_pShader->_pDXDC->PSSetSamplers(0, 1, &sampler);

		//셰이더 리소스 설정.
		m_pShader->_pDXDC->PSSetShaderResources(0, 1, &m_pTexture[0]->_ResourceView);	//PS 에 텍스처 설정
	}

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