#include "Texture.h"

//DirectXTK �� �ؽ�ó API ���
#include "DDSTextureLoader.h" 
#include "WICTextureLoader.h"

#include "FJUtility.h"

unordered_map<UINT, CTexture2D*> CTexture2D::_textureMap;
ID3D11SamplerState*	CTexture2D::_pSampler[ADDRESS_MAX];

CTexture2D::CTexture2D(LPCWSTR name)
	: m_vBorderColor(1.0f, 1.0f, 1.0f, 1.0f), _pResourceView(nullptr), m_AddressFilter(ADDRESS_WRAP)
{
	_textureMap.insert(pair<UINT, CTexture2D*>(GetID(), this));
	m_name = name;
}


CTexture2D::~CTexture2D()
{
	SAFE_RELEASE(_pResourceView);
}


bool CTexture2D::CreateTexture2D(LPCWSTR i_fileName)
{
	HRESULT hr;
	WCHAR extention[16];
	
	FileNameExtension(i_fileName, extention, ARRAYSIZE(extention));

	if (_wcsnicmp(extention, L".dds", ARRAYSIZE(extention)) == 0)
	{
		hr = DirectX::CreateDDSTextureFromFileEx(_pDevice, _pDXDC, i_fileName, 0,
			D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
			0, D3D11_RESOURCE_MISC_GENERATE_MIPS, false,
			nullptr, &_pResourceView);
		
		if (FAILED(hr))
		{
			hr = DirectX::CreateDDSTextureFromFile(_pDevice, i_fileName, nullptr, &_pResourceView);

			if (FAILED(hr))
			{
				//FJError(hr, L"�ؽ�ó �ε� ���� \nFile=%s", fileName);
				return false;
			}
		}
	}

	else if (_wcsnicmp(extention, L".png", ARRAYSIZE(extention)) == 0 ||
		_wcsnicmp(extention, L".jpg", ARRAYSIZE(extention)) == 0 ||
		_wcsnicmp(extention, L".jpeg", ARRAYSIZE(extention)) == 0 ||
		_wcsnicmp(extention, L".bmp", ARRAYSIZE(extention)) == 0)
	{
		// Mip-map �ؽ�ó ����
		hr = DirectX::CreateWICTextureFromFileEx(_pDevice, _pDXDC, i_fileName, 0,
			D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
			0, D3D11_RESOURCE_MISC_GENERATE_MIPS, WIC_LOADER_DEFAULT,
			nullptr, &_pResourceView);
		
		if (FAILED(hr))
		{
			hr = DirectX::CreateWICTextureFromFile(_pDevice, i_fileName, nullptr, &_pResourceView);

			if (FAILED(hr))
			{
				//FJError(hr, L"�ؽ�ó �ε� ���� \nFile=%s", fileName);
				return false;
			}
		}
	}

	return true;
}




////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
//
// �ؽ�ó ���÷� ���� ��ü ���� �Լ���.
//
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
// 
// �ؽ�ó ���÷� ���� ��ü ����.
//
void CTexture2D::SamplerCreate()
{
	HRESULT hr;

	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));

	//���� �⺻�� ó��..
	//sd.Filter = D3D11_FILTER_ANISOTROPIC;
	//sd.MaxAnisotropy = 1;	
	//sd.MinLOD = 0;							//�Ӹ���	
	//sd.MaxLOD = D3D11_FLOAT32_MAX;
	//sd.MipLODBias = 0;
	//sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	//sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	//sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	//sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
	//sd.BorderColor[0] = 1;
	//sd.BorderColor[1] = 1;
	//sd.BorderColor[2] = 1;
	//sd.BorderColor[3] = 1;	


	//�ؽ�ó ���� ����.
	//sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;				// ����Ʈ ���͸� (Point Filter)��
	//sd.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;		// ���/Ȯ��=��, �Ӹ�=��������
	//sd.Filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;  // ���=��, Ȯ��=����
	//sd.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;		// ���=��, Ȯ��=����, �Ӹ�=����
	//sd.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;		// ���=����, Ȯ��=��
	//sd.Filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR; // ���=����, Ȯ��=��, �Ӹ�=����

	// ���߼��� ���͸� (Bi-linear Filter)��(�Ӹ� �������)
	//sd.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;	  

	// ���߼��� ���͸� (Tril-linear Filter)(�⺻��)(�Ӹ� �����)��
	//sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;			  

	// ���� ���͸� (Anisotropic Filter)(�Ӹ������ʿ�)��
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = g_setting.anisotropy;

	//�Ӹ���	Mip-Mapping ����
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	//*/

	//�ؽ�ó ��巹�� ���. (�⺻��)
	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	hr = _pDevice->CreateSamplerState(&sd, &_pSampler[ADDRESS_CLAMP]);

	//�ؽ�ó ��巹�� ���. (Wrap)
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = _pDevice->CreateSamplerState(&sd, &_pSampler[ADDRESS_WRAP]);

	//�ؽ�ó ��巹�� ���. (BorderColor)(�⺻���� ���)
	sd.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	/*
	sd.BorderColor[0] = m_vBorderColor.x;
	sd.BorderColor[1] = m_vBorderColor.y;
	sd.BorderColor[2] = m_vBorderColor.z;
	sd.BorderColor[3] = m_vBorderColor.w;
	*/
	sd.BorderColor[0] = 1.0f;
	sd.BorderColor[1] = 1.0f;
	sd.BorderColor[2] = 1.0f;
	sd.BorderColor[3] = 1.0f;
	hr = _pDevice->CreateSamplerState(&sd, &_pSampler[ADDRESS_BORDER]);
}



// �ؽ�ó ���÷� ���� ��ü ����.
void CTexture2D::SamplerRelease()
{
	for (UINT i = 0; i < ADDRESS_MAX; i++)
		SAFE_RELEASE(_pSampler[i]);
}



CTexture2D* CTexture2D::Find(UINT id)
{
	if (_textureMap.find(id) == _textureMap.end())
		return nullptr;

	return _textureMap[id];
}


CTexture2D* CTexture2D::Find(LPCWSTR name)
{
	FOR_STL(_textureMap)
	{
		if (iter->second->m_name == name)
			return iter->second;
	}

	return nullptr;
}



ID3D11SamplerState* CTexture2D::GetSampler(UINT addressFilter)
{
	return _pSampler[addressFilter];
}