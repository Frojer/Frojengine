#include "FJSystemEngine.h"

SystemSetting g_setting;

FJSystemEngine* FJSystemEngine::_pInstance = nullptr;

FJSystemEngine::FJSystemEngine()
{
	
}

FJSystemEngine::~FJSystemEngine()
{
	SAFE_DELETE(_pRenderer)
	ShutdownWindow();
}


bool FJSystemEngine::CreateEngine()
{
	bool result;

	m_bEnd = false;
	_className = L"FJ";
	_windowName = L"FJEngine";


	//-------------------
	// 설정 불러오기
	//-------------------
	result = LoadSetting();

	if (!result)
	{
#ifdef _FJ_DEBUG
		ErrMsgBox(L"Failed to load data.");
#endif
		return false;
	}


	//-------------------
	// 윈도우 초기화
	//-------------------
	result = InitWindow();

	if (!result)
	{
		ErrMsgBox(L"Failed to create window.");
		
		return false;
	}


	//----------------------------
	// 렌더링 엔진 생성 및 초기화
	//----------------------------
	_pRenderer = new FJRenderingEngine();

	result = _pRenderer->CreateRenderingEngine(_hWnd);

	if (!result)
		return false;

	return true;
}


////////////////////////////////////////
//
//  InitWindow : 윈도우 생성 함수
//
//  기본적인 윈도우를 생성합니다.
//
////////////////////////////////////////
bool FJSystemEngine::InitWindow()
{
	WNDCLASSEX wc = { 
		sizeof(WNDCLASSEX), CS_CLASSDC, MessageProcedure, 0, 0,
			::GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
			_className, NULL 
	};

	RegisterClassEx(&wc);
 
	//윈도우 생성.
	_hWnd = ::CreateWindow( _className, _windowName, 
							WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
							//WS_OVERLAPPEDWINDOW, 
							0, 0, 
							g_setting.displayMode.Width, g_setting.displayMode.Height,
							GetDesktopWindow(), NULL, 
							wc.hInstance, NULL
	);
	
	if(_hWnd == NULL)
		return false;
	

	::ShowWindow(_hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(_hWnd);

	// 클라이언트 영역 크기 재조정 : 필수. 삭제금지
	ResizeWindow();

	// 마우스 커서 : 기본화살표로 설정.
	SetCursor(LoadCursor(NULL, IDC_ARROW));

	return true;
}



void FJSystemEngine::ShutdownWindow()
{
	// 마우스 커서를 표시합니다.
	ShowCursor(true);

	/*// 풀스크린 모드를 빠져나올 때 디스플레이 설정을 바꿉니다.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}*/

	// 창을 제거합니다.
	DestroyWindow(_hWnd);
	_hWnd = NULL;

	// 어플리케이션 인스턴스를 제거합니다.
	UnregisterClass(_className, ::GetModuleHandle(NULL));

	return;
}



/////////////////////////////////////////////////////////////////////////
//
// void B3D_ResizeWindow(UINT NewWidth, UINT NewHeight)
//
// 창모드의 경우 해상도 사이즈에 맞추어 전체 윈도우 크기를 재조정합니다. 
// ex)
//'클라이 언트'영역을 640 * 480 에 맞추어  전체 윈도우의 크기를 
// 다시 보정합니다. --> 그래야 '게임 이미지'가 모두 화면에 보인다.
//
// 인  자 :  클라이언트 영역의 크기 - NewWidth, NewHeight
// 리턴값 :  없음. 
//
// 2004.05.28. 버전업.
//
/////////////////////////////////////////////////////////////////////////
bool FJSystemEngine::ResizeWindow()
{
	BOOL result;

	//현재 윈도우의 스타일 구하기.
	RECT oldrc;
	DWORD sytle = (DWORD) ::GetWindowLong(_hWnd, GWL_STYLE);
	DWORD exstyle = (DWORD) ::GetWindowLong(_hWnd, GWL_EXSTYLE);

	//현재 윈도우 '전체' 크기 (스크린좌표) 를 얻는다.
	result = ::GetWindowRect(_hWnd, &oldrc);

	if (!result)
	{
		return false;
	}

	//새로 생성될 윈도우의 '클라이언트' 영역 크기 계산하기.
	RECT newrc;
	newrc.left = 0;
	newrc.top = 0;
	newrc.right = g_setting.displayMode.Width;
	newrc.bottom = g_setting.displayMode.Height;

	//newrc 만큼의 클라이언트 영역을 포함하는 윈도우 '전체' 크기를 구합니다.
	//현재 '메뉴' 는 없다는 가정하에 처리되고 있음.
	//계산된 결과는 newrc 에 다시 들어온다. ( 스크린 좌표 )
	//exstyle &= ~WS_EX_TOPMOST;
	result = ::AdjustWindowRectEx(&newrc, sytle, NULL, exstyle);

	if (!result)
	{
		return false;
	}


	//보정된 윈도우의 너비와 폭을 구합니다. 
	int newWidth = (newrc.right - newrc.left);
	int newHeight = (newrc.bottom - newrc.top);


	//새로운 크기를 윈도우에 설정합니다.
	result = ::SetWindowPos(_hWnd, HWND_NOTOPMOST,
		oldrc.left, oldrc.top,
		newWidth, newHeight, SWP_SHOWWINDOW);

	if (!result)
	{
		return false;
	}

	return true;
}




/////////////////////////////////////////////////////////////////////
// 
//  int MessagePump () : 윈도우 메세지 처리 함수.
// 
//  윈도우즈(OS)로부터 이 어플리케이션으로 특별히 메세지가 날라오지  
//  않는다면 게임을 진행 하도록 합니다.                               
// 
/////////////////////////////////////////////////////////////////////
void FJSystemEngine::MessagePump()
{
	static MSG msg;	::ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		// 메세지 큐에 메세지가 있으면...처리하고..
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)			// 윈도우 종료를 위해 리턴.
			{
				m_bEnd = true;
				break;
			}

			//나머지 메세지 처리.
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		else  //특별한 메세지가 없다면 리턴후 게임(렌더링)을 진행합니다..
		{
			break;
		}
	}
}




void FJSystemEngine::LoadData()
{
	// 원래는 이곳에서 Scene을 로드해야하는데
	// 일단은 씬이 하나라 생각하고 데이터들을 로드합니다

	// Clear할 색 설정
	_pRenderer->SetClearColor(COLOR(0.0f, 0.125f, 0.3f, 1.0f));
}



void FJSystemEngine::Run()
{
	//---------------
	// Game Loop
	//---------------
	while (!m_bEnd)
	{
		MessagePump();

		Update();
		Render();
	}
}




void FJSystemEngine::Update()
{
	//-------------------------------
	// 엔진/시스템 갱신.
	//------------------------------- 
	//float dTime = EngineUpdate();
}




void FJSystemEngine::Render()
{
	//-------------------------------
	// 장면 그리기 시작.. 
	//-------------------------------

	//렌더타겟(백버퍼) 지우기.. 
	_pRenderer->ClearBackBuffer();

	/////////////////////////////////////////////////////////////////////////////////////////////
	//                                      임시 생성 라인                                       //

	//도움말 및 기타 렌더링 정보 출력.
	ShowInfo();


	//                                      임시 생성 라인                                       // 
	/////////////////////////////////////////////////////////////////////////////////////////////


	//-------------------------------
	// 장면 그리기 종료.
	//------------------------------- 
	_pRenderer->Flip();
}




LRESULT CALLBACK FJSystemEngine::MessageProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			SendMessage(hwnd, WM_DESTROY, 0, 0);
			break;
		}
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


bool FJSystemEngine::SaveSetting()
{
	return true;
}

bool FJSystemEngine::LoadSetting()
{
	ZeroMemory(&g_setting, sizeof(g_setting));

	g_setting.displayMode.Width = 1600;								// 해상도 결정.(백버퍼 크기)
	g_setting.displayMode.Height = 900;
	g_setting.displayMode.RefreshRate.Numerator = 0;				// 버퍼 갱신율.(수직동기화 VSync 활성화시 표준갱신율 적용 : 60hz)
	//g_setting.displayMode.RefreshRate.Numerator = 0;				// 버퍼 갱신율.(수직동기화 VSync Off)
	g_setting.displayMode.RefreshRate.Denominator = 1;
	g_setting.displayMode.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// 백버퍼 색상규격 (A8R8G8B8) 창모드에서는 생략 가능 
	g_setting.bWindowMode = true;

	return true;
}

FJSystemEngine* FJSystemEngine::GetInstance()
{
	if (_pInstance == nullptr)
	{
		_pInstance = new FJSystemEngine();
	}

	return _pInstance;
}













/////////////////////////////////////////////////////////////////////////////////////////////
//                                      임시 생성 라인                                       //


/////////////////////////////////////////////////////////////////////////////// 
//
//  도움말. 
//
void FJSystemEngine::ShowInfo()
{ 
	PutFPS(0, 0);

	static bool bShow = true;
	if (IsKeyUp(VK_F1)) bShow ^= true;

	if (!bShow)
	{
		//ynTextDraw(1,20, COLOR(1, 1, 0, 1), "[Help] F1"); 
		return;
	}


	// Today's Topic.
	{
		int x = 300, y = 50;
		COLOR col(1, 1, 1, 1);
		COLOR col2(1, 1, 0, 1);
		_pRenderer->m_pFontEngine->TextDraw(x, y, col, L"■ %s", _windowName);

		y += 24;
		WCHAR* msg =
			L"1.기본프레임워크 구축.\n"
			L"2.HW 렌더링 디바이스(DX11 Device) 를 생성.\n"
			L"3.Idle 시간 렌더링.\n"
			L"4.스왑체인 Swap(Flipping) chain 의 이해 \n";
			//L"5.전체화면 또는 창모드 전환 (Alt-Enter) \n"
			//L"6.수직동기화(VSync): 티어링(Tearing), 셔터링(Shuttering) 방지";
		_pRenderer->m_pFontEngine->TextDraw(x, y, col, msg);
	}

 
	int x = 300, y = 300;
	static int cnt = 0;
	COLOR col(1, 1, 0, 1);
	_pRenderer->m_pFontEngine->TextDraw(x, y, col, L"Hello, Device!!    cnt=%08d", ++cnt);

	y += 14;
	y += 14;
	_pRenderer->m_pFontEngine->TextDraw(x, y, col, L"* HW Rendering Device (%s) 버전 *", g_setting.strFeatureLevels);

}




///////////////////////////////////////////////////////////////////////////////
//
// 초당 프레임수 출력.
//
// 인자 : int x, y : 출력할 화면 좌표.
//
void FJSystemEngine::PutFPS(int x, int y)
{
	static int oldtime = GetTickCount();
	int nowtime = GetTickCount();

	static int frmcnt = 0;
	static float fps = 0.0f;

	++frmcnt;

	int time = nowtime - oldtime;
	if (time >= 999)
	{
		oldtime = nowtime;

		fps = (float)frmcnt * 1000 / (float)time;
		frmcnt = 0;
	}

	//char msg[40];
	//sprintf(msg,"FPS:%.1f/%d", fps, frmcnt );
	//g_pFont->TextDraw(NULL, msg, -1, NULL, DT_NOCLIP, COLOR(0, 1, 0, 1));
	_pRenderer->m_pFontEngine->TextDraw(x, y, COLOR(0, 1, 0, 1), L"FPS:%.1f/%d", fps, frmcnt);
}



//                                      임시 생성 라인                                       //
/////////////////////////////////////////////////////////////////////////////////////////////