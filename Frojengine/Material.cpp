#include "Material.h"

unordered_map<UINT, CMaterial*> CMaterial::_mtrlMap;
CTexture2D* CMaterial::_pDefaultTex = nullptr;
CMaterial::WVP_Data CMaterial::_WVPData;
CMaterial::Light_Data CMaterial::_LightData[LIGHT_SIZE];

CMaterial::CMaterial(CShader* shader)
	: _pShader(shader), _countTexture(0), m_diffuse(VECTOR4(1.0f, 1.0f, 1.0f, 1.0f)), m_ambient(VECTOR3(0.2f, 0.2f, 0.2f)), m_specular(VECTOR3(1.0f, 1.0f, 1.0f))
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


void CMaterial::UpdateLightData()
{
	auto iter = Light::_lightList.begin();
	VECTOR3 vec;
	
	ZeroMemory(_LightData, sizeof(_LightData));

	for (UINT i = 0; iter != Light::_lightList.end() && i < LIGHT_SIZE; iter++)
	{
		while (!(*iter)->GetEnable()) continue;

		_LightData[i].diffuse = XMLoadFloat4(&(*iter)->m_diffuse);
		_LightData[i].ambient = XMLoadFloat4(&(*iter)->m_ambient);
		_LightData[i].specular = XMLoadFloat4(&(*iter)->m_specular);
		_LightData[i].position = XMLoadFloat3(&(*iter)->GetMyObject()->m_pTransform->GetPositionWorld());
		vec = (*iter)->GetMyObject()->m_pTransform->m_vRot;
		_LightData[i].direction = XMVector3Transform(XMLoadFloat3(&VECTOR3(0.0f, 0.0f, 1.0f)), XMMatrixRotationRollPitchYaw(vec.x, vec.y, vec.z));
		_LightData[i].range = (*iter)->m_range;
		_LightData[i].lightType = (*iter)->m_lightType;
		_LightData[i].useLight = true;

		++i;
	}
}


void CMaterial::UpdateConstantBuffer(MATRIXA& mWorld)
{
	_WVPData.mWorld = mWorld;
	_WVPData.mWV = mWorld * _WVPData.mView;

	if (_constData.size() > 0)
	{
		if (_vecMatrix.size() > 0)
			memcpy_s(&_constData[0], sizeof(MATRIXA) * _vecMatrix.size(), &_vecMatrix[0], sizeof(MATRIXA) * _vecMatrix.size());
		if (_vecVector.size() > 0)
			memcpy_s(&_constData[_vecMatrix.size() * 4], sizeof(VECTOR) * _vecVector.size(), &_vecVector[0], sizeof(VECTOR) * _vecVector.size());
		if (_vecScalarA.size() > 0)
			memcpy_s(&_constData[(_vecMatrix.size() * 4) + _vecVector.size()], sizeof(VECTOR) * _vecScalarA.size(), &_vecScalarA[0], sizeof(VECTOR) * _vecScalarA.size());

		_pShader->UpdateConstantBuffer(&_constData[0], _constData.size() * sizeof(VECTOR));
	}

	else
		_pShader->UpdateConstantBuffer(nullptr, 0);
}


void CMaterial::Render()
{
	if (_pShader == nullptr)
		return;

	auto sampler = CTexture2D::GetSampler(1);

	// ¼ÀÇÃ·¯ ¼³Á¤
	_pShader->_pDXDC->PSSetSamplers(0, 1, &sampler);

	for (UINT i = 0; i < _countTexture; i++)
	{
		// ¼ÎÀÌ´õ ¸®¼Ò½º ¼³Á¤.
		if (m_pTexture[i] == nullptr)
			_pShader->_pDXDC->PSSetShaderResources(i, 1, &_pDefaultTex->_pResourceView);
		else
			_pShader->_pDXDC->PSSetShaderResources(i, 1, &m_pTexture[i]->_pResourceView);
	}

	_pShader->Render();
}



void CMaterial::SetShader(CShader* shader)
{
	UINT totalSize = 0;
	UINT scalarSize = 0;

	if (_pShader == nullptr)
	{
		// Default Shader¸¦ »ðÀÔ
		_pShader = CShader::Find(L"Error");
	}

	else _pShader = shader;

	scalarSize = (_pShader->_countScalar / 4) + (_pShader->_countScalar % 4 == 0 ? 0 : 1);

	_countTexture = _pShader->_countTexture;
	totalSize += _pShader->_countMatrix * 4;
	totalSize += _pShader->_countVector;
	totalSize += scalarSize;

	_constData.resize(totalSize);
	_vecScalar.resize(scalarSize);
	_vecScalarA.resize(scalarSize);
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
}



CShader* CMaterial::GetShader()
{
	return _pShader;
}



void CMaterial::SetScalar(UINT id, float scalar)
{
	/*VECTOR vec;
	memcpy(&vec, &scala, sizeof(VECTOR));
	_vecScala[id] = vec;*/
	*((float*)&_vecScalar[id / 4] + (id % 4)) = scalar;

	for (UINT i = 0; i < _vecScalarA.size(); i++)
	{
		_vecScalarA[i] = XMLoadFloat4(&_vecScalar[i]);
	}
}



void CMaterial::SetVector(UINT id, VECTOR4& vector)
{
	_vecVector[id] = XMLoadFloat4(&vector);
}



void CMaterial::SetMatrix(UINT id, MATRIX& matrix)
{
	_vecMatrix[id] = XMLoadFloat4x4(&matrix);
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