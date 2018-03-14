#include "AnimScene.h"
#include "AWSystem.h"
#include "CameraControl.h"

bool AnimScene::Load()
{
	CObject* pCam = new CObject;
	pCam->m_name = L"Camera";
	Camera* c = pCam->AddComponent<Camera>();
	CameraControl* cc = pCam->AddComponent<CameraControl>();
	cc->cam = c;

	CObject* system = new CObject;
	system->AddComponent<AWSystem>();

	FJRenderingEngine::SetClearColor(COLOR(0, 0.125f, 0.3f, 1));

	return true;
}