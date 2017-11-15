#pragma once

#include "Frojengine.h"

class CTexture2D : public IObject
{
private:
	static unordered_map<UINT, CTexture2D*> _textureMap;

	static LPDEVICE _pDevice;
	static LPDXDC _pDXDC;
public:
	LPRESOURCEVIEW _ResourceView;
private:

public:
	CTexture2D(LPCWSTR i_fileName);
	~CTexture2D();

	bool CreateTexture2D(LPCWSTR i_fileName);

	CTexture2D* Find(UINT id);
	CTexture2D* Find(LPCWSTR name);

	friend class FJRenderingEngine;
};