#include "Texture.h"

//DirectXTK 의 텍스처 API 사용
#include "DDSTextureLoader.h" 
#include "WICTextureLoader.h"

#include "FJUtility.h"

unordered_map<UINT, CTexture2D*> CTexture2D::_textureMap;
LPDEVICE CTexture2D::_pDevice = nullptr;
LPDXDC CTexture2D::_pDXDC = nullptr;
ID3D11SamplerState*	CTexture2D::_pSampler[ADDRESS_MAX];

CTexture2D::CTexture2D(LPCWSTR i_fileName)
	: m_vBorderColor(1.0f, 1.0f, 1.0f, 1.0f), _ResourceView(nullptr), m_AddressFilter(ADDRESS_WRAP)
{
	_textureMap.insert(pair<UINT, CTexture2D*>(GetID(), this));
	m_name = i_fileName;
}


CTexture2D::~CTexture2D()
{
	SAFE_RELEASE(_ResourceView);
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
			nullptr, &_ResourceView);
		
		if (FAILED(hr))
		{
			//FJError(hr, L"텍스처 로드 실패 \nFile=%s", fileName);
			return false;
		}
	}

	else if (_wcsnicmp(extention, L".png", ARRAYSIZE(extention)) == 0 ||
		_wcsnicmp(extention, L".jpg", ARRAYSIZE(extention)) == 0 ||
		_wcsnicmp(extention, L".jpeg", ARRAYSIZE(extention)) == 0 ||
		_wcsnicmp(extention, L".bmp", ARRAYSIZE(extention)) == 0)
	{
		// Mip-map 텍스처 생성
		hr = DirectX::CreateWICTextureFromFileEx(_pDevice, _pDXDC, i_fileName, 0,
			D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
			0, D3D11_RESOURCE_MISC_GENERATE_MIPS, WIC_LOADER_DEFAULT,
			nullptr, &_ResourceView);
		
		if (FAILED(hr))
		{
			//FJError(hr, L"텍스처 로드 실패 \nFile=%s", fileName);
			return false;
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
// 텍스처 셈플러 상태 객체 관련 함수들.
//
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
// 
// 텍스처 셈플러 상태 객체 생성.
//
void CTexture2D::SamplerCreate()
{
	HRESULT hr;

	D3D11_SAMPLER_DESC sd;
	ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));

	//이하 기본값 처리..
	//sd.Filter = D3D11_FILTER_ANISOTROPIC;
	//sd.MaxAnisotropy = 1;	
	//sd.MinLOD = 0;							//밉멥핑	
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


	//텍스처 필터 결정.
	//sd.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;				// 포인트 필터링 (Point Filter)★
	//sd.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;		// 축소/확대=점, 밉멥=선형필터
	//sd.Filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;  // 축소=점, 확대=선형
	//sd.Filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;		// 축소=점, 확대=선형, 밉멥=선형
	//sd.Filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;		// 축소=선형, 확대=점
	//sd.Filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR; // 축소=선형, 확대=점, 밉멥=선형

	// 이중선형 필터링 (Bi-linear Filter)★(밉멥 미적용시)
	//sd.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;	  

	// 삼중선형 필터링 (Tril-linear Filter)(기본값)(밉멥 적용시)★
	//sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;			  

	// 비등방 필터링 (Anisotropic Filter)(밉멥적용필요)★
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = g_setting.anisotropy;

	//밉멥핑	Mip-Mapping 설정
	sd.MinLOD = 0;
	sd.MaxLOD = D3D11_FLOAT32_MAX;
	sd.MipLODBias = 0;
	//*/

	//텍스처 어드레스 모드. (기본값)
	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	hr = _pDevice->CreateSamplerState(&sd, &_pSampler[ADDRESS_CLAMP]);

	//텍스처 어드레스 모드. (Wrap)
	sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	hr = _pDevice->CreateSamplerState(&sd, &_pSampler[ADDRESS_WRAP]);

	//텍스처 어드레스 모드. (BorderColor)(기본색은 흰색)
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



// 텍스처 셈플러 상태 객체 제거.
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