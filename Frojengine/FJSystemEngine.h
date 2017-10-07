#pragma once

#include "Frojengine.h"

struct SystemSetting
{
	DISPLAYMODE	displayMode;

	// ��üȭ�� ��� ����
	bool bWindowMode;
	// ��������ȭ ��뿩��
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

	// ���� ���� ����, �ε�
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