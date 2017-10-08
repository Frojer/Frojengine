#include "FJRenderingEngine.h"



FJRenderingEngine::FJRenderingEngine(HWND i_hWnd)
{
	DXSetup(i_hWnd);

	m_pFontEngine = new FJFontEngine(_pDevice, _pDXDC);

	if (m_pFontEngine == nullptr)
	{
		ErrMsgBox(L"Failed to create FontEngine");
		return;
	}
}



FJRenderingEngine::~FJRenderingEngine()
{
	SAFE_DELETE(m_pFontEngine)
	DXRelease();
}


bool FJRenderingEngine::DXSetup(HWND i_hWnd)
{
	bool result;
	
	//--------------------------------------------------
	// 1단계 : 렌더링 장치 관련 필수 설정
	//--------------------------------------------------

	// D3D 렌더링 장치 Device 및 스왑체인 Swap Chain 생성. 
	result = CreateDeviceSwapChain(i_hWnd);

	if (!result)
	{
		return false;
	}

	// 장치-스왑체인의 렌더타겟(백버퍼) 획득
	result = CreateRenderTarget();

	if (!result)
	{
		return false;
	}

	// 장치 출력병합기(Output Merger) 에 렌터링 타겟 및 깊이-스텐실 버퍼 등록.
	_pDXDC->OMSetRenderTargets(
		1,							// 렌더타겟 개수.(max: D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
		&_pRTView,					// 렌더타겟("백버퍼") 등록.	
		nullptr
	);
	
	// 뷰포트 설정.
	SetViewPort();



	//장치 정보 획득
	GetDeviceInfo();


	//----------------------------------------
	// 2단계 : 기타 렌더링 관련 추가 설정.
	//----------------------------------------
	// 렌더링에 필요한 사용자 객체등을 생성/관리 합니다.
	// 카메라, 조명, 폰트, 셰이더 등등...


	return true;
}



///////////////////////////////////////////////////////////////////////////// 
//
//  디바이스 개체 및 관련 장치 제거 : 종료시 1회 호출.  
//  기본 렌더링 개체들 제거시 생성된 역순으로 처리합니다.(권장)
//
void FJRenderingEngine::DXRelease()
{
	// 장치 상태 리셋 : 제거 전에 초기화를 해야 합니다. (메모리 누수 방지)
	if (_pDXDC)
		_pDXDC->ClearState();
	//if (_pSwapChain)
	//	_pSwapChain->SetFullscreenState(false, NULL);


	SAFE_RELEASE(_pRTView);			//렌더타겟 제거.
	SAFE_RELEASE(_pSwapChain);			//스왑체인 제거.
	SAFE_RELEASE(_pDXDC);
	SAFE_RELEASE(_pDevice);			//디바이스 제거. 맨 나중에 제거합니다.
}





/////////////////////////////////////////////////////////////////////////////
//
//  D3D 렌더링 '장치(Device)' 및 스왑체인 생성. 
//
bool FJRenderingEngine::CreateDeviceSwapChain(HWND i_hWnd)
{
	HRESULT hr = S_OK;

	// D3D 기능 레벨 (Direct3D feature level) : DX11 미지원 HW 에서의 호환성 향상
	g_setting.featureLevels = D3D_FEATURE_LEVEL_11_0;		// DX11 대응.
	//D3D_FEATURE_LEVEL featureLevels = D3D_FEATURE_LEVEL_9_3;		// DX9.0c 대응.

	//--------------------------------------------------
	// 장치 (Device) 및 스왑체인(SwapChain) 정보 구성.
	// 스왑체인은 다중버퍼링 시스템을 말하며
	// 고전적인 '플립핑Flipping' 체인과 동일한 의미입니다.  
	//--------------------------------------------------
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.Windowed = g_setting.bWindowMode;				// 풀스크린 또는 창모드 선택.
	sd.OutputWindow = i_hWnd;							// 출력할 윈도우 핸들.
	sd.BufferCount = 1;									// 백버퍼 개수.
	sd.BufferDesc = g_setting.displayMode;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//용도 설정: '렌더타겟' 
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//sd.Flags = 0;
	sd.SampleDesc.Count = 1;							//AA 설정
	sd.SampleDesc.Quality = 0;
	
	
	
	// D3D 렌더링 '장치(Device)' 및 스왑체인 생성. 
	hr = D3D11CreateDeviceAndSwapChain(
			NULL,						// 비디오 어뎁터 포인터 (기본장치는 NULL)
			D3D_DRIVER_TYPE_HARDWARE,	// HW 가속 
			NULL,						// SW Rasterizer DLL 핸들.  HW 가속시에는 NULL.
			0,							// 디바이스 생성 플래그.(기본값)
			//D3D11_CREATE_DEVICE_DEBUG,// 디바이스 생성 플래그.(디버그)
			&g_setting.featureLevels,	// (생성할) 디바이스 기능 레벨(Feature Level) 배열
			1,							// (생성할) 디바이스 기능 레벨(Feature Level) 배열 크기.
			D3D11_SDK_VERSION,			// DX SDK 버전.
			&sd,						// 디바이스 생성 상세 옵션.
			&_pSwapChain,				// [출력] 스왑체인 인터페이스 얻기.
			&_pDevice,					// [출력] 디바이스 인터페이스 얻기.
			NULL,						// [출력] (생성된) 디바이스 기능 레벨. 필요없다면 NULL 설정. 
			&_pDXDC						// [출력] 디바이스 컨텍스트 얻기.
	);

	if (FAILED(hr))
	{
		FJError(hr, L"디바이스 / 스왑체인 생성 실패.");
		return false;
	}
	
	return true;
}





/////////////////////////////////////////////////////////////////////////////
//
// 장치-스왑체인의 렌더타겟(백버퍼) 획득
//
// 리소스뷰(Resource View)  
// DX11 의 리소스는 "공유" 목적으로 유연하게 설계되어 있습니다. 
// (메모리 관리의 효율성 증대 및 다용도 활용이 가능..)
// 따라서 사용전 각 용도에 맞는 접근방식(인터페이스)를 획득(생성)해야 합니다.
// 이 작업을 "리소스 뷰 Resource View" 를 통해 처리합니다.
//                     
// 스왑체인에 연결(Bind)된 렌더링 출력버퍼 - "백버퍼 BackBuffer" 는 2D 기반 리소스(텍스처) 이며
// "렌더타겟 RenderTarget" 용으로 리소스뷰를 생성, 해당 버퍼를 접근/운용 하겠습니다.
//
bool FJRenderingEngine::CreateRenderTarget()
{
	HRESULT hr = S_OK;

	// 벡버퍼 획득.
	ID3D11Texture2D *pBackBuffer;
	
	hr = _pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);		// 백버퍼 얻기, 2D 텍스쳐 행태, 화면출력용
	if (FAILED(hr))
		return false;

	// 획득한 백버퍼에 렌더타겟 뷰 생성.(렌더타겟'형'으로 설정함)
	hr = _pDevice->CreateRenderTargetView(pBackBuffer, NULL, &_pRTView);		// '렌더타겟뷰'를 생성.	
	if (FAILED(hr)) 
	{
		FJError(hr, L"백버퍼-렌더타겟 뷰 생성 실패.");
		return false;
	}

	// 리소스 뷰 생성 후, 불필요한 DX 핸들은 해제해야 합니다.(메모리 누수 방지)
	SAFE_RELEASE(pBackBuffer);

	return true;
}





/////////////////////////////////////////////////////////////////////////////
//
// 뷰포트 설정 :  DX11 에서는 기본처리 되지 않으며 사용자가 직접 설정해야합니다 
//			     (OpenGL 에서는 예전부터 이렇게 해 왔습니다.)
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




/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// 장치/GPU 정보 획득 함수들
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// 장치/GPU 정보 획득
//
void FJRenderingEngine::GetDeviceInfo()
{
	//장치 기능레벨 확인.
	GetFeatureLevel();

	//GPU 정보 얻기.
	//...

	//모니터 정보 얻기.
	//...

}




/////////////////////////////////////////////////////////////////////////////
//
//  DX 기능 레벨 구하기.
//
static TCHAR* strFeature[4][4] =
{
	{ L"DX9",   L"DX9.1",  L"DX9.2", L"DX9.3" },
	{ L"DX10",  L"DX10.1", L"N/A",   L"N/A" },
	{ L"DX11",  L"DX11.1", L"N/A",   L"N/A" },
	{ L"DX12",  L"DX12.1"  L"N/A",   L"N/A" }
};

void FJRenderingEngine::GetFeatureLevel()
{
	UINT feat = g_setting.featureLevels;
	UINT ver = 0;
	UINT sub = 0;

#define OFFSET 0x9;

	ver = ((feat & 0xf000) >> 12) - OFFSET;	//메인 버전 산출.   	
	sub = ((feat & 0x0f00) >> 8);			//하위 버전 산출.

	g_setting.strFeatureLevels = strFeature[ver][sub];
}





////////////////////// 
//
//  에러 메세지 처리
//
//////////////////////
void FJRenderingEngine::FJErrorW(TCHAR* file, UINT line, TCHAR* func, BOOL bMBox, HRESULT hr, TCHAR* msg, ...)
{
	TCHAR msgva[1024] = L"";
	va_list vl;
	va_start(vl, msg);
	_vstprintf(msgva, sizeof(msgva), msg, vl);
	va_end(vl);


	// HRESULT 에서 에러 메세지 얻기.
	TCHAR herr[1024] = L"";
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, hr,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		herr, 1024, NULL);

	// 에러 메세지 붙이기.
	TCHAR errmsg[2048] = L"";
	/*_stprintf(errmsg, L"%s \nErrorCode(0x%X) : %s \nFile = %s (%d)\nFunc = %s\n\n",
		msgva, hr, herr,
		file, line, func);*/

	swprintf_s(errmsg, L"%s \nErrorCode(0x%X) : %s \nFile = %s (%d)\nFunc = %s\n\n", msgva, hr, herr, file, line, func);


	// (디버깅 중) VS 출력창으로 출력..
	OutputDebugString(L"\n");
	OutputDebugString(errmsg);

	//로그파일로 출력.
	//...

	// 메세지 창 출력..
	if (bMBox)
	{
		MessageBox(NULL, errmsg, L"Error", MB_OK | MB_ICONERROR);
		//GetLastError(hr);
	}
}




/////////////////////////////////////////////////////////////////////////////
//
//  장치-스왑체인/렌더타겟/깊이/스텐실 버퍼 클리어 
//
void FJRenderingEngine::ClearBackBuffer()
{
	// 렌더타겟 지우기
	_pDXDC->ClearRenderTargetView(_pRTView, (float*)&_clearCol);
}



/////////////////////////////////////////////////////////////////////////////
//
//  장치-스왑체인 전환 : 장면 그리기 완료시 꼭 호출해야 함.
//
void FJRenderingEngine::Flip()
{
	// 화면출력 : Flip!
	_pSwapChain->Present(0, 0);
}









//////////////////////////////////////////////////////////////
//
//  Get / Set 함수들
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