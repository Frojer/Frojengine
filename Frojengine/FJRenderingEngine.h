#pragma once

#include "Frojengine.h"

class FJFontEngine;

enum
{
	RM_SOLID = 0x00,
	RM_WIRE = 0x01,
};

enum
{
	DM_TEST_ON = 0x00,
	DM_TEST_OFF = 0x01,
	DM_WRITE_ON = 0x00,
	DM_WRITE_OFF = 0x02,
};

class FJRenderingEngine
{
private:
	//-------------------
	// 렌더링 관련 데이터
	//-------------------

	// 장치 상태 및 렌더링 명령 관리 인터페이스.
	LPDEVICE	_pDevice;
	LPDXDC		_pDXDC;
	LPSWAPCHAIN	_pSwapChain;
	LPRTVIEW	_pRTView;

	//깊이 스텐실 버퍼 관련.
	ID3D11Texture2D*		 _pDS;			//깊이-스텐실 버퍼.
	ID3D11DepthStencilView*  _pDSView;		//깊이-스텐실 뷰.

	// 무슨색으로 백버퍼를 지울지
	static COLOR _clearCol;

	enum {
		RS_SOLID,				// 기본 렌더링 : 솔리드 Soild
		RS_WIRE,				// 와이어프레임 렌더링.

		RS_MAX_
	};
	//상태 객체 배열 : "기능별" 그룹으로 관리합니다.
	ID3D11RasterizerState*	_pRState[RS_MAX_];
	//               Wire
	// Bit : 0000000 0
	static byte _rsData;



	// 깊이/스텐실 버퍼 상태객체.
	enum {
		DS_DEFAULT,			//깊이버퍼 ON! (기본값)
		DS_DEPTH_TEST_OFF,	//깊이버퍼 OFF!
		DS_DEPTH_WRITE_OFF,	//깊이버퍼 쓰기 끄기.
		DS_DT_OFF_DW_OFF,	//깊이테스트 끄기.

		DS_MAX_,
	};
	ID3D11DepthStencilState* _pDSState[DS_MAX_];
	//       DepthTest DepthWrite
	// Bit : 0         0          000000
	static byte _dsData;



	// 색상/알파 혼합 상태 객체 Blending State Objects
	enum {
		BS_DEFAULT,		//기본혼합 객체.
		BS_ADD,			//덧셈.

		BS_MAX_
	};
	// 색상/알파 혼합 관리 객체
	ID3D11BlendState* _pBState[BS_MAX_] = { nullptr, };
	void BlendStateCreate();
	void BlendStateRelease();

private:
	bool CreateRenderingEngine(HWND i_hWnd);

	bool DXSetup(HWND i_hWnd);
	void DXRelease();

	bool CreateDeviceSwapChain(HWND i_hWnd);
	bool CreateRenderTarget();
	bool CreateDepthStencil();

	void SetViewPort();
	void GetDeviceInfo();
	void GetFeatureLevel();

	// 장치 상태 객체 생성/제거
	void StateObjectCreate();
	void StateObjectRelease();

	// 레스터 상태객체
	void RasterStateLoad();
	void RasterStateUpdate();
	void RasterStateRelease();

	// 깊이/스텐실 버퍼 상태객체
	void DSStateLoad();
	void DSStateUpdate();
	void DSStateRelease();

	void ClearBackBuffer();
	void Flip();

	static void FJErrorW(TCHAR* file, UINT line, TCHAR* func, BOOL bMBox, HRESULT hr, TCHAR* msg, ...);

public:
	FJRenderingEngine();
	~FJRenderingEngine();

	static void		SetWireFrame(bool i_bSet);
	static bool		GetWireFrame();
	static void		SetSolidFrame(bool i_bSet);
	static bool		GetSolidFrame();
	static void		SetRasterMode(byte i_rm);
	static void		SetDepthTest(bool i_bSet);
	static bool		GetDepthTest();
	static void		SetDepthWrite(bool i_bSet);
	static bool		GetDepthWrite();
	static void		SetClearColor(COLOR& i_col);
	static COLOR	GetClearColor();

	friend class FJSystemEngine;
	friend class CShader;
};

#define FJError(hr, msg, ...)  \
FJRenderingEngine::FJErrorW( __FILEW__, __LINE__, __FUNCTIONW__, TRUE, hr, msg, __VA_ARGS__ )