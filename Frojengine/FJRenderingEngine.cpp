#include "FJRenderingEngine.h"

FJRenderingEngine* FJRenderingEngine::_pInstance = nullptr;
COLOR FJRenderingEngine::_clearCol = COLOR(0.0f, 0.0f, 0.8f, 1.0f);
LPDEVICE Device::_pDevice;
LPDXDC Device::_pDXDC;

FJRenderingEngine::FJRenderingEngine()
	: _pDevice(nullptr), _pDXDC(nullptr), _pSwapChain(nullptr), _pRTView(nullptr), _pDS(nullptr), _pDSView(nullptr)
{
	
}



FJRenderingEngine::~FJRenderingEngine()
{
	DXRelease();
}


bool FJRenderingEngine::CreateRenderingEngine(HWND i_hWnd)
{
	bool result;

	result = DXSetup(i_hWnd);

	if (!result)
		return false;

	return true;
}


bool FJRenderingEngine::DXSetup(HWND i_hWnd)
{
	bool result;
	
	//--------------------------------------------------
	// 1단계 : 렌더링 장치 관련 필수 설정
	//--------------------------------------------------

	// D3D 렌더링 장치 Device 및 스왑체인 Swap Chain 생성. 
	result = CreateDeviceSwapChain(i_hWnd);

	Device::_pDevice = _pDevice;
	Device::_pDXDC = _pDXDC;

	if (!result)
		return false;

	// 장치-스왑체인의 렌더타겟(백버퍼) 획득
	result = CreateRenderTarget();

	if (!result)
		return false;

	// 깊이/스텐실 버퍼 생성.
	result = CreateDepthStencil();

	if (!result)
		return false;


	// 장치 출력병합기(Output Merger) 에 렌터링 타겟 및 깊이-스텐실 버퍼 등록.
	_pDXDC->OMSetRenderTargets(
		1,							// 렌더타겟 개수.(max: D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
		&_pRTView,					// 렌더타겟("백버퍼") 등록.	
		_pDSView					// 깊이/스텐실 버퍼 등록
	);
	
	// 뷰포트 설정.
	SetViewPort();

	//장치 정보 획득
	GetDeviceInfo();

	//깊이-스텐실 렌더링 상태 객체 생성.
	StateObjectCreate();

	//블렌드 상태 객체 생성
	BlendStateCreate();

	CTexture2D::SamplerCreate();

	//----------------------------------------
	// 2단계 : 기타 렌더링 관련 추가 설정.
	//----------------------------------------
	// 렌더링에 필요한 사용자 객체등을 생성/관리 합니다.
	// 카메라, 조명, 폰트, 셰이더 등등...
	CShader::CreateDefaultBuffer();

	return true;
}



///////////////////////////////////////////////////////////////////////////// 
//
//  디바이스 개체 및 관련 장치 제거 : 종료시 1회 호출.  
//  기본 렌더링 개체들 제거시 생성된 역순으로 처리합니다.(권장)
//
void FJRenderingEngine::DXRelease()
{
	CTexture2D::SamplerRelease();
	StateObjectRelease();
	BlendStateRelease();

	// 장치 상태 리셋 : 제거 전에 초기화를 해야 합니다. (메모리 누수 방지)
	if (_pDXDC)
		_pDXDC->ClearState();
	//if (_pSwapChain)
	//	_pSwapChain->SetFullscreenState(false, NULL);


	SAFE_RELEASE(_pDS);				// 깊이/스텐실 버퍼 제거.
	SAFE_RELEASE(_pDSView);
	SAFE_RELEASE(_pRTView);			// 렌더타겟 제거.
	SAFE_RELEASE(_pSwapChain);		// 스왑체인 제거.
	SAFE_RELEASE(_pDXDC);
	SAFE_RELEASE(_pDevice);			// 디바이스 제거. 맨 나중에 제거합니다.
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
	sd.SampleDesc = g_setting.sampleDesc;				//AA 설정
	
	
	
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
// 깊이-스텐실버퍼 생성. : DX11 에서는 렌더링버퍼-렌더타겟뷰- 와 마찬가지로, 
//                       깊이-스텐실버퍼역시 직접 만들어야 합니다.
//                       디바이스에 등록도 역시 수동입니다.
//
bool FJRenderingEngine::CreateDepthStencil()
{
	HRESULT hr = S_OK;

	//깊이/스텐실 버퍼용 정보 구성.
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = g_setting.displayMode.Width;
	td.Height = g_setting.displayMode.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	//td.Format = DXGI_FORMAT_D32_FLOAT;				// 32BIT. 깊이 버퍼
	//td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;		// 깊이 버퍼 (24bit) + 스텐실 (8bit) / 구형 하드웨어 (DX9)
	td.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;		// 깊이 버퍼 (32bit) + 스텐실 (8bit) / 신형 하드웨어 (DX11)
	td.SampleDesc = g_setting.sampleDesc;				// AA 설정 - RT 과 동일 규격 준수.
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;			// 깊이-스텐실 버퍼용으로 설정.
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;


	// 깊이 버퍼 생성.
	hr = _pDevice->CreateTexture2D(&td, NULL, &_pDS);
	if (FAILED(hr)) return false;


	// 깊이-스텐실버퍼용 리소스 뷰 정보 설정. 
	D3D11_DEPTH_STENCIL_VIEW_DESC  dd;
	ZeroMemory(&dd, sizeof(dd));
	dd.Format = td.Format;
	//dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; //AA 없음.
	dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS; //AA 적용.
	dd.Texture2D.MipSlice = 0;

	//깊이-스텐실 버퍼 뷰 생성.
	hr = _pDevice->CreateDepthStencilView(_pDS, &dd, &_pDSView);
	if (FAILED(hr))
	{
		FJError(hr, L"깊이/스텐실 버퍼 뷰 생성 실패.");
		return false;
	}

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



//----------------------------------------------------------------------------
// 상태 객체 State Objects (DX10/11)
//
// DX10 부터 구형 TnL 의 RenderState 가 제거되었습니다.
// 이를 대신하는 것이 상태객체 State Objects 인터페이스로, 렌더링 상태별 그룹으로 묶고
// 렌더링시 디바이스에 설정합니다.  이를 통해 장치의 여러 상태 변화를 한번에 설정하여 
// 불필요한 연산부하(Overhead) 를 줄이고 보다 효과적인 렌더링을 가능케 합니다.
// 상태객체는 엔진 초기시 제작후 사용하기를 권장하며 종료시 제거(Release) 해야 합니다.
// 상태객체는 수정불가능(Immutable, 읽기전용) 개체 입니다.
// DX9 에서는 State-Block 이 이와 유사한 기능을 담당합니다. 
//
// 상태 객체 인터페이스 중 '레스터라이즈 스테이지 Rasterize Stage' 상태 조절은 
// ID3D11RasterizerState 인터페이스를 통해 처리합니다.  
// 렌더링 설정/기능 모듬 정도로 생각합시다.
//----------------------------------------------------------------------------

void FJRenderingEngine::RasterStateLoad()
{
	RasterStateCreate(RS_SOLID);
	RasterStateCreate(RS_CULL_FRONT);
	RasterStateCreate(RS_CULL_BACK);
	RasterStateCreate(RS_WIREFRAME);
	RasterStateCreate(RS_WIREFRAME | RS_CULL_FRONT);
	RasterStateCreate(RS_WIREFRAME | RS_CULL_BACK);
}


void FJRenderingEngine::RasterStateCreate(byte flag)
{
	// 렌더링 상태 객체
	D3D11_RASTERIZER_DESC rd;
	rd.FillMode = flag & 0x80 ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;		// 삼각형 색상 채우기
																				// 백페이스 컬링
	switch (flag & 0x60)
	{
	case RS_CULL_NONE:
		rd.CullMode = D3D11_CULL_NONE;
		break;
	case RS_CULL_FRONT:
		rd.CullMode = D3D11_CULL_FRONT;
		break;
	case RS_CULL_BACK:
		rd.CullMode = D3D11_CULL_BACK;
		break;
	}
	rd.FrontCounterClockwise = flag & 0x10 ? true : false;
	rd.DepthBias = 0;
	rd.DepthBiasClamp = 0;
	rd.SlopeScaledDepthBias = 0;
	rd.DepthClipEnable = flag & 0x08 ? false : true;
	rd.ScissorEnable = flag & 0x04 ? true : false;
	rd.MultisampleEnable = flag & 0x02 ? true : false;
	rd.AntialiasedLineEnable = flag & 0x01 ? true : false;

	_pDevice->CreateRasterizerState(&rd, &_RSStateMap[flag]);
}



void FJRenderingEngine::RasterStateRelease()
{
	FOR_STL(_RSStateMap)
	{
		SAFE_RELEASE((*iter).second);
	}

	_RSStateMap.clear();
}




///////////////////////////////////////////////////////////////////////////////
//
//  깊이/스텐실 버퍼 상태객체 생성. 
//
void FJRenderingEngine::DSStateLoad()
{
	//----------------------------
	// 깊이/스텐실 상태 개체 생성.: "출력병합기 Output Merger" 상태 조절. 
	//----------------------------

	DSStateCreate(0);
	DSStateCreate(DS_DEPTH_TEST_OFF);
	DSStateCreate(DS_DEPTH_WRITE_OFF);
	DSStateCreate(DS_DEPTH_TEST_OFF | DS_DEPTH_WRITE_OFF);
	DSStateCreate(DS_STENCIL_ON | DS_STENCIL_FRONT_PASS_OP_REPLACE | DS_STENCIL_BACK_PASS_OP_REPLACE);
	DSStateCreate(DS_STENCIL_ON | DS_STENCIL_FRONT_COMPARISON_EQUAL | DS_STENCIL_BACK_COMPARISON_EQUAL);
	DSStateCreate(DS_STENCIL_ON | DS_STENCIL_FRONT_COMPARISON_NOT_EQUAL | DS_STENCIL_BACK_COMPARISON_NOT_EQUAL);

	/*// DS 상태객체 #7 : 깊이버퍼 Off, 스텐실버퍼 ON (참조값 쓰기) : "스텐실만 기록" 
	ds.DepthEnable	  = FALSE;										//깊이버퍼 OFF!
	ds.StencilEnable = TRUE;										//스텐실 버퍼 ON! 
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;		//비교함수 : "항상 통과" (성공)
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_REPLACE;		//성공시 : 참조값(Stencil Reference Value) 로 교체.
	ds.BackFace = ds.FrontFace;										//뒷면 설정 동일.
	_pDevice->CreateDepthStencilState(&ds, &_pDSState[DS_DEPTH_OFF_STENCIL_ON]);
	*/
	
	/*// DS 상태객체 #8 : 깊이버퍼 On, 스텐실버퍼 ON (동일비교, 성시 증가) : "이중그리기 방지용" 
	//ds.DepthEnable	= TRUE;										//깊이버퍼 ON! (기본값)(생략)
	//ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//ds.DepthFunc		= D3D11_COMPARISON_LESS;
	ds.StencilEnable = TRUE;										//스텐실 버퍼 ON! 
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_EQUAL;		//비교함수 : "동일한가?" 
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_INCR;		//성공시 : 증가 (+1) 
	ds.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;		//실패시 : 유지.
	ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		//실패시 : 유지.
	ds.BackFace = ds.FrontFace;										//뒷면 설정 동일.
	_pDevice->CreateDepthStencilState(&ds, &_pDSState[DS_DEPTH_ON_STENCIL_EQUAL_INCR]);
	*/

	/*// DS 상태객체 #9 : 깊이버퍼 On, 스텐실버퍼 ON (항상, 성공시 증가) : "스텐실만 기록" 
	ds.DepthEnable	  = TRUE;										//깊이버퍼 ON! (기본값)
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;				//깊이버퍼 쓰기 OFF.
	ds.DepthFunc	  = D3D11_COMPARISON_LESS;						//깊이연산 ON. (기본값)
	ds.StencilEnable  = TRUE;										//스텐실 버퍼 ON! 
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;		//비교함수 : "항상 통과" (성공)
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_REPLACE;		//성공시 : 참조값(Stencil Reference Value) 로 교체.
	ds.BackFace = ds.FrontFace;										//뒷면 설정 동일.
	_pDevice->CreateDepthStencilState(&ds, &_pDSState[DS_DEPTH_WRITE_OFF_STENCIL_ON]);
	*/
}


void FJRenderingEngine::DSStateCreate(DWORD flag)
{
	D3D11_DEPTH_STENCIL_DESC ds;
	ds.DepthEnable = flag & 0x80000000 ? FALSE : TRUE;
	ds.DepthWriteMask = flag & 0x40000000 ? D3D11_DEPTH_WRITE_MASK_ZERO : D3D11_DEPTH_WRITE_MASK_ALL;
	switch (flag & 0x38000000)
	{
	case DS_DEPTH_COMPARISON_NEVER:
		ds.DepthFunc = D3D11_COMPARISON_NEVER;
		break;
	case DS_DEPTH_COMPARISON_LESS:
		ds.DepthFunc = D3D11_COMPARISON_LESS;
		break;
	case DS_DEPTH_COMPARISON_EQUAL:
		ds.DepthFunc = D3D11_COMPARISON_EQUAL;
		break;
	case DS_DEPTH_COMPARISON_LESS_EQUAL:
		ds.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		break;
	case DS_DEPTH_COMPARISON_GREATER:
		ds.DepthFunc = D3D11_COMPARISON_GREATER;
		break;
	case DS_DEPTH_COMPARISON_NOT_EQUAL:
		ds.DepthFunc = D3D11_COMPARISON_NOT_EQUAL;
		break;
	case DS_DEPTH_COMPARISON_GREATER_EQUAL:
		ds.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
		break;
	case DS_DEPTH_COMPARISON_ALWAYS:
		ds.DepthFunc = D3D11_COMPARISON_LESS;
		break;
	}

	//----------------------------------------------------------------------
	// 스텐실 버퍼 연산 객체들 생성.★
	//----------------------------------------------------------------------
	// 스텐실 버퍼 비트 연산 공식.
	// (Stencil.Ref & Stencil.Mask) Comparison-Func ( StencilBuffer.Value & Stencil.Mask)
	//
	// *StencilBufferValue : 현재 검사할 픽셀의 스텐실값.
	// *ComFunc : 비교 함수. ( > < >= <= ==  Always Never)
	//----------------------------------------------------------------------

	// 스텐실 버퍼 설정
	ds.StencilEnable = flag & 0x04000000 ? TRUE : FALSE;		// 스텐실 버퍼를 사용 할 것인가.
	ds.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;		// 스텐실 읽기 마스크 (8bit: 0xff)
	ds.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;		// 스텐실 쓰기 마스크 (8bit: 0xff)
	// [앞면] 스텐실 비교 실패시 동작
	switch (flag & 0x03800000)
	{
	case DS_STENCIL_FRONT_FAIL_OP_KEEP:
		ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		break;
	case DS_STENCIL_FRONT_FAIL_OP_ZERO:
		ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		break;
	case DS_STENCIL_FRONT_FAIL_OP_REPLACE:
		ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
		break;
	case DS_STENCIL_FRONT_FAIL_OP_INCR_SAT:
		ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR_SAT;
		break;
	case DS_STENCIL_FRONT_FAIL_OP_DECR_SAT:
		ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_DECR_SAT;
		break;
	case DS_STENCIL_FRONT_FAIL_OP_INVERT:
		ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INVERT;
		break;
	case DS_STENCIL_FRONT_FAIL_OP_INCR:
		ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
		break;
	case DS_STENCIL_FRONT_FAIL_OP_DECR:
		ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_DECR;
		break;
	}
	// [앞면] 스텐실/깊이 비교 실패시 동작
	switch (flag & 0x00700000)
	{
	case DS_STENCIL_FRONT_DEPTH_FAIL_OP_KEEP:
		ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		break;
	case DS_STENCIL_FRONT_DEPTH_FAIL_OP_ZERO:
		ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		break;
	case DS_STENCIL_FRONT_DEPTH_FAIL_OP_REPLACE:
		ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
		break;
	case DS_STENCIL_FRONT_DEPTH_FAIL_OP_INCR_SAT:
		ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR_SAT;
		break;
	case DS_STENCIL_FRONT_DEPTH_FAIL_OP_DECR_SAT:
		ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR_SAT;
		break;
	case DS_STENCIL_FRONT_DEPTH_FAIL_OP_INVERT:
		ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INVERT;
		break;
	case DS_STENCIL_FRONT_DEPTH_FAIL_OP_INCR:
		ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		break;
	case DS_STENCIL_FRONT_DEPTH_FAIL_OP_DECR:
		ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		break;
	}
	// [앞면] 스텐실 비교 성공시 동작
	switch (flag & 0x000E0000)
	{
	case DS_STENCIL_FRONT_PASS_OP_KEEP:
		ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		break;
	case DS_STENCIL_FRONT_PASS_OP_ZERO:
		ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		break;
	case DS_STENCIL_FRONT_PASS_OP_REPLACE:
		ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		break;
	case DS_STENCIL_FRONT_PASS_OP_INCR_SAT:
		ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
		break;
	case DS_STENCIL_FRONT_PASS_OP_DECR_SAT:
		ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_DECR_SAT;
		break;
	case DS_STENCIL_FRONT_PASS_OP_INVERT:
		ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INVERT;
		break;
	case DS_STENCIL_FRONT_PASS_OP_INCR:
		ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
		break;
	case DS_STENCIL_FRONT_PASS_OP_DECR:
		ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_DECR;
		break;
	}
	// [앞면] 스텐실 비교 함수
	switch (flag & 0x0001C000)
	{
	case DS_STENCIL_FRONT_COMPARISON_NEVER:
		ds.FrontFace.StencilFunc = D3D11_COMPARISON_NEVER;
		break;
	case DS_STENCIL_FRONT_COMPARISON_LESS:
		ds.FrontFace.StencilFunc = D3D11_COMPARISON_LESS;
		break;
	case DS_STENCIL_FRONT_COMPARISON_EQUAL:
		ds.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		break;
	case DS_STENCIL_FRONT_COMPARISON_LESS_EQUAL:
		ds.FrontFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;
		break;
	case DS_STENCIL_FRONT_COMPARISON_GREATER:
		ds.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER;
		break;
	case DS_STENCIL_FRONT_COMPARISON_NOT_EQUAL:
		ds.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		break;
	case DS_STENCIL_FRONT_COMPARISON_GREATER_EQUAL:
		ds.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;
		break;
	case DS_STENCIL_FRONT_COMPARISON_ALWAYS:
		ds.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		break;
	}
	// [뒷면] 스텐실 비교 실패시 동작
	switch (flag & 0x00003800)
	{
	case DS_STENCIL_BACK_FAIL_OP_KEEP:
		ds.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		break;
	case DS_STENCIL_BACK_FAIL_OP_ZERO:
		ds.BackFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
		break;
	case DS_STENCIL_BACK_FAIL_OP_REPLACE:
		ds.BackFace.StencilFailOp = D3D11_STENCIL_OP_REPLACE;
		break;
	case DS_STENCIL_BACK_FAIL_OP_INCR_SAT:
		ds.BackFace.StencilFailOp = D3D11_STENCIL_OP_INCR_SAT;
		break;
	case DS_STENCIL_BACK_FAIL_OP_DECR_SAT:
		ds.BackFace.StencilFailOp = D3D11_STENCIL_OP_DECR_SAT;
		break;
	case DS_STENCIL_BACK_FAIL_OP_INVERT:
		ds.BackFace.StencilFailOp = D3D11_STENCIL_OP_INVERT;
		break;
	case DS_STENCIL_BACK_FAIL_OP_INCR:
		ds.BackFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
		break;
	case DS_STENCIL_BACK_FAIL_OP_DECR:
		ds.BackFace.StencilFailOp = D3D11_STENCIL_OP_DECR;
		break;
	}
	// [뒷면] 스텐실/깊이 비교 실패시 동작
	switch (flag & 0x00000700)
	{
	case DS_STENCIL_BACK_DEPTH_FAIL_OP_KEEP:
		ds.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		break;
	case DS_STENCIL_BACK_DEPTH_FAIL_OP_ZERO:
		ds.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
		break;
	case DS_STENCIL_BACK_DEPTH_FAIL_OP_REPLACE:
		ds.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;
		break;
	case DS_STENCIL_BACK_DEPTH_FAIL_OP_INCR_SAT:
		ds.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR_SAT;
		break;
	case DS_STENCIL_BACK_DEPTH_FAIL_OP_DECR_SAT:
		ds.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR_SAT;
		break;
	case DS_STENCIL_BACK_DEPTH_FAIL_OP_INVERT:
		ds.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INVERT;
		break;
	case DS_STENCIL_BACK_DEPTH_FAIL_OP_INCR:
		ds.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		break;
	case DS_STENCIL_BACK_DEPTH_FAIL_OP_DECR:
		ds.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		break;
	}
	// [뒷면] 스텐실 비교 성공시 동작
	switch (flag & 0x000000E0)
	{
	case DS_STENCIL_BACK_PASS_OP_KEEP:
		ds.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		break;
	case DS_STENCIL_BACK_PASS_OP_ZERO:
		ds.BackFace.StencilPassOp = D3D11_STENCIL_OP_ZERO;
		break;
	case DS_STENCIL_BACK_PASS_OP_REPLACE:
		ds.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
		break;
	case DS_STENCIL_BACK_PASS_OP_INCR_SAT:
		ds.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
		break;
	case DS_STENCIL_BACK_PASS_OP_DECR_SAT:
		ds.BackFace.StencilPassOp = D3D11_STENCIL_OP_DECR_SAT;
		break;
	case DS_STENCIL_BACK_PASS_OP_INVERT:
		ds.BackFace.StencilPassOp = D3D11_STENCIL_OP_INVERT;
		break;
	case DS_STENCIL_BACK_PASS_OP_INCR:
		ds.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
		break;
	case DS_STENCIL_BACK_PASS_OP_DECR:
		ds.BackFace.StencilPassOp = D3D11_STENCIL_OP_DECR;
		break;
	}
	// [뒷면] 스텐실 비교 함수
	switch (flag & 0x0000001C)
	{
	case DS_STENCIL_BACK_COMPARISON_NEVER:
		ds.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
		break;
	case DS_STENCIL_BACK_COMPARISON_LESS:
		ds.BackFace.StencilFunc = D3D11_COMPARISON_LESS;
		break;
	case DS_STENCIL_BACK_COMPARISON_EQUAL:
		ds.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		break;
	case DS_STENCIL_BACK_COMPARISON_LESS_EQUAL:
		ds.BackFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;
		break;
	case DS_STENCIL_BACK_COMPARISON_GREATER:
		ds.BackFace.StencilFunc = D3D11_COMPARISON_GREATER;
		break;
	case DS_STENCIL_BACK_COMPARISON_NOT_EQUAL:
		ds.BackFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		break;
	case DS_STENCIL_BACK_COMPARISON_GREATER_EQUAL:
		ds.BackFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;
		break;
	case DS_STENCIL_BACK_COMPARISON_ALWAYS:
		ds.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		break;
	}

	_pDevice->CreateDepthStencilState(&ds, &_DSStateMap[flag]);
}




///////////////////////////////////////////////////////////////////////////////
//
//  깊이/스텐실 버퍼 상태객체 제거하기 : 엔진 종료시 1회 호출.
//
void FJRenderingEngine::DSStateRelease()
{
	FOR_STL(_DSStateMap)
	{
		SAFE_RELEASE((*iter).second);
	}

	_DSStateMap.clear();
}




////////////////////////////////////////////////////////////////////////////// 
//
// 장치 렌더링 상태 객체 생성.
//
//----------------------------------------------------------------------------
// 상태 객체 State Objects (DX10/11)
// DX10 부터 구형 TnL 의 RenderState 가 제거되었습니다.
// 이를 대신하는 것이 상태객체 State Objects 인터페이스로, 렌더링 상태를 하나의 그룹으로 
// 묶고 렌더링시 디바이스에 설정합니다.  이를 통해 장치의 어려 상태 변화를 한번에 설정하여 
// 불필요한 연산부하(Overhead) 를 줄이고 보다 효과적인 렌더링을 가능케 합니다.
//
// 상태객체는 엔진 초기시 제작후 사용하기를 권장하며 종료시 제거(Release) 해야 합니다.
// 상태객체는 수정불가능(Immutable, 읽기전용) 개체 입니다.
// DX9 에서는 State-Block 이 이와 유사한 기능을 담당했었습니다.
//
// 상태 객체 인터페이스 중 '레스터라이즈 스테이지 Rasterize Stage' 상태 조절은 
// ID3D11RasterizerState 인터페이스를 통해 처리합니다.  
// 간단하게 렌더링 설정/기능 모듬 정도로 생각합시다.  자세한 것은 다음을 참조하십시요. 
// 링크1 : 상태 객체 https://msdn.microsoft.com/en-us/library/windows/desktop/bb205071(v=vs.85).aspx
// 링크2 : 깊이버퍼 상태 구성하기 https://msdn.microsoft.com/ko-kr/library/windows/desktop/bb205074(v=vs.85).aspx#Create_Depth_Stencil_State 
//----------------------------------------------------------------------------
//
void FJRenderingEngine::StateObjectCreate()
{

	//----------------------------
	// 레스터 상태 개체 생성 : "레스터라이즈 스테이지 Rasterize Stage" 상태 조절.
	//----------------------------
	RasterStateLoad();

	//----------------------------
	// 깊이/스텐실 상태 개체 생성.: "출력병합기 Output Merger" 상태 조절. 
	//----------------------------
	DSStateLoad();



	//----------------------------
	// 알파블렌딩 상태 개체 생성 : "출력병합기 Output Merger" 상태 조절. 
	//----------------------------
	//...


}



///////////////////////////////////////////////////////////////////////////////
//
//  상태객체 제거하기 : 엔진 종료시 1회 호출.
//
void FJRenderingEngine::StateObjectRelease()
{
	RasterStateRelease();	//레스터 상태 개체 제거.
	DSStateRelease();		//깊이/스텐실 상태 개체 제거.
}




////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
//
// 블렌딩 상태 객체 관련 함수들.
//
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
// 
// 블렌딩 상태 객체 생성.
//
void FJRenderingEngine::BlendStateCreate()
{
	//블렌딩 상태 객체 구성 옵션 : 이하 기본값들.
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	
	// [알림] 다중 렌터타겟(RenderTarget) 사용시 각 RT 별 지정이 가능하나
	// 당분간 RT #0 (기본)-백버퍼-만을 사용하겠습니다.★
	//
	bd.RenderTarget[0].BlendEnable = FALSE;							//블렌딩 동작 결정. 기본값은 FALSE(OFF)★	
	
	//색상 성분 혼합 : Color Blending.(기본값)★
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//색상 혼합 연산(Color - Operation), 기본값은 덧셈 : 최종색 = Src.Color + Dest.Color ★	
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;					//소스(원본) 혼합 비율, 원본 100% : Src.Color = Src * 1;★
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;				//타겟(대상) 혼합 비율, 대상 0%   : Dest.Color = Dest * 0;  통상 0번 RT 는 "백버퍼"를 의미합니다.★	

	//알파 성분 혼합 : Alpha Blending.(기본값)
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;			//알파 혼합 함수(Alpha - Opertion), 기본값은 덧셈.
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;				//소스(원본) 알파 혼합 비율.
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;			//타겟(대상) 알파 혼합 비율.
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;	//렌더타겟에 쓰기 옵션.
	//이하 추가 옵션.(기본값)	
	//bd.AlphaToCoverageEnable = FALSE;
	//bd.IndependentBlendEnable = FALSE;

	//[상태객체 1] 기본 블렌딩 상태 개체.생성.
	_pDevice->CreateBlendState(&bd, &_pBState[BS_DEFAULT]);




	//------------------------------
	//[상태객체2] 색상 혼합 개체 생성.
	//------------------------------
	bd.RenderTarget[0].BlendEnable = TRUE;							//색상 혼합 ON! ★	
	//bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//색상 혼합 연산 (덧셈, 기본값) : 최종색 = Src.Color + Dest.Color ★	

	//[혼합 테스트1] 100% 혼합★
	//bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;					//소스(원본) 혼합 비율, 원본 100% : Src.Color = Src * 1;
	//bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;					//타겟(대상) 혼합 비율, 대상 100% : Dest.Color = Dest * 1;
		
	//[혼합 테스트2] 색상비 혼합★
	//bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;			//소스(원본) 혼합 비율, 원본 색상비 적용 : Src.Color = Src * Src;
	//bd.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_COLOR;		//타겟(대상) 혼합 비율, 대상 색상비 적용 : Dest.Color = Dest * Dest;
	
	//[혼합 테스트3] 색상비 혼합2★
	//bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;			//소스(원본) 혼합 비율, 원본 색상비 적용 : Src.Color = Src * Src;
	//bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_DEST_COLOR;	//타겟(대상) 혼합 비율, 대상 색상비 적용 : Dest.Color = Dest * (1-Dest);

	//[혼합 테스트4] 색상비 혼합3★
	//bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;			//소스(원본) 혼합 비율, 원본 색상비 적용 : Src.Color = Src * Src;
	//bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;				//타겟(대상) 혼합 비율, 대상 100% 적용  : Dest.Color = Dest * 1;
		
	
	//#1 흰색 빼기 : 배경과 곱셈 : OP 이 지원되는 것은 아니나, 파이프라인의 기능을 적극적으로 활용해 보자.★
	//bd.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_COLOR;				//소스(원본) 혼합 비율, 원본 100% : Src.Color = Src * Dest;
	//bd.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;					//타겟(대상) 혼합 비율, 대상 0% : Dest.Color = Dest * 0;			 	
	

	//------------------------------
	// 알파기준 색상 혼합 : Alpha Blending ON! ★
	//------------------------------
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;			//소스(원본) 혼합 비율, 원본 알파비율  : Src.Color = Src * Src.a;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;		//타겟(대상) 혼합 비율, 원본 알파비율 반전 : Dest.Color = Dest * (1-Src.a);			 	

	_pDevice->CreateBlendState(&bd, &_pBState[BS_ADD]);
}




////////////////////////////////////////////////////////////////////////////// 
//
// 블렌딩 상태 객체 제거 : 어플리케이션 종료시 호출.
//
void FJRenderingEngine::BlendStateRelease()
{
	for (int i = 0; i < BS_MAX_; i++)
	{
		SAFE_RELEASE(_pBState[i]);
	}

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
	_pDXDC->ClearDepthStencilView(_pDSView, D3D11_CLEAR_DEPTH, 1.0f, 0);	//깊이/스텐실 지우기.
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


FJRenderingEngine* FJRenderingEngine::GetInstance()
{
	if (_pInstance == nullptr)
	{
		_pInstance = new FJRenderingEngine();
	}

	return _pInstance;
}









//////////////////////////////////////////////////////////////
//
//  Get / Set 함수들
//
//////////////////////////////////////////////////////////////

void FJRenderingEngine::SetRSState(byte flag)
{
	FJRenderingEngine* pRenderer = GetInstance();
	if (pRenderer->_RSStateMap.find(flag) == pRenderer->_RSStateMap.end())
	{
		pRenderer->RasterStateCreate(flag);
	}

	pRenderer->_pDXDC->RSSetState(pRenderer->_RSStateMap[flag]);
}

void FJRenderingEngine::SetDSState(DWORD flag, UINT stencilRef)
{
	FJRenderingEngine* pRenderer = GetInstance();
	if (pRenderer->_DSStateMap.find(flag) == pRenderer->_DSStateMap.end())
	{
		pRenderer->DSStateCreate(flag);
	}

	pRenderer->_pDXDC->OMSetDepthStencilState(pRenderer->_DSStateMap[flag], stencilRef);
}

void FJRenderingEngine::SetClearColor(COLOR& i_col)
{
	_clearCol = i_col;
}


COLOR FJRenderingEngine::GetClearColor()
{
	return _clearCol;
}