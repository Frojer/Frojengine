#pragma once

#include "Frojengine.h"

class CShader : public IObject, private Device
{
private:
	static unordered_map<UINT, CShader*> _shaderMap;

	static LPBUFFER _pWVP_CB;
	static LPBUFFER _pLight_CB;
	LPBUFFER _pConstBuffer;

	LPVERTEXSHADER _pVS;
	LPPIXELSHADER _pPS;
	LPXDATA	_pVSCode;

	LPINPUTLAYOUT _pInputLayout;

	UINT _countTexture;
	UINT _countMatrix;
	UINT _countVector;
	UINT _countScalar;

	bool _useLight;

private:
	static CShader* CreateShader(LPCWSTR i_fileName);
	static HRESULT ShaderCompile(
		LPCWSTR i_fileName,		//�ҽ������̸�.
		char* i_EntryPoint,		//�����Լ� ������.
		char* i_ShaderModel,		//���̴� ��.
		ID3DBlob** o_ppCode		//[���] �����ϵ� ���̴� �ڵ�.
	);

	static void ShaderError(BOOL bMBox, TCHAR* msg, HRESULT hr, ID3DBlob* pBlob, LPCWSTR filename, char* EntryPoint, char* ShaderModel);

	static void ClearMap();

	virtual void Render();
	static HRESULT CreateDefaultBuffer();

	HRESULT CreateConstantBuffer(UINT size, ID3D11Buffer** ppCB);
	HRESULT CreateDynamicConstantBuffer(UINT size, LPVOID pData, ID3D11Buffer** ppCB);
	HRESULT UpdateDynamicConstantBuffer(ID3D11Resource* pBuff, LPVOID pData, UINT size);

	void UpdateConstantBuffer(LPVOID pCB, UINT size);

public:
	CShader();
	~CShader();

	static CShader* Find(UINT id);
	static CShader* Find(LPCWSTR name);

	friend class FJRenderingEngine;
	friend class FJSystemEngine;
	friend class CMaterial;
};