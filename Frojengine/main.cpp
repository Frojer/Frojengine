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

	//-------------
	// ���� ����
	//-------------
	pSystem->Run();

	SAFE_DELETE(pSystem);

	return 0;
}