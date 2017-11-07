#pragma once

#include "Frojengine.h"

class CShader
{
private:
	static LPDEVICE _pDevice;
	static LPDXDC _pDXDC;

	LPVERTEXSHADER _pVS;
	LPPIXELSHADER _pPS;
	LPXDATA	_pVSCode;

	LPINPUTLAYOUT _pInputLayout;

private:
	static HRESULT ShaderCompile(
		LPCWSTR i_fileName,		//소스파일이름.
		char* i_EntryPoint,		//메인함수 진입점.
		char* i_ShaderModel,		//셰이더 모델.
		ID3DBlob** o_ppCode		//[출력] 컴파일된 셰이더 코드.
	);

	void Render();

public:
	CShader();
	~CShader();

	static CShader* CreateShader(LPCWSTR i_fileName);

	friend class ShaderManager;
	friend class FJSystemEngine;
	friend class CMaterial;
};