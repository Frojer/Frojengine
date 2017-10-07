#pragma once

#include "Frojengine.h"

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
	COLOR _clearCol;

	

public:

private:
	bool DXSetup(HWND i_hWnd);
	void DXRelease();

	bool CreateDeviceSwapChain(HWND i_hWnd);
	bool CreateRenderTarget();

	void SetViewPort();

	void FJErrorW(TCHAR* file, UINT line, TCHAR* func, BOOL bMBox, HRESULT hr, TCHAR* msg, ...);

public:
	FJRenderingEngine(HWND i_hWnd);
	~FJRenderingEngine();

	LPDEVICE	GetDevice();
	LPDXDC		GetDXDC();

	void		SetClearColor(COLOR i_col);
	COLOR		GetClearColor();

	void ClearBackBuffer();
	void Flip();
};

#define FJError(hr, msg, ...)  \
FJErrorW( __FILEW__, __LINE__, __FUNCTIONW__, TRUE, hr, msg, __VA_ARGS__ )