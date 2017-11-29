#include "Transform.h"
#include "FJMath.h"

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

	if (_pObj->GetParent() != nullptr)
		mScale *= _pObj->GetParent()->m_pTransform->GetScaleMatrix();

	mWorld = mScale * mRot * mPos;

	if (_pObj->GetParent() != nullptr)
		mWorld *= _pObj->GetParent()->m_pTransform->GetRotPosMatrix();

	return mWorld;
}


MATRIXA Transform::GetPositionMatrix()
{
	MATRIXA mPos;
	mPos = DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&m_vPos));

	if (_pObj->GetParent() != nullptr)
		mPos *= _pObj->GetParent()->m_pTransform->GetPositionMatrix();

	return mPos;
}


MATRIXA Transform::GetRotationMatrix()
{
	MATRIXA mRot;
	mRot = DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&m_vRot));

	if (_pObj->GetParent() != nullptr)
		mRot *= _pObj->GetParent()->m_pTransform->GetRotationMatrix();

	return mRot;
}


MATRIXA Transform::GetRotPosMatrix()
{
	MATRIXA mPos, mRot, mRP;
	mPos = DirectX::XMMatrixTranslationFromVector(XMLoadFloat3(&m_vPos));
	mRot = DirectX::XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&m_vRot));

	mRP = mRot * mPos;

	if (_pObj->GetParent() != nullptr)
		mRP *= _pObj->GetParent()->m_pTransform->GetRotPosMatrix();

	return mRP;
}


MATRIXA Transform::GetScaleMatrix()
{
	MATRIXA mScale;
	mScale = DirectX::XMMatrixScalingFromVector(XMLoadFloat3(&m_vScale));

	if (_pObj->GetParent() != nullptr)
		mScale *= _pObj->GetParent()->m_pTransform->GetScaleMatrix();

	return mScale;
}


void Transform::AfterUpdate()
{

}


void Transform::SetPositionWorld(VECTOR3& pos)
{
	m_vPos = VECTOR3(0.0f, 0.0f, 0.0f);
	m_vPos = pos - GetPositionWorld();
}


void Transform::SetPositionLocal(VECTOR3& pos)
{
	m_vPos = pos;
}


void Transform::SetRotationDegree(VECTOR3& angle)
{
	m_vRot = angle / 180.0f * XM_PI;
}


void Transform::SetRotationRadian(VECTOR3& radian)
{
	m_vRot = radian;
}


VECTOR3 Transform::GetPositionWorld()
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


VECTOR3 Transform::GetPositionLocal()
{
	return m_vPos;
}


VECTOR3 Transform::GetRotationDegree()
{
	return m_vRot;
}


VECTOR3 Transform::GetRotationRadian()
{
	return m_vRot / (180.0f * XM_PI);
}


VECTOR3 Transform::GetLookAt()
{
	VECTOR rot = XMLoadFloat3(&m_vRot);
	VECTOR3 lookAt;

	XMStoreFloat3(&lookAt, XMVector3Transform(XMLoadFloat3(&VECTOR3(0.0f, 0.0f, 1.0f)), XMMatrixRotationRollPitchYawFromVector(rot)));

	return lookAt;
}


VECTOR3 Transform::GetUpVector()
{
	VECTOR rot = XMLoadFloat3(&_pObj->m_pTransform->m_vRot);
	VECTOR3 up;

	XMStoreFloat3(&up, XMVector3Transform(XMLoadFloat3(&VECTOR3(0.0f, 1.0f, 0.0f)), XMMatrixRotationRollPitchYawFromVector(rot)));

	return up;
}


VECTOR3 Transform::GetRightVector()
{
	VECTOR rot = XMLoadFloat3(&_pObj->m_pTransform->m_vRot);
	VECTOR3 right;

	XMStoreFloat3(&right, XMVector3Transform(XMLoadFloat3(&VECTOR3(1.0f, 0.0f, 0.0f)), XMMatrixRotationRollPitchYawFromVector(rot)));

	return right;
}