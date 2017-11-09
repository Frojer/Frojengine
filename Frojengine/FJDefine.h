#pragma once

#define _FJ_DEBUG


/////////////////////////////////////////////////////////////////////////
//
//  DirectX 표준 라이브러리 
//
/////////////////////////////////////////////////////////////////////////
//
//  < DirectX 버전별 개발 라이브러리 >
//  DX9         : DirectX SDK 2002 Nov. 
//  DX10.x/DX11 : DirectX SDK 2010 June (DX 독립 배포 마지막 버전)
//  DX11.X/DX12 : Windows SDK 8.x  (DX 가 포함되어 배포됨)
//
/////////////////////////////////////////////////////////////////////////

// DX 표준 헤더
#include <d3d11.h>					// DX 표준 헤더. (DX 11.0)
//#include <d3d11_1.h>				// DX 표준 헤더. (DX 11.1)
#pragma comment(lib, "D3D11")		// DX 라이브러리 로딩.  D3D11.dll 필요.

// DirectX Math : Dx 수학 라이브러리 
//#include <XNAMath.h>				// XNA Math 라이브러리.(ver.2.0) DirectX SDK 2010 June. 대응.
#include <DirectXMath.h>			// XNA Math 라이브러리.(ver.3.0) VS2012 (Windows SDK 8) 이상.
using namespace DirectX;

// DXGI : DirectX Graphics Infrastructure 
// DX 의 버전에 독립적인 하드웨어 저수준 접근 방식을 제공합니다.
//#include <dxgi.h>					// DXGI 헤더. (d3d11.h 에 포함됨)
#pragma comment(lib, "dxgi")		// DXGI 라이브러리. DXGI.dll 필요.

///////////////////////////////////////////////////////////////////////////////
//
//  타입 과 상수 재정의 : 버전별 호환성 향상 증대가 목적.
//
///////////////////////////////////////////////////////////////////////////////
typedef ID3D11Device*			LPDEVICE;
typedef ID3D11DeviceContext*	LPDXDC;
typedef IDXGISwapChain*			LPSWAPCHAIN;
typedef ID3D11RenderTargetView*	LPRTVIEW;

typedef ID3D11Buffer*		LPBUFFER;
typedef LPBUFFER			LPVERTEXBUFFER;
typedef LPBUFFER			LPINDEXBUFFER;
typedef ID3D11InputLayout*	LPINPUTLAYOUT;

typedef ID3D11Texture2D*	LPTEXTURE2D;

typedef ID3D11VertexShader*	LPVERTEXSHADER;
typedef ID3D11PixelShader*	LPPIXELSHADER;
typedef ID3DBlob*			LPXDATA;			// 임의 데이터 저장용 버퍼.(Vertex, Adjacency, material, binary code, etc..)
typedef LPXDATA				LPVSCODE;			// 컴파일된 셰이더 바이너리코드 저장 버퍼.




// 장치 설정 정보 구조체. (DX9/11 구형 호환성 유지용)
typedef DXGI_MODE_DESC		DISPLAYMODE;		// DX11 대응
//typedef DXGI_MODE_DESC1	DISPLAYMODE;		// DX11.1 대응
//typedef D3DDISPLAYMODE	DISPLAYMODE;		// DX9 용.


/*
//------------------------------------------
// DirectX Math 타입 재정의 : 호환성 향상
// DX9/DX10 : DirectX SDK June.2010 이하
//------------------------------------------
typedef D3DXMATRIXA16	MATRIXA;	//행렬 : 16바이트 정렬 버전.
typedef D3DXMATRIX		MATRIX;		//행렬 : 일반 버전.
typedef D3DXVECTOR4		VECTOR4;
typedef D3DXVECTOR3		VECTOR3;
typedef D3DXVECTOR2		VECTOR2;
*/

//------------------------------------------------------------------------------------------
// DirectX Math 타입 재정의 : 구형 소스와의 호환성 향상이 목적.
// DX10/11 "XNAMath ver.2" : DirectX SDK june.2010
// DX11/12 "XNAMath ver.3" : DirectXMath, Windows SDK 8.x 에 포함  
// 링크 : https://msdn.microsoft.com/ko-kr/library/windows/desktop/ee418728(v=vs.85).aspx
//------------------------------------------------------------------------------------------
typedef XMMATRIX		MATRIXA;	// 행렬 : 16바이트 정렬, SIMD 버전. 전역/지역 변수용. "Register Type"
typedef XMFLOAT4X4		MATRIX;		// 행렬 : 일반 버전. SIMD 미지원, Class 데이터 저장용. "Storage Type"
typedef XMVECTOR		VECTOR;		// 4성분 벡터 : 16바이트 정렬, SIMD 버전. 전역/지역 변수용. "Register Type"
typedef XMFLOAT4		VECTOR4;	// 4성분 벡터 : 일반 버전, SIMD 미지원, Class 데이터 저장용. "Storage Type"
typedef XMFLOAT3		VECTOR3;
typedef XMFLOAT2		VECTOR2;

// 색상 타입 : 2가지.
//typedef XMCOLOR		COLOR;		// r, g, b, a.  [정수형 0~255]
typedef XMFLOAT4		COLOR;		// r, g, b, a.  [실수형 0~1.0]


//////////////////////////////////////////////////
//
//  정점 데이터 구조 정의
//
//////////////////////////////////////////////////
struct VF_PNT
{
	VECTOR3 pos;
	VECTOR3 nor;
	VECTOR2 uv;
};


struct VF_Pos
{
	VECTOR3 pos;
};



///////////////////////////////////////////////////////////////////////////////
//
// 기타 시스템 관련 함수 및 재정의.
//
///////////////////////////////////////////////////////////////////////////////

//비동기 키보드 입력 처리.
#ifndef IsKeyDown
#define IsKeyDown(key)	((GetAsyncKeyState(key)&0x8000) == 0x8000)
#define IsKeyUp(key)	((GetAsyncKeyState(key)&0x8001) == 0x8001)
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(pObj)	if ((pObj) != nullptr) { (pObj)->Release();	(pObj) = nullptr; }
#define SAFE_DELETE(pObj)	if ((pObj) != nullptr) { delete (pObj);		(pObj) = nullptr; }
#define SAFE_DELARRY(pObj)	if ((pObj) != nullptr) { delete[] (pObj);	(pObj) = nullptr; }
#endif

#define ErrMsgBox(msg)	MessageBox(nullptr, msg, L"Error", MB_OK | MB_ICONERROR)

#define FOR_LIST(list) for (auto iter = list.begin(); iter != list.end(); iter++)