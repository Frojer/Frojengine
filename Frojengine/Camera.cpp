#include "Camera.h"
#include "FJMath.h"

Camera::Camera()
{
	m_name = L"Camera";

	_vp.TopLeftX = 0;
	_vp.TopLeftY = 0;
	_vp.Width = g_setting.displayMode.Width;
	_vp.Height = g_setting.displayMode.Height;
	_vp.MinDepth = 0.0f;
	_vp.MaxDepth = 1.0f;

	Create(45.0f, 1.0f, 1000.0f, g_setting.displayMode.Width, g_setting.displayMode.Height);
}


Camera::~Camera()
{

}


void Camera::Create(float fovAngle, float zNear, float zFar, UINT width, UINT height)
{
	m_FOV = XMConvertToRadians(fovAngle);
	m_Znear = zNear;
	m_Zfar = zFar;

	m_Width = width;
	m_Height = height;

	m_Aspect = (FLOAT)m_Width / (FLOAT)m_Height;

	SceneManager::pCurrentScene->AddCamera(this);
}



void Camera::SetViewport(D3D11_VIEWPORT& viewport)
{
	_vp = viewport;
}



void Camera::UseCamera()
{
	_pDXDC->RSSetViewports(1, &_vp);
	CScene::mView = GetViewMatrixLH();
	CScene::mProj = GetPerspectiveFovLH();
}


void Camera::LookAt(VECTOR3 target)
{

}



MATRIXA Camera::GetViewMatrixLH()
{
	CObject* pObj = GetMyObject();

	VECTOR pos = XMLoadFloat3(&pObj->m_pTransform->GetPositionWorld());
	VECTOR lookAt = XMLoadFloat3(&(pObj->m_pTransform->GetLookAt() + pObj->m_pTransform->GetPositionWorld()));
	VECTOR up = XMLoadFloat3(&pObj->m_pTransform->GetUpVector());

	//MATRIX mView;
	//XMStoreFloat4x4(&mView, XMMatrixLookAtLH(pos, lookAt, up));

	//return mView;

	return XMMatrixLookAtLH(pos, lookAt, up);
}


MATRIXA Camera::GetPerspectiveFovLH()
{
	//MATRIX mProj;
	//XMStoreFloat4x4(&mProj, XMMatrixPerspectiveFovLH(m_FOV, m_Aspect, m_Znear, m_Zfar));

	//return mProj;

	return XMMatrixPerspectiveFovLH(m_FOV, m_Aspect, m_Znear, m_Zfar);
}


float Camera::GetAspect()
{
	return m_Aspect;
}