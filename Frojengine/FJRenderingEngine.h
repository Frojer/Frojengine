#pragma once

#include "Frojengine.h"

class FJFontEngine;

enum
{
	RM_SOLID = 0x00,
	RM_WIRE = 0x01,
};

enum
{
	DM_TEST_ON = 0x00,
	DM_TEST_OFF = 0x01,
	DM_WRITE_ON = 0x00,
	DM_WRITE_OFF = 0x02,
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

		RS_MAX_
	};
	//���� ��ü �迭 : "��ɺ�" �׷����� �����մϴ�.
	ID3D11RasterizerState*	_pRState[RS_MAX_];
	//               Wire
	// Bit : 0000000 0
	static byte _rsData;



	// ����/���ٽ� ���� ���°�ü.
	enum {
		DS_DEFAULT,			//���̹��� ON! (�⺻��)
		DS_DEPTH_TEST_OFF,	//���̹��� OFF!
		DS_DEPTH_WRITE_OFF,	//���̹��� ���� ����.
		DS_DT_OFF_DW_OFF,	//�����׽�Ʈ ����.

		DS_MAX_,
	};
	ID3D11DepthStencilState* _pDSState[DS_MAX_];
	//       DepthTest DepthWrite
	// Bit : 0         0          000000
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