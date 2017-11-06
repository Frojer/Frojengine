#include "Frojengine.h"

///////////////////////////
//
//  WinMain : 메인 함수.
//
///////////////////////////
int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	FJSystemEngine* pSystem = FJSystemEngine::GetInstance();

	if (!pSystem->CreateEngine())
	{
		SAFE_DELETE(pSystem);

		return 0;
	}

	//---------------
	// 데이터 로딩
	//---------------
	pSystem->LoadData();

	//-------------
	// 엔진 루프
	//-------------
	pSystem->Run();

	SAFE_DELETE(pSystem);

	return 0;
}