#include "Temp.h"

Temp::Temp()
{

}


Temp::~Temp()
{

}


void Temp::Initialize()
{

}


void Temp::Update()
{
	if (IsKeyUp('Y')) _pObj->m_pTransform->m_vPos.z += 10.0f;
	if (IsKeyUp('Y')) _pObj->m_pTransform->m_vPos.z += 10.0f;
}