#include <d3dcompiler.h>				//DX 셰이더 컴파일러 헤더.
#pragma comment(lib, "d3dcompiler")		//DX 셰이더 컴파일러 라이브러리.  D3DCompiler.dll 필요.

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
	// 정점 셰이더 생성.
	//--------------------------
	// 정점 셰이더 컴파일 Compile the vertex shader

	HRESULT hr = ShaderCompile(i_fileName, "VS_Main", "vs_5_0", &_pVSCode);
	
	if (FAILED(hr))
	{
		FJError(hr, L"Failed to Vertex Shader Compile");
		//MessageBox(NULL, L"[실패] ShaderLoad :: Vertex Shader 컴파일 실패", L"Error", MB_OK);
		return false;
	}

	// 정점 셰이더 객체 생성 Create the vertex shader
	hr = i_pDevice->CreateVertexShader(	_pVSCode->GetBufferPointer(), 
									    _pVSCode->GetBufferSize(), 
									    nullptr, 
	  								    &_pVS
			                            );
	if (FAILED(hr))
	{
		FJError(hr, L"Failed to CreateVertexShader()");
		SAFE_RELEASE(_pVSCode);			//임시 개체 제거.
		return false;
	}

	//--------------------------
	// 픽셀 셰이더 생성.
	//--------------------------
	// 픽셀 셰이더 컴파일 Compile the pixel shader
	ID3DBlob* pPSCode = nullptr;
	hr = ShaderCompile(i_fileName, "PS_Main", "ps_5_0", &pPSCode);
	
	if (FAILED(hr))
	{
		FJError(hr, L"Failed to Pixel Shader Compile");
		//MessageBox(NULL, L"[실패] ShaderLoad :: Pixel Shader 컴파일 실패", L"Error", MB_OK);
		return false;
	}
	// 픽셀 셰이더 객체 생성 Create the pixel shader
	hr = i_pDevice->CreatePixelShader(	pPSCode->GetBufferPointer(), 
										pPSCode->GetBufferSize(), 
										nullptr,
										&_pPS
									 );

	SAFE_RELEASE(pPSCode);				//임시 개체 제거.	

	if (FAILED(hr))
	{
		FJError(hr, L"Failed to CreatePixelShader()");
		return false;
	}

	return true;
}






////////////////////////////////////////////////////////////////////////////// 
//
// 셰이더 소스 컴파일
//
HRESULT CShader::ShaderCompile(
						LPCWSTR FileName,		//소스파일이름.
						char* EntryPoint,		//메인함수 진입점.
						char* ShaderModel,		//셰이더 모델.
						ID3DBlob** ppCode		//[출력] 컴파일된 셰이더 코드.
						)
{
	HRESULT hr = S_OK; 
	ID3DBlob* pError = nullptr;

	//컴파일 옵션1.
	UINT Flags = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;		//열우선 행렬 처리. 구형 DX9 이전까지의 전통적인 방식. 속도가 요구된다면, "행우선" 으로 처리할 것.
	//UINT Flags = D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;	//행우선 행렬 처리. 열 우선 처리보다 속도의 향상이 있지만, 행렬을 전치한수 GPU 에 공급해야 한다.
	//UINT Flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
	//Flags |=  D3DCOMPILE_DEBUG;		//디버깅 모드 추가.


	//셰이더 소스 컴파일.
	hr = D3DCompileFromFile(FileName, 
							nullptr, nullptr, 
							EntryPoint, 
							ShaderModel,
							Flags,		//컴파일 옵션.1							 
							//Flags | D3DCOMPILE_DEBUG,	//컴파일 옵션1 + 디버깅 정보 추가.
							0,					//컴파일 옵션2,  Effect 파일 컴파일시 적용됨. 이외에는 무시됨.
							ppCode,				//[출력] 컴파일된 셰이더 코드.
							&pError				//[출력] 컴파일 에러 코드.
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