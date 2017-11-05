#include "Object.h"

CObject::CObject()
{
	ZeroMemory(&m_vPos, sizeof(VECTOR3));
	ZeroMemory(&m_vRot, sizeof(VECTOR3));
	ZeroMemory(&m_vScale, sizeof(VECTOR3));
}

CObject::CObject(VECTOR3& pos, VECTOR3& rot, VECTOR3& scale)
	: m_vPos(pos), m_vRot(rot), m_vScale(scale)
{
	
}

CObject::~CObject()
{

}


void CObject::Update()
{

}


void CObject::Render()
{

}