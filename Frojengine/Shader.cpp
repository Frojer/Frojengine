#include <d3dcompiler.h>				//DX 셰이더 컴파일러 헤더.
#pragma comment(lib, "d3dcompiler")		//DX 셰이더 컴파일러 라이브러리.  D3DCompiler.dll 필요.

#include "Shader.h"


LPDEVICE CShader::_pDevice = nullptr;
LPDXDC CShader::_pDXDC = nullptr;


CShader::CShader()
	: _pVS(nullptr), _pPS(nullptr), _pVSCode(nullptr), _pInputLayout(nullptr)
{

}


CShader::~CShader()
{
	SAFE_RELEASE(_pInputLayout);
	SAFE_RELEASE(_pPS);
	SAFE_RELEASE(_pVS);
	SAFE_RELEASE(_pVSCode);
}


CShader* CShader::CreateShader(LPCWSTR i_fileName)
{
	CShader* pShader = new CShader();

	if (pShader == nullptr)
	{
		ErrMsgBox(L"Failed to Memory Allocate");
		return nullptr;
	}

	//--------------------------
	// 정점 셰이더 생성.
	//--------------------------
	// 정점 셰이더 컴파일 Compile the vertex shader
	HRESULT hr = ShaderCompile(i_fileName, "VS_Main", "vs_5_0", &pShader->_pVSCode);
	
	if (FAILED(hr))
	{
		FJError(hr, L"Failed to Vertex Shader Compile");
		//MessageBox(NULL, L"[실패] ShaderLoad :: Vertex Shader 컴파일 실패", L"Error", MB_OK);
		delete pShader;
		pShader = nullptr;

		return nullptr;
	}

	// 정점 셰이더 객체 생성 Create the vertex shader
	hr = _pDevice->CreateVertexShader(	pShader->_pVSCode->GetBufferPointer(), 
									    pShader->_pVSCode->GetBufferSize(), 
									    nullptr, 
	  								    &pShader->_pVS
			                            );
	if (FAILED(hr))
	{
		FJError(hr, L"Failed to CreateVertexShader()");
		delete pShader;
		pShader = nullptr;

		return nullptr;
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

		delete pShader;
		pShader = nullptr;

		return nullptr;
	}
	// 픽셀 셰이더 객체 생성 Create the pixel shader
	hr = _pDevice->CreatePixelShader(	pPSCode->GetBufferPointer(), 
										pPSCode->GetBufferSize(), 
										nullptr,
										&pShader->_pPS
									 );

	SAFE_RELEASE(pPSCode);				//임시 개체 제거.	

	if (FAILED(hr))
	{
		FJError(hr, L"Failed to CreatePixelShader()");

		delete pShader;
		pShader = nullptr;

		return nullptr;
	}

	// 정점 입력구조 Input layout ★
	// GPU 에 공급될 기하데이터 - 개별 정점의 데이터 구조와 용도등의 정보를 구성합니다.
	// 구형 Vertex Format(DX7/8/9) 또는 Vertex Declaration(DX9) 과 동일 목적으로 사용되지만 
	// 신형 렌더링 기술의 요구에 맞추어 구조적 및 기능적으로 확장되었습니다.
	//
	// 바른 렌더링 결과를 위해서는 아래의 조건이 동일 또는 호환되어야 합니다.
	// 1.정점 버퍼의 데이터.  Vertex Buffer Data
	// 2.정점 구조 Vertex Format (Input Layout)
	// 3.셰이더 함수의 입력구조.  Vertex Shader (Input Layout)
	//
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		//  Sementic          format                       offset         classification             
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// 정접 입력구조 객체 생성 Create the input layout
	hr = _pDevice->CreateInputLayout(  layout,
										numElements,
										pShader->_pVSCode->GetBufferPointer(),
										pShader->_pVSCode->GetBufferSize(),
										&pShader->_pInputLayout
									);
	if (FAILED(hr))
		return nullptr;

	return pShader;
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


void CShader::Render()
{
	//입력 레이아웃 설정. Set the input layout
	_pDXDC->IASetInputLayout(_pInputLayout);

	//셰이더 설정.
	_pDXDC->VSSetShader(_pVS, nullptr, 0);
	_pDXDC->PSSetShader(_pPS, nullptr, 0);
}