#include "Frojengine.h"

///////////////////////////
//
//  WinMain : ���� �Լ�.
//
///////////////////////////
int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	FJSystemEngine* pSystem = FJSystemEngine::GetInstance();


	//---------------
	// ������ �ε�
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