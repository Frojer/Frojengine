#pragma once

#include "Frojengine.h"

struct CB_Default
{
	MATRIXA mWorld;
	MATRIXA mView;
	MATRIXA mWV;
	MATRIXA mProj;
};

class CShader : public IObject
{
private:
	static unordered_map<UINT, CShader*> _shaderMap;
	
	static CB_Default _cbDefault;
	static LPBUFFER _pCBDefault;

	static LPDEVICE _pDevice;
	static LPDXDC _pDXDC;

	LPVERTEXSHADER _pVS;
	LPPIXELSHADER _pPS;
	LPXDATA	_pVSCode;

	LPINPUTLAYOUT _pInputLayout;

private:
	static CShader* CreateShader(LPCWSTR i_fileName);
	static HRESULT ShaderCompile(
		LPCWSTR i_fileName,		//소스파일이름.
		char* i_EntryPoint,		//메인함수 진입점.
		char* i_ShaderModel,		//셰이더 모델.
		ID3DBlob** o_ppCode		//[출력] 컴파일된 셰이더 코드.
	);

	static void ClearMap();

protected:
	virtual void Render();

	HRESULT CreateConstantBuffer(UINT size, ID3D11Buffer** ppCB);
	HRESULT CreateDynamicConstantBuffer(UINT size, LPVOID pData, ID3D11Buffer** ppCB);
	HRESULT UpdateDynamicConstantBuffer(LPDXDC pDXDC, ID3D11Resource* pBuff, LPVOID pData, UINT size);

	HRESULT UpdateDefaultBuffer(MATRIXA& mWorld);

public:
	CShader();
	~CShader();

	static CShader* Find(UINT id);
	static CShader* Find(LPCWSTR name);

	friend class FJRenderingEngine;
	friend class FJSystemEngine;
	friend class CMaterial;
	friend class CScene;
};