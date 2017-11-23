#include "FJSystemEngine.h"

#include "MainScene.h"

SystemSetting g_setting;

FJSystemEngine* FJSystemEngine::_pInstance = nullptr;

FJSystemEngine::FJSystemEngine()
{
	
}

FJSystemEngine::~FJSystemEngine()
{
	CMaterial::ClearMap();
	CShader::ClearMap();
	CMesh::ClearMap();
	delete SceneManager::GetInstance();

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
	// ���� �ҷ�����
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
	// ������ �ʱ�ȭ
	//-------------------
	result = InitWindow();

	if (!result)
	{
		ErrMsgBox(L"Failed to create window.");
		
		return false;
	}


	//----------------------------
	// ������ ���� ���� �� �ʱ�ȭ
	//----------------------------
	_pRenderer = new FJRenderingEngine();

	result = _pRenderer->CreateRenderingEngine(_hWnd);

	if (!result)
		return false;

	return true;
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
							g_setting.displayMode.Width, g_setting.displayMode.Height,
							GetDesktopWindow(), NULL, 
							wc.hInstance, NULL
	);
	
	if(_hWnd == NULL)
		return false;
	

	::ShowWindow(_hWnd, SW_SHOWDEFAULT);
	::UpdateWindow(_hWnd);

	// Ŭ���̾�Ʈ ���� ũ�� ������ : �ʼ�. ��������
	ResizeWindow();

	// ���콺 Ŀ�� : �⺻ȭ��ǥ�� ����.
	SetCursor(LoadCursor(NULL, IDC_ARROW));

	return true;
}



void FJSystemEngine::ShutdownWindow()
{
	// ���콺 Ŀ���� ǥ���մϴ�.
	ShowCursor(true);

	/*// Ǯ��ũ�� ��带 �������� �� ���÷��� ������ �ٲߴϴ�.
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}*/

	// â�� �����մϴ�.
	DestroyWindow(_hWnd);
	_hWnd = NULL;

	// ���ø����̼� �ν��Ͻ��� �����մϴ�.
	UnregisterClass(_className, ::GetModuleHandle(NULL));

	return;
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
	newrc.right = g_setting.displayMode.Width;
	newrc.bottom = g_setting.displayMode.Height;

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




void FJSystemEngine::CreateShaderBuffer(CShader* i_pShader)
{
	VECTOR* pData;
	UINT totalSize;

	totalSize = (i_pShader->_countMatrix * 4) + i_pShader->_countVector + i_pShader->_countScalar;
	if (totalSize != 0)
	{
		pData = (VECTOR*)_aligned_malloc(sizeof(VECTOR) * totalSize, 16);
		//pData = new VECTOR[totalSize];
		ZeroMemory(pData, sizeof(VECTOR) * totalSize);
		i_pShader->CreateDynamicConstantBuffer(totalSize * sizeof(VECTOR), pData, &i_pShader->_pConstBuffer);
		//delete[] pData;
		_aligned_free(pData);
	}
}




void FJSystemEngine::LoadData()
{
	CShader* pShader = nullptr;
	SceneManager* pSM = SceneManager::GetInstance();
	CScene* pScene = nullptr;

	//==============
	// ���̴� �ε�
	//==============
	pShader = CShader::CreateShader(L"./fx/Standard.fx");
	pShader->m_name = L"Standard";
	pShader->_countTexture = 1;
	pShader->_countScalar = 1;
	pShader->_countVector = 3;
	pShader->_countMatrix = 0;

	CreateShaderBuffer(pShader);

	pShader = CShader::CreateShader(L"./fx/Error.fx");
	pShader->m_name = L"Error";

	CreateShaderBuffer(pShader);
	
	pShader = CShader::CreateShader(L"./fx/Line.fx");
	pShader->m_name = L"Line";
	pShader->_countVector = 1;

	CreateShaderBuffer(pShader);

	//FileLoader::ObjectFileLoad(L"./Data/Terrain/terrain.x");
	//FileLoader::ObjectFileLoad(L"./Data/Windmill/Windmill.x");
	//FileLoader::ObjectFileLoad(L"./Data/Box/Box.x");

	//==========================================
	// Scene ���� �� SceneManager ���� �־��ش�
	//==========================================
	pScene = new MainScene();

	pSM->AddScene(pScene);

	//===========================================
	// ù��° ���� �ε��Ѵ�
	//===========================================
	pSM->LoadScene((UINT)0);
}



void FJSystemEngine::Run()
{
	//---------------
	// Game Loop
	//---------------
	while (!m_bEnd)
	{
		MessagePump();

		EngineTimer();

		SceneManager::GetInstance()->ChangeScene();

		//-------------------------------
		// ��� �׸��� ����.. 
		//-------------------------------

		//����Ÿ��(�����) �����.. 
		_pRenderer->ClearBackBuffer();
		_pRenderer->RasterStateUpdate();
		_pRenderer->DSStateUpdate();

		SceneManager::pCurrentScene->Update();
		SceneManager::pCurrentScene->Render();

		//-------------------------------
		// ��� �׸��� ����.
		//------------------------------- 
		_pRenderer->Flip();
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
	ZeroMemory(&g_setting, sizeof(g_setting));

	g_setting.displayMode.Width = 1600;								// �ػ� ����.(����� ũ��)
	g_setting.displayMode.Height = 900;
	g_setting.displayMode.RefreshRate.Numerator = 0;				// ���� ������.(��������ȭ VSync Ȱ��ȭ�� ǥ�ذ����� ���� : 60hz)
	//g_setting.displayMode.RefreshRate.Numerator = 0;				// ���� ������.(��������ȭ VSync Off)
	g_setting.displayMode.RefreshRate.Denominator = 1;
	g_setting.displayMode.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// ����� ����԰� (A8R8G8B8) â��忡���� ���� ���� 
	g_setting.bWindowMode = true;
	g_setting.sampleDesc.Count = 8;					//AA ����
	g_setting.sampleDesc.Quality = 0;
	g_setting.anisotropy = 16;

	return true;
}


void FJSystemEngine::EngineTimer()
{
	static int oldtime = GetTickCount();
	int nowtime = GetTickCount();
	m_fDeltaTime = (nowtime - oldtime) * 0.001f;
	oldtime = nowtime;
}


FJSystemEngine* FJSystemEngine::GetInstance()
{
	if (_pInstance == nullptr)
	{
		_pInstance = new FJSystemEngine();
	}

	return _pInstance;
}