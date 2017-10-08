#pragma once

#include <tchar.h>
#include <Windows.h>

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

#include "FJDefine.h"
#include "FJSystemEngine.h"
#include "FJRenderingEngine.h"
#include "FJFontEngine.h"