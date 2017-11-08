#include "Hero.h"

Hero::Hero(CModel* model, VECTOR3& pos, VECTOR3& rot, VECTOR3& scale)
	: CObject(model, pos, rot, scale)
{

}


Hero::~Hero()
{

}


void Hero::Initialize()
{

}


void Hero::Update()
{
	if (IsKeyUp(VK_SPACE))
		FJRenderingEngine::SetWireFrame(!FJRenderingEngine::GetWireFrame());
}