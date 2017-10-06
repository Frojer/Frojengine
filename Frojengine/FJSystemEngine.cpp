#include "FJSystemEngine.h"

struct SystemSetting
{
	int windowWidth;
	int windowHeight;
};

SystemSetting g_setting;

FJSystemEngine* FJSystemEngine::_pInstance = nullptr;

FJSystemEngine::FJSystemEngine()
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
#ifdef _FJ_DEBUG_
		MessageBox(nullptr, L"Failed to load data.", L"Error", MB_OK | MB_ICONERROR);
#endif
		m_bEnd = true;
		return;
	}


	//-------------------
	// 윈도우 초기화
	//-------------------
	result = InitWindow();

	if (!result)
	{
		MessageBox(nullptr, L"Failed to create window.", L"Error", MB_OK | MB_ICONERROR);
		m_bEnd = true;
		return;
	}
}

FJSystemEngine::~FJSystemEngine()
{

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
							g_setting.windowWidth, g_setting.windowHeight,
							GetDesktopWindow(), NULL, 
							wc.hInstance, NULL
	);
	
	if(_hWnd == NULL)
		return false;
	

	::ShowWindow(_hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(_hWnd);

	ResizeWindow();

	return true;
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
	newrc.right = g_setting.windowWidth;
	newrc.bottom = g_setting.windowHeight;

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
	g_setting.windowWidth = 1600;
	g_setting.windowHeight = 900;

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