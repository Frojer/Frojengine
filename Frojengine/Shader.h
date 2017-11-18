#pragma once

#include "Frojengine.h"

class CShader : public IObject
{
private:
	static unordered_map<UINT, CShader*> _shaderMap;

	static LPBUFFER _pWVP_CB;
	static LPBUFFER _pLight_CB;
	LPBUFFER _pConstBuffer;

	static LPDEVICE _pDevice;
	static LPDXDC _pDXDC;

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
		LPCWSTR i_fileName,		//소스파일이름.
		char* i_EntryPoint,		//메인함수 진입점.
		char* i_ShaderModel,		//셰이더 모델.
		ID3DBlob** o_ppCode		//[출력] 컴파일된 셰이더 코드.
	);

	static void ShaderError(BOOL bMBox, TCHAR* msg, HRESULT hr, ID3DBlob* pBlob, LPCWSTR filename, char* EntryPoint, char* ShaderModel);

	static void ClearMap();

protected:
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