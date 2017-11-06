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
	static HRESULT ShaderCompile(
		LPCWSTR i_fileName,		//�ҽ������̸�.
		char* i_EntryPoint,		//�����Լ� ������.
		char* i_ShaderModel,		//���̴� ��.
		ID3DBlob** o_ppCode		//[���] �����ϵ� ���̴� �ڵ�.
	);

public:
	CShader();
	~CShader();

	static CShader* CreateShader(LPCWSTR i_fileName, LPDEVICE i_pDevice);

	friend class ShaderManager;
};