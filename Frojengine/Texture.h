#pragma once

#include "Frojengine.h"

class CTexture2D : public IObject
{
private:
	static unordered_map<UINT, CTexture2D*> _textureMap;

	static LPDEVICE _pDevice;
	static LPDXDC _pDXDC;

	enum
	{
		ADDRESS_CLAMP,
		ADDRESS_WRAP,
		ADDRESS_BORDER,

		ADDRESS_MAX,
	};
	// �ؽ�ó ���÷�
	static ID3D11SamplerState*	_pSampler[ADDRESS_MAX];
public:
	VECTOR4 m_vBorderColor;
	LPRESOURCEVIEW _ResourceView;
	UINT m_AddressFilter;
private:
	void SamplerCreate();
	void SamplerRelease();

public:
	CTexture2D(LPCWSTR i_fileName);
	~CTexture2D();

	bool CreateTexture2D(LPCWSTR i_fileName);

	CTexture2D* Find(UINT id);
	CTexture2D* Find(LPCWSTR name);

	static ID3D11SamplerState* GetSampler(UINT addressFilter);

	friend class FJRenderingEngine;
};