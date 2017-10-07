#include "FJRenderingEngine.h"



FJRenderingEngine::FJRenderingEngine(HWND i_hWnd)
{
	DXSetup(i_hWnd);
}



FJRenderingEngine::~FJRenderingEngine()
{
	DXRelease();
}


bool FJRenderingEngine::DXSetup(HWND i_hWnd)
{
	bool result;
	
	//--------------------------------------------------
	// 1�ܰ� : ������ ��ġ ���� �ʼ� ����
	//--------------------------------------------------

	// D3D ������ ��ġ Device �� ����ü�� Swap Chain ����. 
	result = CreateDeviceSwapChain(i_hWnd);

	if (!result)
	{
		return false;
	}

	// ��ġ-����ü���� ����Ÿ��(�����) ȹ��
	result = CreateRenderTarget();

	if (!result)
	{
		return false;
	}

	// ��ġ ��º��ձ�(Output Merger) �� ���͸� Ÿ�� �� ����-���ٽ� ���� ���.
	_pDXDC->OMSetRenderTargets(
		1,							// ����Ÿ�� ����.(max: D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
		&_pRTView,					// ����Ÿ��("�����") ���.	
		nullptr
	);
	
	// ����Ʈ ����.
	SetViewPort();


	//----------------------------------------
	// 2�ܰ� : ��Ÿ ������ ���� �߰� ����.
	//----------------------------------------
	// �������� �ʿ��� ����� ��ü���� ����/���� �մϴ�.
	// ī�޶�, ����, ��Ʈ, ���̴� ���...


	return true;
}



///////////////////////////////////////////////////////////////////////////// 
//
//  ����̽� ��ü �� ���� ��ġ ���� : ����� 1ȸ ȣ��.  
//  �⺻ ������ ��ü�� ���Ž� ������ �������� ó���մϴ�.(����)
//
void FJRenderingEngine::DXRelease()
{
	// ��ġ ���� ���� : ���� ���� �ʱ�ȭ�� �ؾ� �մϴ�. (�޸� ���� ����)
	if (_pDXDC)
		_pDXDC->ClearState();
	//if (_pSwapChain)
	//	_pSwapChain->SetFullscreenState(false, NULL);


	SAFE_RELEASE(_pRTView);			//����Ÿ�� ����.
	SAFE_RELEASE(_pSwapChain);			//����ü�� ����.
	SAFE_RELEASE(_pDXDC);
	SAFE_RELEASE(_pDevice);			//����̽� ����. �� ���߿� �����մϴ�.
}





/////////////////////////////////////////////////////////////////////////////
//
//  D3D ������ '��ġ(Device)' �� ����ü�� ����. 
//
bool FJRenderingEngine::CreateDeviceSwapChain(HWND i_hWnd)
{
	HRESULT hr = S_OK;

	// D3D ��� ���� (Direct3D feature level) : DX11 ������ HW ������ ȣȯ�� ���
	D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_11_0;		// DX11 ����.
	//D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_9_3;		// DX9.0c ����.

	//--------------------------------------------------
	// ��ġ (Device) �� ����ü��(SwapChain) ���� ����.
	// ����ü���� ���߹��۸� �ý����� ���ϸ�
	// �������� '�ø���Flipping' ü�ΰ� ������ �ǹ��Դϴ�.  
	//--------------------------------------------------
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Windowed = g_setting.bWindowMode;				// Ǯ��ũ�� �Ǵ� â��� ����.
	sd.OutputWindow = i_hWnd;							// ����� ������ �ڵ�.
	sd.BufferCount = 1;									// ����� ����.
	sd.BufferDesc = g_setting.displayMode;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//�뵵 ����: '����Ÿ��' 
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//sd.Flags = 0;
	sd.SampleDesc.Count = 1;							//AA ����
	sd.SampleDesc.Quality = 0;
	
	
	
	// D3D ������ '��ġ(Device)' �� ����ü�� ����. 
	hr = D3D11CreateDeviceAndSwapChain(
			NULL,						// ���� ��� ������ (�⺻��ġ�� NULL)
			D3D_DRIVER_TYPE_HARDWARE,	// HW ���� 
			NULL,						// SW Rasterizer DLL �ڵ�.  HW ���ӽÿ��� NULL.
			0,							// ����̽� ���� �÷���.(�⺻��)
			//D3D11_CREATE_DEVICE_DEBUG,// ����̽� ���� �÷���.(�����)
			&featureLevels,				// (������) ����̽� ��� ����(Feature Level) �迭
			1,							// (������) ����̽� ��� ����(Feature Level) �迭 ũ��.
			D3D11_SDK_VERSION,			// DX SDK ����.
			&sd,						// ����̽� ���� �� �ɼ�.
			&_pSwapChain,				// [���] ����ü�� �������̽� ���.
			&_pDevice,					// [���] ����̽� �������̽� ���.
			NULL,						// [���] (������) ����̽� ��� ����. �ʿ���ٸ� NULL ����. 
			&_pDXDC						// [���] ����̽� ���ؽ�Ʈ ���.
	);

	if (FAILED(hr))
	{
		FJError(hr, L"����̽� / ����ü�� ���� ����.");
		return false;
	}
	
	return true;
}





/////////////////////////////////////////////////////////////////////////////
//
// ��ġ-����ü���� ����Ÿ��(�����) ȹ��
//
// ���ҽ���(Resource View)  
// DX11 �� ���ҽ��� "����" �������� �����ϰ� ����Ǿ� �ֽ��ϴ�. 
// (�޸� ������ ȿ���� ���� �� �ٿ뵵 Ȱ���� ����..)
// ���� ����� �� �뵵�� �´� ���ٹ��(�������̽�)�� ȹ��(����)�ؾ� �մϴ�.
// �� �۾��� "���ҽ� �� Resource View" �� ���� ó���մϴ�.
//                     
// ����ü�ο� ����(Bind)�� ������ ��¹��� - "����� BackBuffer" �� 2D ��� ���ҽ�(�ؽ�ó) �̸�
// "����Ÿ�� RenderTarget" ������ ���ҽ��並 ����, �ش� ���۸� ����/��� �ϰڽ��ϴ�.
//
bool FJRenderingEngine::CreateRenderTarget()
{
	HRESULT hr = S_OK;

	// ������ ȹ��.
	ID3D11Texture2D *pBackBuffer;
	
	hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);		// ����� ���, 2D �ؽ��� ����, ȭ����¿�
	if (FAILED(hr))
		return false;

	// ȹ���� ����ۿ� ����Ÿ�� �� ����.(����Ÿ��'��'���� ������)
	hr = _pDevice->CreateRenderTargetView(pBackBuffer, NULL, &_pRTView);		// '����Ÿ�ٺ�'�� ����.	
	if (FAILED(hr)) 
	{
		FJError(hr, L"�����-����Ÿ�� �� ���� ����.");
		return false;
	}

	// ���ҽ� �� ���� ��, ���ʿ��� DX �ڵ��� �����ؾ� �մϴ�.(�޸� ���� ����)
	SAFE_RELEASE(pBackBuffer);

	return true;
}





/////////////////////////////////////////////////////////////////////////////
//
// ����Ʈ ���� :  DX11 ������ �⺻ó�� ���� ������ ����ڰ� ���� �����ؾ��մϴ� 
//			     (OpenGL ������ �������� �̷��� �� �Խ��ϴ�.)
//
void FJRenderingEngine::SetViewPort()
{
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = (FLOAT)g_setting.displayMode.Width;
	vp.Height = (FLOAT)g_setting.displayMode.Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	_pDXDC->RSSetViewports(1, &vp);
}



////////////////////// 
//
//  ���� �޼��� ó��
//
//////////////////////
void FJRenderingEngine::FJErrorW(TCHAR* file, UINT line, TCHAR* func, BOOL bMBox, HRESULT hr, TCHAR* msg, ...)
{
	TCHAR msgva[1024] = L"";
	va_list vl;
	va_start(vl, msg);
	_vstprintf(msgva, sizeof(msgva), msg, vl);
	va_end(vl);


	// HRESULT ���� ���� �޼��� ���.
	TCHAR herr[1024] = L"";
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		herr, 1024, NULL);

	// ���� �޼��� ���̱�.
	TCHAR errmsg[2048] = L"";
	/*_stprintf(errmsg, L"%s \nErrorCode(0x%X) : %s \nFile = %s (%d)\nFunc = %s\n\n",
		msgva, hr, herr,
		file, line, func);*/

	swprintf_s(errmsg, L"%s \nErrorCode(0x%X) : %s \nFile = %s (%d)\nFunc = %s\n\n", msgva, hr, herr, file, line, func);


	// (����� ��) VS ���â���� ���..
	OutputDebugString(L"\n");
	OutputDebugString(errmsg);

	//�α����Ϸ� ���.
	//...

	// �޼��� â ���..
	if (bMBox)
	{
		MessageBox(NULL, errmsg, L"Error", MB_OK | MB_ICONERROR);
		//GetLastError(hr);
	}
}




/////////////////////////////////////////////////////////////////////////////
//
//  ��ġ-����ü��/����Ÿ��/����/���ٽ� ���� Ŭ���� 
//
void FJRenderingEngine::ClearBackBuffer()
{
	// ����Ÿ�� �����
	_pDXDC->ClearRenderTargetView(_pRTView, (float*)&_clearCol);
}



/////////////////////////////////////////////////////////////////////////////
//
//  ��ġ-����ü�� ��ȯ : ��� �׸��� �Ϸ�� �� ȣ���ؾ� ��.
//
void FJRenderingEngine::Flip()
{
	// ȭ����� : Flip!
	_pSwapChain->Present(0, 0);
}









//////////////////////////////////////////////////////////////
//
//  Get / Set �Լ���
//
//////////////////////////////////////////////////////////////
LPDEVICE FJRenderingEngine::GetDevice()
{
	return _pDevice;
}


LPDXDC FJRenderingEngine::GetDXDC()
{
	return _pDXDC;
}


void FJRenderingEngine::SetClearColor(COLOR i_col)
{
	_clearCol = i_col;
}


COLOR FJRenderingEngine::GetClearColor()
{
	return _clearCol;
}