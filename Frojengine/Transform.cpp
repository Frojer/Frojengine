#include "Transform.h"

Transform::Transform()
{
	m_name = L"Transform";
	_type = COMPONENT_TYPE_AFTERUPDATE;

	ZeroMemory(&m_vPos, sizeof(VECTOR3));
	ZeroMemory(&m_vRot, sizeof(VECTOR3));
	m_vScale.x = 1.0f;
	m_vScale.y = 1.0f;
	m_vScale.z = 1.0f;
}

Transform::~Transform()
{

}


MATRIXA Transform::GetWorldMatrix()
{
	MATRIXA mPos, mRot, mScale;
	MATRIXA mWorld;

	mPos = DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&m_vPos));
	mRot = DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_vRot));
	mScale = DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&m_vScale));

	mWorld = mScale * mRot * mPos;

	if (_pObj->GetParent() != nullptr)
		mWorld *= _pObj->GetParent()->m_pTransform->GetWorldMatrix();

	return mWorld;
}


void Transform::AfterUpdate()
{

}

VECTOR3 operator+ (const VECTOR3& lhs, const VECTOR3& rhs)
{
	VECTOR3 vec;
	vec.x = lhs.x + rhs.x;
	vec.y = lhs.y + rhs.y;
	vec.z = lhs.z + rhs.z;
	return vec;
}

VECTOR3 Transform::GetWorldPositioni()
{
	CObject* pObj = _pObj->GetParent();

	VECTOR3 pos = m_vPos;

	while (pObj != nullptr)
	{
		pos = pos + pObj->m_pTransform->m_vPos;
		pObj = pObj->GetParent();
	}

	return pos;
}