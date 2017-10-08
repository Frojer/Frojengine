#include "FJFontEngine.h"

FJFontEngine::FJFontEngine(LPDEVICE i_pDevice, LPDXDC i_pDXDC)
{
	_pDevice = i_pDevice;
	_pDXDC = i_pDXDC;

	//ASCII 0 ~ 255 + Ư������'��' + Unicode �ѱ� �ϼ��� �� 11,440 ����, ũ��:9
	FontCreate(L"Font/����9k.sfont");
}


FJFontEngine::~FJFontEngine()
{
	FontRelease();
}



bool FJFontEngine::FontCreate(LPCWSTR i_fileName)
{
	// Sprite Batch ��ü ����.
	_pFontBatch = new SpriteBatch(_pDXDC);

	// DirectX Toolkit : Sprite Font ��ü ����.
	
	try
	{
		_pFont = new SpriteFont(_pDevice, i_fileName);
		_pFont->SetLineSpacing(14.0f);				// ��Ʈ9 ����, �ٰ��� ����. '���߶��� ��½� ����� ������'
		_pFont->SetDefaultCharacter('_');			// ��� ���ڰ� �̰˻��� ��� ����� Ű��.
	}

	catch (std::exception& e)
	{
		TCHAR msg[1024] = L"";
		::mbstowcs(msg, e.what(), strlen(e.what()));
		FJError(0, L"��Ʈ ���� ���� : %s \n File=%s", msg, i_fileName);
		return false;
	}

	return true;
}



void FJFontEngine::FontRelease()
{
	SAFE_DELETE(_pFontBatch);
	SAFE_DELETE(_pFont);
}



///////////////////////////////////////////////////////////////////////////////
//
//  ���ڿ� ��� : DX11 (�����ڵ�) ����.
//
///////////////////////////////////////////////////////////////////////////////
void FJFontEngine::TextDraw(int x, int y, COLOR col, TCHAR* msg, ...)
{
	const DWORD _size = 2048;

	TCHAR buff[_size] = L"";
	va_list vl;
	va_start(vl, msg);
	_vstprintf(buff, msg, vl);
	va_end(vl);

	_pFontBatch->Begin();
	//_pFontBatch->Begin(SpriteSortMode_Deferred, nullptr, nullptr, DepthStencilState);							// ���̿��� �߰�.
	_pFont->DrawString(_pFontBatch, buff, Vector2((float)x, (float)y), Vector4(col));							// ���� ũ��.
	//_pFont->DrawString(_pFontBatch, buff, Vector2((float)x, (float)y), Vector4(col), 0, Vector4::Zero, 0.8f);	// 80% ��� ���.
	_pFontBatch->End();
}