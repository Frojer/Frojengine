#pragma once

#include "Frojengine.h"

struct SystemSetting
{
	DISPLAYMODE	displayMode;

	// 전체화면 사용 여부
	bool bWindowMode;
	// 수직동기화 사용여부
	bool bVSync;
};

class FJRenderingEngine;

class FJSystemEngine
{
private:
	HWND	_hWnd;

	TCHAR*	_className;
	TCHAR*	_windowName;

	static FJSystemEngine* _pInstance;

	FJRenderingEngine* _pRenderer;

public:
	bool	m_bEnd;

private:
	FJSystemEngine();

	bool InitWindow();
	void ShutdownWindow();
	bool ResizeWindow();
	static LRESULT CALLBACK MessageProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// 게임 세팅 저장, 로드
	bool SaveSetting();
	bool LoadSetting();
	
public:
	~FJSystemEngine();
	void MessagePump();

	void LoadData();
	void Update();
	void Rendering();

	static FJSystemEngine* GetInstance();
};

extern SystemSetting g_setting;