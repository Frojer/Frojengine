#include "Texture.h"

//DirectXTK 의 텍스처 API 사용
#include "DDSTextureLoader.h" 
#include "WICTextureLoader.h"

#include "FJUtility.h"

unordered_map<UINT, CTexture2D*> CTexture2D::_textureMap;
LPDEVICE CTexture2D::_pDevice = nullptr;
LPDXDC CTexture2D::_pDXDC = nullptr;

CTexture2D::CTexture2D(LPCWSTR i_fileName)
{
	_textureMap.insert(pair<UINT, CTexture2D*>(GetID(), this));
	m_name = i_fileName;
}


CTexture2D::~CTexture2D()
{

}


bool CTexture2D::CreateTexture2D(LPCWSTR i_fileName)
{
	HRESULT hr;
	WCHAR extention[64];
	
	FileNameExtension(i_fileName, extention, ARRAYSIZE(extention));

	if (_wcsnicmp(extention, L".dds", ARRAYSIZE(extention)))
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

	else if (_wcsnicmp(extention, L".png", ARRAYSIZE(extention)) ||
		_wcsnicmp(extention, L".jpg", ARRAYSIZE(extention)) ||
		_wcsnicmp(extention, L".jpeg", ARRAYSIZE(extention)) ||
		_wcsnicmp(extention, L".bmp", ARRAYSIZE(extention)))
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