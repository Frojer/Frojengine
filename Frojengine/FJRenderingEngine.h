#pragma once

#include "Frojengine.h"

class FJFontEngine;

enum CULLMODE
{
	CULL_NONE = 0x00,
	CULL_BACK = 0x02,
	CULL_FRONT = 0x04
};

enum
{
	RM_SOLID = 0x00,		// 00000000
	RM_WIRE = 0x01,			// 00000001
	RM_CULLNONE = 0x00,		// 00000000
	RM_CULLBACK = 0x02,		// 00000010
	RM_CULLFRONT = 0x04,	// 00000100
};

enum
{
	DS_TEST_ON = 0x00,
	DS_TEST_OFF = 0x01,
	DS_WRITE_ON = 0x00,
	DS_WRITE_OFF = 0x02,
};

class FJRenderingEngine
{
private:
	//-------------------
	// ������ ���� ������
	//-------------------

	// ��ġ ���� �� ������ ��� ���� �������̽�.
	LPDEVICE	_pDevice;
	LPDXDC		_pDXDC;
	LPSWAPCHAIN	_pSwapChain;
	LPRTVIEW	_pRTView;

	//���� ���ٽ� ���� ����.
	ID3D11Texture2D*		 _pDS;			//����-���ٽ� ����.
	ID3D11DepthStencilView*  _pDSView;		//����-���ٽ� ��.

	// ���������� ����۸� ������
	static COLOR _clearCol;

	enum {
		RS_SOLID,				// �⺻ ������ : �ָ��� Soild
		RS_WIRE,				// ���̾������� ������.
		RS_SOLID_CULL_BACK,
		RS_WIRE_CULL_BACK,
		RS_SOLID_CULL_FRONT,
		RS_WIRE_CULL_FRONT,

		RS_MAX_
	};
	//���� ��ü �迭 : "��ɺ�" �׷����� �����մϴ�.
	ID3D11RasterizerState*	_pRState[RS_MAX_];
	//             CullMode   Wire
	// Bit : 00000 00         0
	static byte _rsData;



	// ����/���ٽ� ���� ���°�ü.
	enum {
		DS_DT_ON_DW_ON_ST_OFF,			// ���̹��� ON! (�⺻��)
		DS_DT_OFF_DW_ON_ST_OFF,			// ���̹��� OFF!
		DS_DT_ON_DW_OFF_ST_OFF,			// ���̹��� ���� ����.
		DS_DT_OFF_DW_OFF_ST_OFF,		// �����׽�Ʈ ����.
		DS_DT_ON_DW_ON_ST_ON,			// ���̹��� ON! (�⺻��)
		DS_DT_OFF_DW_ON_ST_OFF,			// ���̹��� OFF!
		DS_DT_ON_DW_OFF_ST_OFF,			// ���̹��� ���� ����.
		DS_DT_OFF_DW_OFF_ST_OFF,			// �����׽�Ʈ ����.

		DS_MAX_,
	};
	ID3D11DepthStencilState* _pDSState[DS_MAX_];
	//                    DepthComp  DepthTest  DepthWrite
	// Bit : 00000000000000          0          0
	static byte _dsData;



	// ����/���� ȥ�� ���� ��ü Blending State Objects
	enum {
		BS_DEFAULT,		//�⺻ȥ�� ��ü.
		BS_ADD,			//����.

		BS_MAX_
	};
	// ����/���� ȥ�� ���� ��ü
	ID3D11BlendState* _pBState[BS_MAX_] = { nullptr, };
	void BlendStateCreate();
	void BlendStateRelease();

private:
	bool CreateRenderingEngine(HWND i_hWnd);

	bool DXSetup(HWND i_hWnd);
	void DXRelease();

	bool CreateDeviceSwapChain(HWND i_hWnd);
	bool CreateRenderTarget();
	bool CreateDepthStencil();

	void SetViewPort();
	void GetDeviceInfo();
	void GetFeatureLevel();

	// ��ġ ���� ��ü ����/����
	void StateObjectCreate();
	void StateObjectRelease();

	// ������ ���°�ü
	void RasterStateLoad();
	void RasterStateUpdate();
	void RasterStateRelease();

	// ����/���ٽ� ���� ���°�ü
	void DSStateLoad();
	void DSStateUpdate();
	void DSStateRelease();

	void ClearBackBuffer();
	void Flip();

	static void FJErrorW(TCHAR* file, UINT line, TCHAR* func, BOOL bMBox, HRESULT hr, TCHAR* msg, ...);

public:
	FJRenderingEngine();
	~FJRenderingEngine();

	static void		SetWireFrame(bool i_bSet);
	static bool		GetWireFrame();
	static void		SetSolidFrame(bool i_bSet);
	static bool		GetSolidFrame();
	static void		SetCullMode(CULLMODE mode);
	static CULLMODE	GetCullMode();
	static void		SetRasterMode(byte i_rm);
	static void		SetDepthTest(bool i_bSet);
	static bool		GetDepthTest();
	static void		SetDepthWrite(bool i_bSet);
	static bool		GetDepthWrite();
	static void		SetClearColor(COLOR& i_col);
	static COLOR	GetClearColor();

	friend class FJSystemEngine;
	friend class CShader;
};

#define FJError(hr, msg, ...)  \
FJRenderingEngine::FJErrorW( __FILEW__, __LINE__, __FUNCTIONW__, TRUE, hr, msg, __VA_ARGS__ )