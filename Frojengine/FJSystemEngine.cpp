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
	// ���� �ҷ�����
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
	// ������ �ʱ�ȭ
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
//  InitWindow : ������ ���� �Լ�
//
//  �⺻���� �����츦 �����մϴ�.
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
 
	//������ ����.
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
// â����� ��� �ػ� ����� ���߾� ��ü ������ ũ�⸦ �������մϴ�. 
// ex)
//'Ŭ���� ��Ʈ'������ 640 * 480 �� ���߾�  ��ü �������� ũ�⸦ 
// �ٽ� �����մϴ�. --> �׷��� '���� �̹���'�� ��� ȭ�鿡 ���δ�.
//
// ��  �� :  Ŭ���̾�Ʈ ������ ũ�� - NewWidth, NewHeight
// ���ϰ� :  ����. 
//
// 2004.05.28. ������.
//
/////////////////////////////////////////////////////////////////////////
bool FJSystemEngine::ResizeWindow()
{
	BOOL result;

	//���� �������� ��Ÿ�� ���ϱ�.
	RECT oldrc;
	DWORD sytle = (DWORD) ::GetWindowLong(_hWnd, GWL_STYLE);
	DWORD exstyle = (DWORD) ::GetWindowLong(_hWnd, GWL_EXSTYLE);

	//���� ������ '��ü' ũ�� (��ũ����ǥ) �� ��´�.
	result = ::GetWindowRect(_hWnd, &oldrc);

	if (!result)
	{
		return false;
	}

	//���� ������ �������� 'Ŭ���̾�Ʈ' ���� ũ�� ����ϱ�.
	RECT newrc;
	newrc.left = 0;
	newrc.top = 0;
	newrc.right = g_setting.windowWidth;
	newrc.bottom = g_setting.windowHeight;

	//newrc ��ŭ�� Ŭ���̾�Ʈ ������ �����ϴ� ������ '��ü' ũ�⸦ ���մϴ�.
	//���� '�޴�' �� ���ٴ� �����Ͽ� ó���ǰ� ����.
	//���� ����� newrc �� �ٽ� ���´�. ( ��ũ�� ��ǥ )
	//exstyle &= ~WS_EX_TOPMOST;
	result = ::AdjustWindowRectEx(&newrc, sytle, NULL, exstyle);

	if (!result)
	{
		return false;
	}


	//������ �������� �ʺ�� ���� ���մϴ�. 
	int newWidth = (newrc.right - newrc.left);
	int newHeight = (newrc.bottom - newrc.top);


	//���ο� ũ�⸦ �����쿡 �����մϴ�.
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
//  int MessagePump () : ������ �޼��� ó�� �Լ�.
// 
//  ��������(OS)�κ��� �� ���ø����̼����� Ư���� �޼����� �������  
//  �ʴ´ٸ� ������ ���� �ϵ��� �մϴ�.                               
// 
/////////////////////////////////////////////////////////////////////
void FJSystemEngine::MessagePump()
{
	static MSG msg;	::ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		// �޼��� ť�� �޼����� ������...ó���ϰ�..
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)			// ������ ���Ḧ ���� ����.
			{
				m_bEnd = true;
				break;
			}

			//������ �޼��� ó��.
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		else  //Ư���� �޼����� ���ٸ� ������ ����(������)�� �����մϴ�..
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