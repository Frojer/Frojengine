//
// DirectXTK.h : DirectX Toolkit 헤더
//
// DirectX Toolkit : DX11 ( Dec.8, 2016) 
// 제공 Present : Chuck Walbourn @ Microsoft / MSFT (http://blogs.msdn.com/b/chuckw/)
// 편집 Edit    : Kihong Kim / onlysonim@gmail.com 
// 갱신 Update  : 2016.12.07
//
#pragma once
#pragma warning(disable:4996)

// Platform Header 및 Library
//
#include "Windows.h"
#include "stdio.h"

///////////////////////////////////////////////////////////////////////////////
//
// DX11 Header 및 라이브러리 
//
///////////////////////////////////////////////////////////////////////////////
/*
#include "D3D11.h"					//DX 표준 헤더. (DX 11.0)
//#include "D3D11_1.h"				//DX 표준 헤더. (DX 11.1)
#pragma comment(lib, "D3D11")		//DX 라이브러리 로딩.  D3D11.dll 필요.
//#include "XNAMath.h"				//XNA Math 라이브러리.(ver.2.0) DirectX SDK 2010 June. 대응.
#include "DirectXMath.h"			//XNA Math 라이브러리.(ver.3.0) VS2012 (Windows SDK 8) 이상.
using namespace DirectX;
//#include "DXGI.h"					//DXGI 헤더. (d3d11.h 에 포함됨)
#pragma comment(lib, "dxgi")		//DXGI 라이브러리. DXGI.dll 필요.
*/

///////////////////////////////////////////////////////////////////////////////
//
// DirectX Took Kit  라이브러리 
//
///////////////////////////////////////////////////////////////////////////////
//
// DX 유틸리티 및 일반화 라이브러리 모음집. 
// DX API 사용시 필요한 여러 기능 클래스 및 서비스를 제공합니다.
// 자세한 내용을 아래를 참조하세요.
// 라이브러리 설명 :  DXTK/ReadMe.txt
// 참조 : https://blogs.msdn.microsoft.com/chuckw/2012/03/02/directxtk/
// 
// <버전> DirectXTK, December 5, 2016 (DX11)
// <대상 플렛폼>
// Windows Desk Top Apps(Windows 7/8/10), XBox One, Windows Mobile (Windows Phone 8.1), Windows Store Apps 지원.
// <제공 기능>
// Math, State Objects, Texture, EffectSystem, Model(XFile) Animation, Font, Audio (XAudio 및 Wave), 등..
// <권장 개발환경>
// Visual Studio 2013 (update 5), Visual Studio 2015 (update 3), Windows 7 (sp1) 이상.
//
//
// DirectXTK 를 보다 편하게 사용하기 위해서 Project 속성 추가가 필요합니다.
// 다음 순서대로 Project 속성을 변경/추가 하십시요.
 //
// 1. C/C++>입력>일반>추가 포함 디렉터리 : "../Yena/DXTK/Inc" 추가
// 2. 링커>일반>추가 라이브러리 디렉터리 : "../Yena/DXTK/Lib " 추가
// 3. 링커>입력>추가 종속성 : DirectXTK.lib 추가 (또는 아래와 같이 직접 명시할 것)
// 4. DirectXTK 클래스별 헤더 선언 :  여러분의 프로젝트에 필요한 헤더를 추가. (아래 참조)
//
///////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG 
#pragma comment(lib, "DirectXTKd")		//32bit + 디버그 버전.
//#pragma comment(lib, "DirectXTK64d")	//64bit + 디버그 버전.
#else
#pragma comment(lib, "DirectXTK")		//32bit 버전.
//#pragma comment(lib, "DiectXTK64")	//64bit 버전 
#endif

//-----------------------------------------------------------------------------
// DirectXTK 헤더 선언 : 우리 프로젝트에 필요한 것만 추가하겠습니다.
//-----------------------------------------------------------------------------
// DirectXTK : SimpleMath 
// SIMD 가속 지원 자료형(XMVECTOR 및 XMMATRIX)의 사용시 복잡성을 최소화한 수학 라이브러리. 
// 간편하고 직관적인 사용법으로 다소의 성능저하를 상쇄합니다.
// 참조 : https://github.com/Microsoft/DirectXTK/wiki/SimpleMath
// 
#include "SimpleMath.h"	
using namespace DirectX::SimpleMath;

// DirectXTK : SpriteFont
// 2D Texture / Sprite 기반의 폰트 출력 라이브러리.
// 빠르고 경쾌하지만, CJK 계열의 폰트의 경우 완성형만 지원됩니다.
// 참조 : Yena/DXTK/MakeSpriteFont/SpriteFont 사용법.txt
// 참조 : https://directxtk.codeplex.com/wikipage?title=SpriteFont
//
#include "SpriteFont.h"
  


/**************** end of "DirectXTK.h" ***********************************/