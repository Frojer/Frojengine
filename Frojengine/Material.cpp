#include "Material.h"

unordered_map<UINT, CMaterial*> CMaterial::_mtrlMap;
CMaterial::WVP_Data CMaterial::_WVPData;
CMaterial::Light_Data CMaterial::_LightData;

CMaterial::CMaterial(CShader* shader)
	: _pShader(shader), _countTexture(0), _useLight(false), m_diffuse(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)), m_ambient(VECTOR3(1.0f, 1.0f, 1.0f)), m_specular(VECTOR3(1.0f, 1.0f, 1.0f))
{
	ZeroMemory(m_pTexture, sizeof(m_pTexture));

	SetShader(shader);

	_mtrlMap[GetID()] = this;
}


CMaterial::~CMaterial()
{
	_mtrlMap.erase(GetID());

	_pShader = nullptr;
}


void CMaterial::ClearMap()
{
	auto i = _mtrlMap.begin();

	while (i != _mtrlMap.end())
		delete (i++)->second;
}


void CMaterial::UpdateConstantBuffer(MATRIXA& mWorld)
{
	_WVPData.mWorld = mWorld;
	_WVPData.mWV = mWorld * _WVPData.mView;

	if (_constData.size() > 0)
	{
		if (_vecMatrix.size() > 0)
			memcpy_s(&_vecMatrix[0], sizeof(MATRIXA) * _vecMatrix.size(), &_constData[0], sizeof(MATRIXA) * _vecMatrix.size());
		if (_vecVector.size() > 0)
			memcpy_s(&_vecVector[0], sizeof(VECTOR) * _vecVector.size(), &_constData[_vecMatrix.size() * 4], sizeof(VECTOR) * _vecVector.size());
		if (_vecScala.size() > 0)
			memcpy_s(&_vecScala[0], sizeof(VECTOR) * _vecScala.size(), &_constData[(_vecMatrix.size() * 4) + _vecVector.size()], sizeof(VECTOR) * _vecScala.size());

		_pShader->UpdateConstantBuffer(&_constData[0], _constData.size() * sizeof(float));
	}

	else
		_pShader->UpdateConstantBuffer(nullptr, 0);
}


void CMaterial::Render()
{
	if (_pShader == nullptr)
		return;

	LPRESOURCEVIEW lpNullSV = nullptr;
	auto sampler = CTexture2D::GetSampler(1);

	// ���÷� ����
	_pShader->_pDXDC->PSSetSamplers(0, 1, &sampler);

	for (UINT i = 0; i < _countTexture; i++)
	{
		// ���̴� ���ҽ� ����.
		if (m_pTexture[i] == nullptr)
			_pShader->_pDXDC->PSSetShaderResources(i, 1, &lpNullSV);
		else
			_pShader->_pDXDC->PSSetShaderResources(i, 1, &m_pTexture[i]->_ResourceView);
	}

	_pShader->Render();
}



void CMaterial::SetShader(CShader* shader)
{
	UINT totalSize = 0;

	if (_pShader == nullptr)
	{
		// Default Shader�� ����
		_pShader = CShader::Find(L"Error");
	}

	else _pShader = shader;

	_countTexture = _pShader->_countTexture;
	totalSize += _pShader->_countMatrix * 4;
	totalSize += _pShader->_countVector + _pShader->_countScalar;
	_constData.resize(totalSize);
	_vecScala.resize(_pShader->_countScalar);
	_vecVector.resize(_pShader->_countVector);
	_vecMatrix.resize(_pShader->_countMatrix);

	/*if (totalSize != 0)
	{
		_constData.resize(totalSize);

		if (_pShader->_countScalar != 0)
		{
			_vecScala.resize(_pShader->_countScalar);
			ZeroMemory(&_vecScala[0], _vecScala.size() * sizeof(VECTOR));
		}

		if (_pShader->_countVector != 0)
		{
			_vecVector.resize(_pShader->_countVector);
			ZeroMemory(&_vecVector[0], _vecVector.size() * sizeof(VECTOR));
		}

		if (_pShader->_countMatrix != 0)
		{
			_vecMatrix.resize(_pShader->_countMatrix);
			ZeroMemory(&_vecMatrix[0], _vecMatrix.size() * sizeof(MATRIXA));
		}
	}*/

	_useLight = _pShader->_useLight;
}



CShader* CMaterial::GetShader()
{
	return _pShader;
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