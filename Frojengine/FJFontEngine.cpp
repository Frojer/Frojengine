#include "FJFontEngine.h"

FJFontEngine::FJFontEngine(LPDEVICE i_pDevice, LPDXDC i_pDXDC)
{
	_pDevice = i_pDevice;
	_pDXDC = i_pDXDC;

	//ASCII 0 ~ 255 + 특수문자'■' + Unicode 한글 완성형 총 11,440 글자, 크기:9
	FontCreate(L"Font/굴림9k.sfont");
}


FJFontEngine::~FJFontEngine()
{
	FontRelease();
}



bool FJFontEngine::FontCreate(LPCWSTR i_fileName)
{
	// Sprite Batch 개체 생성.
	_pFontBatch = new SpriteBatch(_pDXDC);

	// DirectX Toolkit : Sprite Font 객체 생성.
	
	try
	{
		_pFont = new SpriteFont(_pDevice, i_fileName);
		_pFont->SetLineSpacing(14.0f);				// 폰트9 기준, 줄간격 설정. '다중라인 출력시 흐려짐 방지용'
		_pFont->SetDefaultCharacter('_');			// 출력 글자값 미검색시 대신 출력할 키값.
	}

	catch (std::exception& e)
	{
		TCHAR msg[1024] = L"";
		::mbstowcs(msg, e.what(), strlen(e.what()));
		FJError(0, L"폰트 생성 실패 : %s \n File=%s", msg, i_fileName);
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
//  문자열 출력 : DX11 (유니코드) 대응.
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
	//_pFontBatch->Begin(SpriteSortMode_Deferred, nullptr, nullptr, DepthStencilState);							// 깊이연산 추가.
	_pFont->DrawString(_pFontBatch, buff, Vector2((float)x, (float)y), Vector4(col));							// 원본 크기.
	//_pFont->DrawString(_pFontBatch, buff, Vector2((float)x, (float)y), Vector4(col), 0, Vector4::Zero, 0.8f);	// 80% 축소 출력.
	_pFontBatch->End();
}