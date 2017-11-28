#pragma once

#define _FJ_DEBUG


/////////////////////////////////////////////////////////////////////////
//
//  DirectX ǥ�� ���̺귯�� 
//
/////////////////////////////////////////////////////////////////////////
//
//  < DirectX ������ ���� ���̺귯�� >
//  DX9         : DirectX SDK 2002 Nov. 
//  DX10.x/DX11 : DirectX SDK 2010 June (DX ���� ���� ������ ����)
//  DX11.X/DX12 : Windows SDK 8.x  (DX �� ���ԵǾ� ������)
//
/////////////////////////////////////////////////////////////////////////

// DX ǥ�� ���
#include <d3d11.h>					// DX ǥ�� ���. (DX 11.0)
//#include <d3d11_1.h>				// DX ǥ�� ���. (DX 11.1)
#pragma comment(lib, "D3D11")		// DX ���̺귯�� �ε�.  D3D11.dll �ʿ�.

// DirectX Math : Dx ���� ���̺귯�� 
//#include <XNAMath.h>				// XNA Math ���̺귯��.(ver.2.0) DirectX SDK 2010 June. ����.
#include <DirectXMath.h>			// XNA Math ���̺귯��.(ver.3.0) VS2012 (Windows SDK 8) �̻�.
using namespace DirectX;

// DXGI : DirectX Graphics Infrastructure 
// DX �� ������ �������� �ϵ���� ������ ���� ����� �����մϴ�.
//#include <dxgi.h>					// DXGI ���. (d3d11.h �� ���Ե�)
#pragma comment(lib, "dxgi")		// DXGI ���̺귯��. DXGI.dll �ʿ�.

#ifdef _DEBUG 
#pragma comment(lib, "DirectXTKd")		//32bit + ����� ����.
//#pragma comment(lib, "DirectXTK64d")	//64bit + ����� ����.
#else
#pragma comment(lib, "DirectXTK")		//32bit ����.
//#pragma comment(lib, "DiectXTK64")	//64bit ���� 
#endif


///////////////////////////////////////////////////////////////////////////////
//
//  Ÿ�� �� ��� ������ : ������ ȣȯ�� ��� ���밡 ����.
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

typedef ID3D11Texture2D*			LPTEXTURE2D;
typedef ID3D11ShaderResourceView*	LPRESOURCEVIEW;

typedef ID3D11VertexShader*	LPVERTEXSHADER;
typedef ID3D11PixelShader*	LPPIXELSHADER;
typedef ID3DBlob*			LPXDATA;			// ���� ������ ����� ����.(Vertex, Adjacency, material, binary code, etc..)
typedef LPXDATA				LPVSCODE;			// �����ϵ� ���̴� ���̳ʸ��ڵ� ���� ����.




// ��ġ ���� ���� ����ü. (DX9/11 ���� ȣȯ�� ������)
typedef DXGI_MODE_DESC		DISPLAYMODE;		// DX11 ����
//typedef DXGI_MODE_DESC1	DISPLAYMODE;		// DX11.1 ����
//typedef D3DDISPLAYMODE	DISPLAYMODE;		// DX9 ��.


/*
//------------------------------------------
// DirectX Math Ÿ�� ������ : ȣȯ�� ���
// DX9/DX10 : DirectX SDK June.2010 ����
//------------------------------------------
typedef D3DXMATRIXA16	MATRIXA;	//��� : 16����Ʈ ���� ����.
typedef D3DXMATRIX		MATRIX;		//��� : �Ϲ� ����.
typedef D3DXVECTOR4		VECTOR4;
typedef D3DXVECTOR3		VECTOR3;
typedef D3DXVECTOR2		VECTOR2;
*/

//------------------------------------------------------------------------------------------
// DirectX Math Ÿ�� ������ : ���� �ҽ����� ȣȯ�� ����� ����.
// DX10/11 "XNAMath ver.2" : DirectX SDK june.2010
// DX11/12 "XNAMath ver.3" : DirectXMath, Windows SDK 8.x �� ����  
// ��ũ : https://msdn.microsoft.com/ko-kr/library/windows/desktop/ee418728(v=vs.85).aspx
//------------------------------------------------------------------------------------------
typedef XMMATRIX		MATRIXA;	// ��� : 16����Ʈ ����, SIMD ����. ����/���� ������. "Register Type"
typedef XMFLOAT4X4		MATRIX;		// ��� : �Ϲ� ����. SIMD ������, Class ������ �����. "Storage Type"
typedef XMVECTOR		VECTOR;		// 4���� ���� : 16����Ʈ ����, SIMD ����. ����/���� ������. "Register Type"
typedef XMFLOAT4		VECTOR4;	// 4���� ���� : �Ϲ� ����, SIMD ������, Class ������ �����. "Storage Type"
typedef XMFLOAT3		VECTOR3;
typedef XMFLOAT2		VECTOR2;

// ���� Ÿ�� : 2����.
//typedef XMCOLOR		COLOR;		// r, g, b, a.  [������ 0~255]
typedef XMFLOAT4		COLOR;		// r, g, b, a.  [�Ǽ��� 0~1.0]


//////////////////////////////////////////////////
//
//  ���� ������ ���� ����
//
//////////////////////////////////////////////////
struct VF_PNT
{
	VECTOR3 pos;
	VECTOR3 nor;
	VECTOR2 uv;
};


struct IndexFormat
{
	UINT x, y, z;
};


///////////////////////////////////////////////////////////////////////////////
//
// ��Ÿ �ý��� ���� �Լ� �� ������.
//
///////////////////////////////////////////////////////////////////////////////

//�񵿱� Ű���� �Է� ó��.
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

#define FOR_STL(stl) for (auto iter = (stl).begin(); iter != (stl).end(); iter++)