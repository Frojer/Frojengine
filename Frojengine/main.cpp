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


	//---------------
	// 데이터 로딩
	//---------------
	pSystem->LoadData();


	//---------------
	// Game Loop
	//---------------
	while (!pSystem->m_bEnd)
	{
		pSystem->MessagePump();

		pSystem->Update();
		pSystem->Rendering();
	}

	SAFE_DELETE(pSystem);

	return 0;
}