#include "AnimScene.h"
#include "AWSystem.h"
#include "CameraControl.h"

bool AnimScene::Load()
{
	CObject* pCam = new CObject(VECTOR3(0.0f, 10.0f, -5.0f), VECTOR3(0.0f, 0.0f, 0.0f), VECTOR3(1.0f, 1.0f, 1.0f));
	pCam->m_name = L"Camera";
	Camera* c = pCam->AddComponent<Camera>();
	CameraControl* cc = pCam->AddComponent<CameraControl>();
	cc->cam = c;

	CObject* system = new CObject;
	system->AddComponent<AWSystem>();

	FJRenderingEngine::SetClearColor(COLOR(0, 0.125f, 0.3f, 1));

	return true;
}