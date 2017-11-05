#include <d3dcompiler.h>				//DX ���̴� �����Ϸ� ���.
#pragma comment(lib, "d3dcompiler")		//DX ���̴� �����Ϸ� ���̺귯��.  D3DCompiler.dll �ʿ�.

#include "Shader.h"


CShader::CShader()
{

}


CShader::~CShader()
{
	SAFE_RELEASE(_pVS);
	SAFE_RELEASE(_pPS);
	SAFE_RELEASE(_pInputLayout);
}


bool CShader::CreateShader(LPCWSTR i_fileName, LPDEVICE i_pDevice)
{
	CShader* pShader = new CShader();

	//--------------------------
	// ���� ���̴� ����.
	//--------------------------
	// ���� ���̴� ������ Compile the vertex shader

	HRESULT hr = ShaderCompile(i_fileName, "VS_Main", "vs_5_0", &_pVSCode);
	
	if (FAILED(hr))
	{
		FJError(hr, L"Failed to Vertex Shader Compile");
		//MessageBox(NULL, L"[����] ShaderLoad :: Vertex Shader ������ ����", L"Error", MB_OK);
		return false;
	}

	// ���� ���̴� ��ü ���� Create the vertex shader
	hr = i_pDevice->CreateVertexShader(	_pVSCode->GetBufferPointer(), 
									    _pVSCode->GetBufferSize(), 
									    nullptr, 
	  								    &_pVS
			                            );
	if (FAILED(hr))
	{
		FJError(hr, L"Failed to CreateVertexShader()");
		SAFE_RELEASE(_pVSCode);			//�ӽ� ��ü ����.
		return false;
	}

	//--------------------------
	// �ȼ� ���̴� ����.
	//--------------------------
	// �ȼ� ���̴� ������ Compile the pixel shader
	ID3DBlob* pPSCode = nullptr;
	hr = ShaderCompile(i_fileName, "PS_Main", "ps_5_0", &pPSCode);
	
	if (FAILED(hr))
	{
		FJError(hr, L"Failed to Pixel Shader Compile");
		//MessageBox(NULL, L"[����] ShaderLoad :: Pixel Shader ������ ����", L"Error", MB_OK);
		return false;
	}
	// �ȼ� ���̴� ��ü ���� Create the pixel shader
	hr = i_pDevice->CreatePixelShader(	pPSCode->GetBufferPointer(), 
										pPSCode->GetBufferSize(), 
										nullptr,
										&_pPS
									 );

	SAFE_RELEASE(pPSCode);				//�ӽ� ��ü ����.	

	if (FAILED(hr))
	{
		FJError(hr, L"Failed to CreatePixelShader()");
		return false;
	}

	return true;
}






////////////////////////////////////////////////////////////////////////////// 
//
// ���̴� �ҽ� ������
//
HRESULT CShader::ShaderCompile(
						LPCWSTR FileName,		//�ҽ������̸�.
						char* EntryPoint,		//�����Լ� ������.
						char* ShaderModel,		//���̴� ��.
						ID3DBlob** ppCode		//[���] �����ϵ� ���̴� �ڵ�.
						)
{
	HRESULT hr = S_OK; 
	ID3DBlob* pError = nullptr;

	//������ �ɼ�1.
	UINT Flags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;		//���켱 ��� ó��. ���� DX9 ���������� �������� ���. �ӵ��� �䱸�ȴٸ�, "��켱" ���� ó���� ��.
	//UINT Flags = D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;	//��켱 ��� ó��. �� �켱 ó������ �ӵ��� ����� ������, ����� ��ġ�Ѽ� GPU �� �����ؾ� �Ѵ�.
	//UINT Flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
	//Flags |=  D3DCOMPILE_DEBUG;		//����� ��� �߰�.


	//���̴� �ҽ� ������.
	hr = D3DCompileFromFile(FileName, 
							nullptr, nullptr, 
							EntryPoint, 
							ShaderModel,
							Flags,		//������ �ɼ�.1							 
							//Flags | D3DCOMPILE_DEBUG,	//������ �ɼ�1 + ����� ���� �߰�.
							0,					//������ �ɼ�2,  Effect ���� �����Ͻ� �����. �̿ܿ��� ���õ�.
							ppCode,				//[���] �����ϵ� ���̴� �ڵ�.
							&pError				//[���] ������ ���� �ڵ�.
							);
	if (FAILED(hr))
	{
		OutputDebugString(reinterpret_cast<TCHAR*>(pError->GetBufferPointer()));
		pError->Release();		 
		return hr;
	}
	
	SAFE_RELEASE(pError);
	return hr;
}