#include "Camera.h"

CCamera::CCamera()
{

}

CCamera::CCamera(const CCamera& obj)
{

}

CCamera::~CCamera()
{

}

bool CCamera::Create(VECTOR3 pos, VECTOR3 up, VECTOR3 lookAt, float fovAngle, float zNear, float zFar, UINT width, UINT height, D3D11_VIEWPORT& viewport)
{
	m_Pos = pos;
	m_Up = up;
	m_LookAt = lookAt;

	m_FOV = XMConvertToRadians(fovAngle);
	m_Znear = zNear;
	m_Zfar = zFar;

	m_Width = width;
	m_Height = height;

	m_Aspect = (FLOAT)m_Width / (FLOAT)m_Height;

	_Viewport = viewport;

	SceneManager::pCurrentScene->AddCamera(this);

	m_Enable = true;

	return true;
}



void CCamera::SetViewport(D3D11_VIEWPORT& viewport)
{
	_Viewport = viewport;
}



void CCamera::UseCamera()
{
	_pDXDC->RSSetViewports(1, &_Viewport);
	CScene::mView = GetViewMatrixLH();
	CScene::mProj = GetPerspectiveFovLH();
}



void CCamera::CameraUpdate()
{

}



MATRIXA CCamera::GetViewMatrixLH()
{
	VECTOR pos = XMLoadFloat3(&m_Pos);
	VECTOR lookAt = XMLoadFloat3(&m_LookAt);
	VECTOR up = XMLoadFloat3(&m_Up);

	//MATRIX mView;
	//XMStoreFloat4x4(&mView, XMMatrixLookAtLH(pos, lookAt, up));

	//return mView;

	return XMMatrixLookAtLH(pos, lookAt, up);
}


MATRIXA CCamera::GetPerspectiveFovLH()
{
	//MATRIX mProj;
	//XMStoreFloat4x4(&mProj, XMMatrixPerspectiveFovLH(m_FOV, m_Aspect, m_Znear, m_Zfar));

	//return mProj;

	return XMMatrixPerspectiveFovLH(m_FOV, m_Aspect, m_Znear, m_Zfar);
}


float CCamera::GetAspect()
{
	return m_Aspect;
}