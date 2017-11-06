#pragma once

#include "Frojengine.h"

struct SystemSetting
{
	DISPLAYMODE	displayMode;

	// DirectX ����
	D3D_FEATURE_LEVEL featureLevels;
	LPCWSTR strFeatureLevels;

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

	void Update();
	void Render();
	
public:
	~FJSystemEngine();
	bool CreateEngine();

	void MessagePump();

	void LoadData();

	void Run();

	static FJSystemEngine* GetInstance();



	/////////////////////////////////////////////////////////////////////////////////////////////
	//                                      �ӽ� ���� ����                                       //

	void PutFPS(int x, int y);
	void ShowInfo();

	//                                      �ӽ� ���� ����                                       //
	/////////////////////////////////////////////////////////////////////////////////////////////
};

extern SystemSetting g_setting;