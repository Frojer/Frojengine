#pragma once

#include "Frojengine.h"

class FJFontEngine;

enum
{
	RM_SOLID = 0x00,
	RM_WIRE = 0x01,
};

class FJRenderingEngine
{
private:
	//-------------------
	// ������ ���� ������
	//-------------------

	// ��ġ ���� �� ������ ��� ���� �������̽�.
	LPDEVICE	_pDevice;
	LPDXDC		_pDXDC;
	LPSWAPCHAIN	_pSwapChain;
	LPRTVIEW	_pRTView;

	// ���������� ����۸� ������
	static COLOR _clearCol;

	enum {
		RS_SOLID,				// �⺻ ������ : �ָ��� Soild
		RS_WIRE,				// ���̾������� ������.

		RS_MAX_
	};

	//���� ��ü �迭 : "��ɺ�" �׷����� �����մϴ�.
	ID3D11RasterizerState*	_pRState[RS_MAX_];
	//               Wire
	// Bit : 0000000 0
	static byte _rsData;

private:
	bool CreateRenderingEngine(HWND i_hWnd);

	bool DXSetup(HWND i_hWnd);
	void DXRelease();

	bool CreateDeviceSwapChain(HWND i_hWnd);
	bool CreateRenderTarget();

	void SetViewPort();
	void GetDeviceInfo();
	void GetFeatureLevel();

	void RasterStateLoad();
	void RasterStateUpdate();
	void RasterStateRelease();

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
	static void		SetClearColor(COLOR i_col);
	static COLOR	GetClearColor();

	friend class FJSystemEngine;
	friend class CShader;
};

#define FJError(hr, msg, ...)  \
FJRenderingEngine::FJErrorW( __FILEW__, __LINE__, __FUNCTIONW__, TRUE, hr, msg, __VA_ARGS__ )