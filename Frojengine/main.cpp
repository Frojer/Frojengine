#include "FJSystemEngine.h"

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

	while (!pSystem->m_bEnd)
	{
		pSystem->MessagePump();
	}

	delete pSystem;

	return 0;
}