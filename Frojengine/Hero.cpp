#include "Hero.h"

Hero::Hero(VECTOR3& pos, VECTOR3& rot, VECTOR3& scale)
	: CObject(pos, rot, scale)
{

}


Hero::~Hero()
{

}


void Hero::Initialize()
{
	cam = SceneManager::pCurrentScene->GetCamera(0);
}


void Hero::Update()
{
	if (IsKeyUp(VK_SPACE))
		FJRenderingEngine::SetWireFrame(!FJRenderingEngine::GetWireFrame());

	cam->m_Pos.x -= 10 * FJSystemEngine::GetInstance()->m_fDeltaTime;
	if (IsKeyUp('A'))
	{
		cam->m_Pos.x -= 0.5f * FJSystemEngine::GetInstance()->m_fDeltaTime;
	}
	if (IsKeyUp('D'))
		FJRenderingEngine::SetWireFrame(!FJRenderingEngine::GetWireFrame());
	if (IsKeyUp('W'))
		FJRenderingEngine::SetWireFrame(!FJRenderingEngine::GetWireFrame());
	if (IsKeyUp('S'))
		FJRenderingEngine::SetWireFrame(!FJRenderingEngine::GetWireFrame());
}