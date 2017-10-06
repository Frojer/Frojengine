#pragma once

#include "FJDefine.h"

class FJSystemEngine
{
private:
	HWND	_hWnd;

	TCHAR*	_className;
	TCHAR*	_windowName;

	static FJSystemEngine* _pInstance;

public:
	bool	m_bEnd;

private:
	FJSystemEngine();

	bool InitWindow();
	bool ResizeWindow();
	static LRESULT CALLBACK MessageProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// ���� ���� ����, �ε�
	bool SaveSetting();
	bool LoadSetting();
	
public:
	~FJSystemEngine();
	void MessagePump();

	static FJSystemEngine* GetInstance();
};