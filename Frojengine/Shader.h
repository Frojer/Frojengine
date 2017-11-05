#pragma once

#include "Frojengine.h"

class CShader
{
private:
	LPVERTEXSHADER _pVS;
	LPPIXELSHADER _pPS;
	LPXDATA	_pVSCode;

	LPINPUTLAYOUT _pInputLayout;

private:
	bool CreateShader(LPCWSTR i_fileName, LPDEVICE i_pDevice);
	HRESULT ShaderCompile(
		LPCWSTR i_fileName,		//소스파일이름.
		char* i_EntryPoint,		//메인함수 진입점.
		char* i_ShaderModel,		//셰이더 모델.
		ID3DBlob** o_ppCode		//[출력] 컴파일된 셰이더 코드.
	);

public:
	CShader();
	~CShader();


	friend class ShaderManager;
};