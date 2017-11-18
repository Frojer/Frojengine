//
// DirectXTK.h : DirectX Toolkit ���
//
// DirectX Toolkit : DX11 ( Dec.8, 2016) 
// ���� Present : Chuck Walbourn @ Microsoft / MSFT (http://blogs.msdn.com/b/chuckw/)
// ���� Edit    : Kihong Kim / onlysonim@gmail.com 
// ���� Update  : 2016.12.07
//
#pragma once
#pragma warning(disable:4996)

// Platform Header �� Library
//
#include "Windows.h"
#include "stdio.h"

///////////////////////////////////////////////////////////////////////////////
//
// DX11 Header �� ���̺귯�� 
//
///////////////////////////////////////////////////////////////////////////////
/*
#include "D3D11.h"					//DX ǥ�� ���. (DX 11.0)
//#include "D3D11_1.h"				//DX ǥ�� ���. (DX 11.1)
#pragma comment(lib, "D3D11")		//DX ���̺귯�� �ε�.  D3D11.dll �ʿ�.
//#include "XNAMath.h"				//XNA Math ���̺귯��.(ver.2.0) DirectX SDK 2010 June. ����.
#include "DirectXMath.h"			//XNA Math ���̺귯��.(ver.3.0) VS2012 (Windows SDK 8) �̻�.
using namespace DirectX;
//#include "DXGI.h"					//DXGI ���. (d3d11.h �� ���Ե�)
#pragma comment(lib, "dxgi")		//DXGI ���̺귯��. DXGI.dll �ʿ�.
*/

///////////////////////////////////////////////////////////////////////////////
//
// DirectX Took Kit  ���̺귯�� 
//
///////////////////////////////////////////////////////////////////////////////
//
// DX ��ƿ��Ƽ �� �Ϲ�ȭ ���̺귯�� ������. 
// DX API ���� �ʿ��� ���� ��� Ŭ���� �� ���񽺸� �����մϴ�.
// �ڼ��� ������ �Ʒ��� �����ϼ���.
// ���̺귯�� ���� :  DXTK/ReadMe.txt
// ���� : https://blogs.msdn.microsoft.com/chuckw/2012/03/02/directxtk/
// 
// <����> DirectXTK, December 5, 2016 (DX11)
// <��� �÷���>
// Windows Desk Top Apps(Windows 7/8/10), XBox One, Windows Mobile (Windows Phone 8.1), Windows Store Apps ����.
// <���� ���>
// Math, State Objects, Texture, EffectSystem, Model(XFile) Animation, Font, Audio (XAudio �� Wave), ��..
// <���� ����ȯ��>
// Visual Studio 2013 (update 5), Visual Studio 2015 (update 3), Windows 7 (sp1) �̻�.
//
//
// DirectXTK �� ���� ���ϰ� ����ϱ� ���ؼ� Project �Ӽ� �߰��� �ʿ��մϴ�.
// ���� ������� Project �Ӽ��� ����/�߰� �Ͻʽÿ�.
 //
// 1. C/C++>�Է�>�Ϲ�>�߰� ���� ���͸� : "../Yena/DXTK/Inc" �߰�
// 2. ��Ŀ>�Ϲ�>�߰� ���̺귯�� ���͸� : "../Yena/DXTK/Lib " �߰�
// 3. ��Ŀ>�Է�>�߰� ���Ӽ� : DirectXTK.lib �߰� (�Ǵ� �Ʒ��� ���� ���� ����� ��)
// 4. DirectXTK Ŭ������ ��� ���� :  �������� ������Ʈ�� �ʿ��� ����� �߰�. (�Ʒ� ����)
//
///////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG 
#pragma comment(lib, "DirectXTKd")		//32bit + ����� ����.
//#pragma comment(lib, "DirectXTK64d")	//64bit + ����� ����.
#else
#pragma comment(lib, "DirectXTK")		//32bit ����.
//#pragma comment(lib, "DiectXTK64")	//64bit ���� 
#endif

//-----------------------------------------------------------------------------
// DirectXTK ��� ���� : �츮 ������Ʈ�� �ʿ��� �͸� �߰��ϰڽ��ϴ�.
//-----------------------------------------------------------------------------
// DirectXTK : SimpleMath 
// SIMD ���� ���� �ڷ���(XMVECTOR �� XMMATRIX)�� ���� ���⼺�� �ּ�ȭ�� ���� ���̺귯��. 
// �����ϰ� �������� �������� �ټ��� �������ϸ� ����մϴ�.
// ���� : https://github.com/Microsoft/DirectXTK/wiki/SimpleMath
// 
#include "SimpleMath.h"	
using namespace DirectX::SimpleMath;

// DirectXTK : SpriteFont
// 2D Texture / Sprite ����� ��Ʈ ��� ���̺귯��.
// ������ ����������, CJK �迭�� ��Ʈ�� ��� �ϼ����� �����˴ϴ�.
// ���� : Yena/DXTK/MakeSpriteFont/SpriteFont ����.txt
// ���� : https://directxtk.codeplex.com/wikipage?title=SpriteFont
//
#include "SpriteFont.h"

//DirectX::XMFLOAT4& DirectX::XMFLOAT4::operator=(const DirectX::XMFLOAT3& lhs)
//{
//	return DirectX::XMFLOAT4(lhs.x, lhs.y, lhs.z, 1.0f);
//}