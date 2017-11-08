#pragma once

#include <tchar.h>
#include <list>
#include <vector>
#include <unordered_map>

using namespace std;

#include <Windows.h>

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



///////////////////////////////////////////
//
//  Frojer Engine System Header
//
///////////////////////////////////////////

#include "FJDefine.h"
#include "FJSystemEngine.h"
#include "FJRenderingEngine.h"


///////////////////////////////////////////
//
//  Frojer Engine Object Header
//
///////////////////////////////////////////

#include "Scene.h"
#include "Object.h"
#include "Model.h"
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"



///////////////////////////////////////////
//
//  Frojer Engine Object Manager Header
//
///////////////////////////////////////////

#include "SceneManager.h"
#include "ShaderManager.h"
#include "MaterIalManager.h"
#include "MeshManager.h"