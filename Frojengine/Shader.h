#pragma once

#include "Frojengine.h"

struct CB_Default
{
	MATRIXA mWorld;
	MATRIXA mView;
	MATRIXA mWV;
	MATRIXA mProj;

	VECTOR mtrlDiffuse;  //�� ����(Ȯ�걤) �� �ݻ���(%) 
	VECTOR mtrlAmbient;  //���� ����(�ֺ���) �� �ݻ���(%) 
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
		LPCWSTR i_fileName,		//�ҽ������̸�.
		char* i_EntryPoint,		//�����Լ� ������.
		char* i_ShaderModel,		//���̴� ��.
		ID3DBlob** o_ppCode		//[���] �����ϵ� ���̴� �ڵ�.
	);

	static void ClearMap();

protected:
	virtual void Render();

	HRESULT CreateConstantBuffer(UINT size, ID3D11Buffer** ppCB);
	HRESULT CreateDynamicConstantBuffer(UINT size, LPVOID pData, ID3D11Buffer** ppCB);
	HRESULT UpdateDynamicConstantBuffer(LPDXDC pDXDC, ID3D11Resource* pBuff, LPVOID pData, UINT size);

	HRESULT UpdateDefaultBuffer();

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