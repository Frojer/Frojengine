#include "FJRenderingEngine.h"

COLOR FJRenderingEngine::_clearCol = COLOR(0.0f, 0.0f, 0.8f, 1.0f);
byte FJRenderingEngine::_rsData = 0;
byte FJRenderingEngine::_dsData = 0;
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
	// 1�ܰ� : ������ ��ġ ���� �ʼ� ����
	//--------------------------------------------------

	// D3D ������ ��ġ Device �� ����ü�� Swap Chain ����. 
	result = CreateDeviceSwapChain(i_hWnd);

	Device::_pDevice = _pDevice;
	Device::_pDXDC = _pDXDC;

	if (!result)
		return false;

	// ��ġ-����ü���� ����Ÿ��(�����) ȹ��
	result = CreateRenderTarget();

	if (!result)
		return false;

	// ����/���ٽ� ���� ����.
	result = CreateDepthStencil();

	if (!result)
		return false;


	// ��ġ ��º��ձ�(Output Merger) �� ���͸� Ÿ�� �� ����-���ٽ� ���� ���.
	_pDXDC->OMSetRenderTargets(
		1,							// ����Ÿ�� ����.(max: D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT)
		&_pRTView,					// ����Ÿ��("�����") ���.	
		_pDSView					// ����/���ٽ� ���� ���
	);
	
	// ����Ʈ ����.
	SetViewPort();

	//��ġ ���� ȹ��
	GetDeviceInfo();

	//����-���ٽ� ������ ���� ��ü ����.
	StateObjectCreate();

	//���� ���� ��ü ����
	BlendStateCreate();

	CTexture2D::SamplerCreate();

	//----------------------------------------
	// 2�ܰ� : ��Ÿ ������ ���� �߰� ����.
	//----------------------------------------
	// �������� �ʿ��� ����� ��ü���� ����/���� �մϴ�.
	// ī�޶�, ����, ��Ʈ, ���̴� ���...
	CShader::CreateDefaultBuffer();

	return true;
}



///////////////////////////////////////////////////////////////////////////// 
//
//  ����̽� ��ü �� ���� ��ġ ���� : ����� 1ȸ ȣ��.  
//  �⺻ ������ ��ü�� ���Ž� ������ �������� ó���մϴ�.(����)
//
void FJRenderingEngine::DXRelease()
{
	CTexture2D::SamplerRelease();
	StateObjectRelease();
	BlendStateRelease();

	// ��ġ ���� ���� : ���� ���� �ʱ�ȭ�� �ؾ� �մϴ�. (�޸� ���� ����)
	if (_pDXDC)
		_pDXDC->ClearState();
	//if (_pSwapChain)
	//	_pSwapChain->SetFullscreenState(false, NULL);


	SAFE_RELEASE(_pDS);				// ����/���ٽ� ���� ����.
	SAFE_RELEASE(_pDSView);
	SAFE_RELEASE(_pRTView);			// ����Ÿ�� ����.
	SAFE_RELEASE(_pSwapChain);		// ����ü�� ����.
	SAFE_RELEASE(_pDXDC);
	SAFE_RELEASE(_pDevice);			// ����̽� ����. �� ���߿� �����մϴ�.
}





/////////////////////////////////////////////////////////////////////////////
//
//  D3D ������ '��ġ(Device)' �� ����ü�� ����. 
//
bool FJRenderingEngine::CreateDeviceSwapChain(HWND i_hWnd)
{
	HRESULT hr = S_OK;

	// D3D ��� ���� (Direct3D feature level) : DX11 ������ HW ������ ȣȯ�� ���
	g_setting.featureLevels = D3D_FEATURE_LEVEL_11_0;		// DX11 ����.
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
	sd.SampleDesc = g_setting.sampleDesc;				//AA ����
	
	
	
	// D3D ������ '��ġ(Device)' �� ����ü�� ����. 
	hr = D3D11CreateDeviceAndSwapChain(
			NULL,						// ���� ��� ������ (�⺻��ġ�� NULL)
			D3D_DRIVER_TYPE_HARDWARE,	// HW ���� 
			NULL,						// SW Rasterizer DLL �ڵ�.  HW ���ӽÿ��� NULL.
			0,							// ����̽� ���� �÷���.(�⺻��)
			//D3D11_CREATE_DEVICE_DEBUG,// ����̽� ���� �÷���.(�����)
			&g_setting.featureLevels,	// (������) ����̽� ��� ����(Feature Level) �迭
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
// ����-���ٽǹ��� ����. : DX11 ������ ����������-����Ÿ�ٺ�- �� ����������, 
//                       ����-���ٽǹ��ۿ��� ���� ������ �մϴ�.
//                       ����̽��� ��ϵ� ���� �����Դϴ�.
//
bool FJRenderingEngine::CreateDepthStencil()
{
	HRESULT hr = S_OK;

	//����/���ٽ� ���ۿ� ���� ����.
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = g_setting.displayMode.Width;
	td.Height = g_setting.displayMode.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	//td.Format = DXGI_FORMAT_D32_FLOAT;				// 32BIT. ���� ����
	//td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;		// ���� ���� (24bit) + ���ٽ� (8bit) / ���� �ϵ���� (DX9)
	td.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;		// ���� ���� (32bit) + ���ٽ� (8bit) / ���� �ϵ���� (DX11)
	td.SampleDesc = g_setting.sampleDesc;				// AA ���� - RT �� ���� �԰� �ؼ�.
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;			// ����-���ٽ� ���ۿ����� ����.
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;


	// ���� ���� ����.
	hr = _pDevice->CreateTexture2D(&td, NULL, &_pDS);
	if (FAILED(hr)) return false;


	// ����-���ٽǹ��ۿ� ���ҽ� �� ���� ����. 
	D3D11_DEPTH_STENCIL_VIEW_DESC  dd;
	ZeroMemory(&dd, sizeof(dd));
	dd.Format = td.Format;
	//dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D; //AA ����.
	dd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS; //AA ����.
	dd.Texture2D.MipSlice = 0;

	//����-���ٽ� ���� �� ����.
	hr = _pDevice->CreateDepthStencilView(_pDS, &dd, &_pDSView);
	if (FAILED(hr))
	{
		FJError(hr, L"����/���ٽ� ���� �� ���� ����.");
		return false;
	}

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




/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// ��ġ/GPU ���� ȹ�� �Լ���
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// ��ġ/GPU ���� ȹ��
//
void FJRenderingEngine::GetDeviceInfo()
{
	//��ġ ��ɷ��� Ȯ��.
	GetFeatureLevel();

	//GPU ���� ���.
	//...

	//����� ���� ���.
	//...

}




/////////////////////////////////////////////////////////////////////////////
//
//  DX ��� ���� ���ϱ�.
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

	ver = ((feat & 0xf000) >> 12) - OFFSET;	//���� ���� ����.   	
	sub = ((feat & 0x0f00) >> 8);			//���� ���� ����.

	g_setting.strFeatureLevels = strFeature[ver][sub];
}



//----------------------------------------------------------------------------
// ���� ��ü State Objects (DX10/11)
//
// DX10 ���� ���� TnL �� RenderState �� ���ŵǾ����ϴ�.
// �̸� ����ϴ� ���� ���°�ü State Objects �������̽���, ������ ���º� �׷����� ����
// �������� ����̽��� �����մϴ�.  �̸� ���� ��ġ�� ���� ���� ��ȭ�� �ѹ��� �����Ͽ� 
// ���ʿ��� �������(Overhead) �� ���̰� ���� ȿ������ �������� ������ �մϴ�.
// ���°�ü�� ���� �ʱ�� ������ ����ϱ⸦ �����ϸ� ����� ����(Release) �ؾ� �մϴ�.
// ���°�ü�� �����Ұ���(Immutable, �б�����) ��ü �Դϴ�.
// DX9 ������ State-Block �� �̿� ������ ����� ����մϴ�. 
//
// ���� ��ü �������̽� �� '�����Ͷ����� �������� Rasterize Stage' ���� ������ 
// ID3D11RasterizerState �������̽��� ���� ó���մϴ�.  
// ������ ����/��� ��� ������ �����սô�.
//----------------------------------------------------------------------------

void FJRenderingEngine::RasterStateLoad()
{
	// ������ ���� ��ü
	D3D11_RASTERIZER_DESC rd;
	rd.FillMode = D3D11_FILL_SOLID;		// �ﰢ�� ���� ä���.(�⺻��)
	rd.CullMode = D3D11_CULL_NONE;		// �����̽� �ø� (�⺻��)		
	rd.FrontCounterClockwise = false;   // ���� �⺻��...
	rd.DepthBias = 0;
	rd.DepthBiasClamp = 0;
	rd.SlopeScaledDepthBias = 0;
	rd.DepthClipEnable = true;
	rd.ScissorEnable = false;
	rd.MultisampleEnable = false;
	rd.AntialiasedLineEnable = false;

	// �����Ͷ����� ��ü ����.
	_pDevice->CreateRasterizerState(&rd, &_pRState[RS_SOLID]);

	rd.CullMode = D3D11_CULL_BACK;
	_pDevice->CreateRasterizerState(&rd, &_pRState[RS_SOLID_CULL_BACK]);

	rd.CullMode = D3D11_CULL_FRONT;
	_pDevice->CreateRasterizerState(&rd, &_pRState[RS_SOLID_CULL_FRONT]);

	// ���̾� ������ �׸���. 
	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.CullMode = D3D11_CULL_NONE;
	_pDevice->CreateRasterizerState(&rd, &_pRState[RS_WIRE]);

	rd.CullMode = D3D11_CULL_BACK;
	_pDevice->CreateRasterizerState(&rd, &_pRState[RS_WIRE_CULL_BACK]);

	rd.CullMode = D3D11_CULL_FRONT;
	_pDevice->CreateRasterizerState(&rd, &_pRState[RS_WIRE_CULL_FRONT]);
}



void FJRenderingEngine::RasterStateUpdate()
{
	switch (_rsData)
	{
	case RM_SOLID | RM_CULLNONE:
		_pDXDC->RSSetState(_pRState[RS_SOLID]);
		break;

	case RM_WIRE | RM_CULLNONE:
		_pDXDC->RSSetState(_pRState[RS_WIRE]);
		break;

	case RM_SOLID | RM_CULLBACK:
		_pDXDC->RSSetState(_pRState[RS_SOLID_CULL_BACK]);
		break;

	case RM_WIRE | RM_CULLBACK:
		_pDXDC->RSSetState(_pRState[RS_WIRE_CULL_BACK]);
		break;

	case RM_SOLID | RM_CULLFRONT:
		_pDXDC->RSSetState(_pRState[RS_SOLID_CULL_FRONT]);
		break;

	case RM_WIRE | RM_CULLFRONT:
		_pDXDC->RSSetState(_pRState[RS_WIRE_CULL_BACK]);
		break;
	}
}



void FJRenderingEngine::RasterStateRelease()
{
	for (int i = 0; i < RS_MAX_; i++)
	{
		SAFE_RELEASE(_pRState[i]);
	}
}




///////////////////////////////////////////////////////////////////////////////
//
//  ����/���ٽ� ���� ���°�ü ����. 
//
void FJRenderingEngine::DSStateLoad()
{
	//----------------------------
	// ����/���ٽ� ���� ��ü ����.: "��º��ձ� Output Merger" ���� ����. 
	//----------------------------
	//...	 
	D3D11_DEPTH_STENCIL_DESC  ds;
	ds.DepthEnable = TRUE;
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ds.DepthFunc = D3D11_COMPARISON_LESS;
	// ���ٽ� ���� ���� (�⺻��)
	ds.StencilEnable = FALSE;									// ���ٽ� ���� OFF.
	ds.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;		// ���ٽ� �б� ����ũ (8bit: 0xff)
	ds.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;		// ���ٽ� ���� ����ũ (8bit: 0xff)
	ds.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;			// [�ո�] ���ٽ� �� �Լ� : "Always" ��, �׻� ���� (���, pass)
	ds.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;			// [�ո�] ���ٽ� �� ������ ���� : ������ ����.
	ds.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;			// [�ո�] ���ٽ� �� ���н� ���� : ������ ����.	
	ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	// [�ո�] ���ٽ�/���� �� ���н� ���� : ������ ����.
	ds.BackFace = ds.FrontFace;									// [�޸�] ���� ����. �ʿ�� ���� ������ ����.
	//...
	// ù��° ���� ��ü : Z-Test ON! (�⺻��)
	_pDevice->CreateDepthStencilState(&ds, &_pDSState[DS_DT_ON_DW_ON_ST_OFF]);

	// �ι�° ���� ��ü : Z-Test OFF ����.
	ds.DepthEnable = FALSE;
	_pDevice->CreateDepthStencilState(&ds, &_pDSState[DS_DT_OFF_DW_ON_ST_OFF]);

	// ����° ���� ��ü : Z-Test On + Z-Write OFF.
	// Z-Test (ZEnable, DepthEnable) �� ������, Z-Write ���� ��Ȱ��ȭ �˴ϴ�.
	ds.DepthEnable = TRUE;
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;		// ���̰� ���� ��.
	_pDevice->CreateDepthStencilState(&ds, &_pDSState[DS_DT_ON_DW_OFF_ST_OFF]);

	// �׹�° ���� ��ü : Z-Test Off + Z-Write OFF.
	// Z-Test (ZEnable, DepthEnable) �� ������, Z-Write ���� ��Ȱ��ȭ �˴ϴ�.
	ds.DepthEnable = FALSE;
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;		// ���̰� ���� ��.
	_pDevice->CreateDepthStencilState(&ds, &_pDSState[DS_DT_OFF_DW_OFF_ST_OFF]);


	


	//----------------------------------------------------------------------
	// ���ٽ� ���� ���� ��ü�� ����.��
	//----------------------------------------------------------------------
	// ���ٽ� ���� ��Ʈ ���� ����.
	// (Stencil.Ref & Stencil.Mask) Comparison-Func ( StencilBuffer.Value & Stencil.Mask)
	//
	// *StencilBufferValue : ���� �˻��� �ȼ��� ���ٽǰ�.
	// *ComFunc : �� �Լ�. ( > < >= <= ==  Always Never)
	//----------------------------------------------------------------------
	// DS ���°�ü #4 :  ���̹��� On, ���ٽǹ��� ON (�׻�, ������ ����) : "����/���ٽ� ���" ��
	ds.DepthEnable	  = TRUE;										//���̹��� ON! (�⺻��)
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ds.DepthFunc	  = D3D11_COMPARISON_LESS;
	ds.StencilEnable = TRUE;										//���ٽ� ���� ON! ��
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;		//���Լ� : "�׻� ���" (����)
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_REPLACE;		//������ : ������(Stencil Reference Value) �� ��ü.
	//ds.FrontFace.StencilFailOp	  = D3D11_STENCIL_OP_KEEP;		//���н� : ����.(�⺻��, ����)
	//ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		//���н� : ����.(�⺻��, ����)
	ds.BackFace = ds.FrontFace;										//�޸� ���� ����.
	_pDevice->CreateDepthStencilState(&ds, &_pDSState[DS_DEPTH_ON_STENCIL_ON]);
	

	// DS ���°�ü #5 : ���̹��� On, ���ٽǹ��� ON (���Ϻ�, ������ ����) : "���� ��ġ���� �׸���" ��
	//ds.DepthEnable	= TRUE;										//���̹��� ON! (�⺻��)(����)
	ds.StencilEnable = TRUE;										//���ٽ� ���� ON! 
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_EQUAL;		//���Լ� : "�����Ѱ�?" 
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;		//������ : ����.
	ds.BackFace = ds.FrontFace;										//�޸� ���� ����.
	_pDevice->CreateDepthStencilState(&ds, &_pDSState[DS_DEPTH_ON_STENCIL_EQUAL_KEEP]);


	// DS ���°�ü #6 : ���̹��� On, ���ٽǹ��� ON (�ٸ���, ������ ����) : "���� ��ġ �̿ܿ� �׸���" ��
	//ds.DepthEnable	= TRUE;										//���̹��� ON! (�⺻��)(����)
	ds.StencilEnable = TRUE;										//���ٽ� ���� ON!
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_NOT_EQUAL;	//���Լ� : "���� ������?" 
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_KEEP;		//������ : ����.
	ds.BackFace = ds.FrontFace;										//�޸� ���� ����.
	_pDevice->CreateDepthStencilState(&ds, &_pDSState[DS_DEPTH_ON_STENCIL_NOTEQUAL_KEEP]);


	/*// DS ���°�ü #7 : ���̹��� Off, ���ٽǹ��� ON (������ ����) : "���ٽǸ� ���" 
	ds.DepthEnable	  = FALSE;										//���̹��� OFF!
	ds.StencilEnable = TRUE;										//���ٽ� ���� ON! 
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;		//���Լ� : "�׻� ���" (����)
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_REPLACE;		//������ : ������(Stencil Reference Value) �� ��ü.
	ds.BackFace = ds.FrontFace;										//�޸� ���� ����.
	_pDevice->CreateDepthStencilState(&ds, &_pDSState[DS_DEPTH_OFF_STENCIL_ON]);
	*/
	
	/*// DS ���°�ü #8 : ���̹��� On, ���ٽǹ��� ON (���Ϻ�, ���� ����) : "���߱׸��� ������" 
	//ds.DepthEnable	= TRUE;										//���̹��� ON! (�⺻��)(����)
	//ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//ds.DepthFunc		= D3D11_COMPARISON_LESS;
	ds.StencilEnable = TRUE;										//���ٽ� ���� ON! 
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_EQUAL;		//���Լ� : "�����Ѱ�?" 
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_INCR;		//������ : ���� (+1) 
	ds.FrontFace.StencilFailOp		= D3D11_STENCIL_OP_KEEP;		//���н� : ����.
	ds.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		//���н� : ����.
	ds.BackFace = ds.FrontFace;										//�޸� ���� ����.
	_pDevice->CreateDepthStencilState(&ds, &_pDSState[DS_DEPTH_ON_STENCIL_EQUAL_INCR]);
	*/

	/*// DS ���°�ü #9 : ���̹��� On, ���ٽǹ��� ON (�׻�, ������ ����) : "���ٽǸ� ���" 
	ds.DepthEnable	  = TRUE;										//���̹��� ON! (�⺻��)
	ds.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;				//���̹��� ���� OFF.
	ds.DepthFunc	  = D3D11_COMPARISON_LESS;						//���̿��� ON. (�⺻��)
	ds.StencilEnable  = TRUE;										//���ٽ� ���� ON! 
	ds.FrontFace.StencilFunc		= D3D11_COMPARISON_ALWAYS;		//���Լ� : "�׻� ���" (����)
	ds.FrontFace.StencilPassOp		= D3D11_STENCIL_OP_REPLACE;		//������ : ������(Stencil Reference Value) �� ��ü.
	ds.BackFace = ds.FrontFace;										//�޸� ���� ����.
	_pDevice->CreateDepthStencilState(&ds, &_pDSState[DS_DEPTH_WRITE_OFF_STENCIL_ON]);
	*/
}



void FJRenderingEngine::DSStateUpdate()
{
	switch (_dsData)
	{
	case DS_TEST_ON | DS_WRITE_ON:
		_pDXDC->OMSetDepthStencilState(_pDSState[DS_DT_ON_DW_ON_ST_OFF], 0);
		break;

	case DS_TEST_OFF | DS_WRITE_ON:
		_pDXDC->OMSetDepthStencilState(_pDSState[DS_DT_OFF_DW_ON_ST_OFF], 0);
		break;

	case DS_TEST_ON | DS_WRITE_OFF:
		_pDXDC->OMSetDepthStencilState(_pDSState[DS_DT_ON_DW_OFF_ST_OFF], 0);
		break;

	case DS_TEST_OFF | DS_WRITE_OFF:
		_pDXDC->OMSetDepthStencilState(_pDSState[DS_DT_OFF_DW_OFF_ST_OFF], 0);
		break;
	}
}




///////////////////////////////////////////////////////////////////////////////
//
//  ����/���ٽ� ���� ���°�ü �����ϱ� : ���� ����� 1ȸ ȣ��.
//
void FJRenderingEngine::DSStateRelease()
{
	for (int i = 0; i < DS_MAX_; i++)
		SAFE_RELEASE(_pDSState[i]);
}




////////////////////////////////////////////////////////////////////////////// 
//
// ��ġ ������ ���� ��ü ����.
//
//----------------------------------------------------------------------------
// ���� ��ü State Objects (DX10/11)
// DX10 ���� ���� TnL �� RenderState �� ���ŵǾ����ϴ�.
// �̸� ����ϴ� ���� ���°�ü State Objects �������̽���, ������ ���¸� �ϳ��� �׷����� 
// ���� �������� ����̽��� �����մϴ�.  �̸� ���� ��ġ�� ��� ���� ��ȭ�� �ѹ��� �����Ͽ� 
// ���ʿ��� �������(Overhead) �� ���̰� ���� ȿ������ �������� ������ �մϴ�.
//
// ���°�ü�� ���� �ʱ�� ������ ����ϱ⸦ �����ϸ� ����� ����(Release) �ؾ� �մϴ�.
// ���°�ü�� �����Ұ���(Immutable, �б�����) ��ü �Դϴ�.
// DX9 ������ State-Block �� �̿� ������ ����� ����߾����ϴ�.
//
// ���� ��ü �������̽� �� '�����Ͷ����� �������� Rasterize Stage' ���� ������ 
// ID3D11RasterizerState �������̽��� ���� ó���մϴ�.  
// �����ϰ� ������ ����/��� ��� ������ �����սô�.  �ڼ��� ���� ������ �����Ͻʽÿ�. 
// ��ũ1 : ���� ��ü https://msdn.microsoft.com/en-us/library/windows/desktop/bb205071(v=vs.85).aspx
// ��ũ2 : ���̹��� ���� �����ϱ� https://msdn.microsoft.com/ko-kr/library/windows/desktop/bb205074(v=vs.85).aspx#Create_Depth_Stencil_State 
//----------------------------------------------------------------------------
//
void FJRenderingEngine::StateObjectCreate()
{

	//----------------------------
	// ������ ���� ��ü ���� : "�����Ͷ����� �������� Rasterize Stage" ���� ����.
	//----------------------------
	RasterStateLoad();

	//----------------------------
	// ����/���ٽ� ���� ��ü ����.: "��º��ձ� Output Merger" ���� ����. 
	//----------------------------
	DSStateLoad();



	//----------------------------
	// ���ĺ��� ���� ��ü ���� : "��º��ձ� Output Merger" ���� ����. 
	//----------------------------
	//...


}



///////////////////////////////////////////////////////////////////////////////
//
//  ���°�ü �����ϱ� : ���� ����� 1ȸ ȣ��.
//
void FJRenderingEngine::StateObjectRelease()
{
	RasterStateRelease();	//������ ���� ��ü ����.
	DSStateRelease();		//����/���ٽ� ���� ��ü ����.
}




////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
//
// ���� ���� ��ü ���� �Լ���.
//
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
////////////////////////////////////////////////////////////////////////////// 
// 
// ���� ���� ��ü ����.
//
void FJRenderingEngine::BlendStateCreate()
{
	//���� ���� ��ü ���� �ɼ� : ���� �⺻����.
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BLEND_DESC));
	
	// [�˸�] ���� ����Ÿ��(RenderTarget) ���� �� RT �� ������ �����ϳ�
	// ��а� RT #0 (�⺻)-�����-���� ����ϰڽ��ϴ�.��
	//
	bd.RenderTarget[0].BlendEnable = FALSE;							//���� ���� ����. �⺻���� FALSE(OFF)��	
	
	//���� ���� ȥ�� : Color Blending.(�⺻��)��
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//���� ȥ�� ����(Color - Operation), �⺻���� ���� : ������ = Src.Color + Dest.Color ��	
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;					//�ҽ�(����) ȥ�� ����, ���� 100% : Src.Color = Src * 1;��
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;				//Ÿ��(���) ȥ�� ����, ��� 0%   : Dest.Color = Dest * 0;  ��� 0�� RT �� "�����"�� �ǹ��մϴ�.��	

	//���� ���� ȥ�� : Alpha Blending.(�⺻��)
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;			//���� ȥ�� �Լ�(Alpha - Opertion), �⺻���� ����.
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;				//�ҽ�(����) ���� ȥ�� ����.
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;			//Ÿ��(���) ���� ȥ�� ����.
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;	//����Ÿ�ٿ� ���� �ɼ�.
	//���� �߰� �ɼ�.(�⺻��)	
	//bd.AlphaToCoverageEnable = FALSE;
	//bd.IndependentBlendEnable = FALSE;

	//[���°�ü 1] �⺻ ���� ���� ��ü.����.
	_pDevice->CreateBlendState(&bd, &_pBState[BS_DEFAULT]);




	//------------------------------
	//[���°�ü2] ���� ȥ�� ��ü ����.
	//------------------------------
	bd.RenderTarget[0].BlendEnable = TRUE;							//���� ȥ�� ON! ��	
	//bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;				//���� ȥ�� ���� (����, �⺻��) : ������ = Src.Color + Dest.Color ��	

	//[ȥ�� �׽�Ʈ1] 100% ȥ�ա�
	//bd.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;					//�ҽ�(����) ȥ�� ����, ���� 100% : Src.Color = Src * 1;
	//bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;					//Ÿ��(���) ȥ�� ����, ��� 100% : Dest.Color = Dest * 1;
		
	//[ȥ�� �׽�Ʈ2] ����� ȥ�ա�
	//bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;			//�ҽ�(����) ȥ�� ����, ���� ����� ���� : Src.Color = Src * Src;
	//bd.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_COLOR;		//Ÿ��(���) ȥ�� ����, ��� ����� ���� : Dest.Color = Dest * Dest;
	
	//[ȥ�� �׽�Ʈ3] ����� ȥ��2��
	//bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;			//�ҽ�(����) ȥ�� ����, ���� ����� ���� : Src.Color = Src * Src;
	//bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_DEST_COLOR;	//Ÿ��(���) ȥ�� ����, ��� ����� ���� : Dest.Color = Dest * (1-Dest);

	//[ȥ�� �׽�Ʈ4] ����� ȥ��3��
	//bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_COLOR;			//�ҽ�(����) ȥ�� ����, ���� ����� ���� : Src.Color = Src * Src;
	//bd.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;				//Ÿ��(���) ȥ�� ����, ��� 100% ����  : Dest.Color = Dest * 1;
		
	
	//#1 ��� ���� : ���� ���� : OP �� �����Ǵ� ���� �ƴϳ�, ������������ ����� ���������� Ȱ���� ����.��
	//bd.RenderTarget[0].SrcBlend = D3D11_BLEND_DEST_COLOR;				//�ҽ�(����) ȥ�� ����, ���� 100% : Src.Color = Src * Dest;
	//bd.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;					//Ÿ��(���) ȥ�� ����, ��� 0% : Dest.Color = Dest * 0;			 	
	

	//------------------------------
	// ���ı��� ���� ȥ�� : Alpha Blending ON! ��
	//------------------------------
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;			//�ҽ�(����) ȥ�� ����, ���� ���ĺ���  : Src.Color = Src * Src.a;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;		//Ÿ��(���) ȥ�� ����, ���� ���ĺ��� ���� : Dest.Color = Dest * (1-Src.a);			 	

	_pDevice->CreateBlendState(&bd, &_pBState[BS_ADD]);
}




////////////////////////////////////////////////////////////////////////////// 
//
// ���� ���� ��ü ���� : ���ø����̼� ����� ȣ��.
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
	_pDXDC->ClearDepthStencilView(_pDSView, D3D11_CLEAR_DEPTH, 1.0f, 0);	//����/���ٽ� �����.
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
void FJRenderingEngine::SetWireFrame(bool i_bSet)
{
	_rsData = i_bSet ? (0xfe & _rsData) | RM_WIRE : (0xfe & _rsData) | RM_SOLID;
}

bool FJRenderingEngine::GetWireFrame()
{
	return (_rsData & 0x01) == RM_WIRE ? true : false;
}

void FJRenderingEngine::SetSolidFrame(bool i_bSet)
{
	SetWireFrame(!i_bSet);
}

bool FJRenderingEngine::GetSolidFrame()
{
	return !GetWireFrame();
}

void FJRenderingEngine::SetCullMode(CULLMODE mode)
{
	_rsData = (0xF9 & _rsData) | mode;
}

CULLMODE FJRenderingEngine::GetCullMode()
{
	return (CULLMODE)(_rsData & 0x06);
}

void FJRenderingEngine::SetRasterMode(byte i_rm)
{
	_rsData = i_rm;
}

void FJRenderingEngine::SetDepthTest(bool i_bSet)
{
	_dsData = i_bSet ? (0xfe & _rsData) | DS_TEST_ON : (0xfe & _rsData) | DS_TEST_OFF;
}

bool FJRenderingEngine::GetDepthTest()
{
	return (_rsData & 0x01) == DS_TEST_ON ? true : false;
}

void FJRenderingEngine::SetDepthWrite(bool i_bSet)
{
	_dsData = i_bSet ? (0xfd & _rsData) | DS_WRITE_ON : (0xfd & _rsData) | DS_WRITE_OFF;
}

bool FJRenderingEngine::GetDepthWrite()
{
	return (_rsData & 0x02) == DS_WRITE_ON ? true : false;
}

void FJRenderingEngine::SetClearColor(COLOR& i_col)
{
	_clearCol = i_col;
}


COLOR FJRenderingEngine::GetClearColor()
{
	return _clearCol;
}