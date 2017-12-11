#pragma once

#include "Frojengine.h"

class CTexture2D : public IObject, private Device
{
private:
	static unordered_map<UINT, CTexture2D*> _textureMap;

	enum
	{
		ADDRESS_CLAMP,
		ADDRESS_WRAP,
		ADDRESS_BORDER,

		ADDRESS_MAX,
	};
	// 텍스처 셈플러
	static ID3D11SamplerState*	_pSampler[ADDRESS_MAX];
	LPRESOURCEVIEW _pResourceView;

public:
	VECTOR4 m_vBorderColor;
	UINT m_AddressFilter;
private:
	static void SamplerCreate();
	static void SamplerRelease();

public:
	CTexture2D(LPCWSTR name);
	~CTexture2D();

	static CTexture2D* CreateTexture2D(LPCWSTR i_fileName);

	static CTexture2D* Find(UINT id);
	static CTexture2D* Find(LPCWSTR name);

	static ID3D11SamplerState* GetSampler(UINT addressFilter);
	const LPRESOURCEVIEW * const GetResourceView();

	friend class FJRenderingEngine;
};