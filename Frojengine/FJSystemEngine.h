#pragma once

#include "Frojengine.h"

struct SystemSetting
{
	DISPLAYMODE	displayMode;
	DXGI_SAMPLE_DESC sampleDesc;

	//Anisotropic Filter 배수.(최대 16)
	DWORD anisotropy;

	// DirectX 버전
	D3D_FEATURE_LEVEL featureLevels;
	LPCWSTR strFeatureLevels;

	// 전체화면 사용 여부
	bool bWindowMode;
	// 수직동기화 사용여부
	bool bVSync;
};

class FJRenderingEngine;
class CShader;

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
	float	m_fDeltaTime;
private:
	FJSystemEngine();

	bool InitWindow();
	void ShutdownWindow();
	bool ResizeWindow();
	static LRESULT CALLBACK MessageProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// 게임 세팅 저장, 로드
	bool SaveSetting();
	bool LoadSetting();

	void CreateShaderBuffer(CShader* i_pShader);

	void EngineTimer();

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
	//                                      임시 생성 라인                                       //

	void PutFPS(int x, int y);
	void ShowInfo();

	//                                      임시 생성 라인                                       //
	/////////////////////////////////////////////////////////////////////////////////////////////
};

extern SystemSetting g_setting;