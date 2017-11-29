#pragma once

#include "Frojengine.h"

class Camera : public Behaviour, private Device
{
private:
	D3D11_VIEWPORT _vp;

	float m_FOV;
	float m_Aspect;

public:
	float m_Znear;
	float m_Zfar;

	UINT m_Width;
	UINT m_Height;

public:
	Camera();
	virtual ~Camera();

	void Create(float fovAngle, float zNear, float zFar, UINT width, UINT height);

	void SetViewport(D3D11_VIEWPORT& viewport);

	void UseCamera();

	void LookAt(VECTOR3 target);

	MATRIXA GetViewMatrixLH();
	MATRIXA GetPerspectiveFovLH();


	void SetFOV(float fovAngle) { m_FOV = XMConvertToRadians(fovAngle); }
	float GetFOV() { return m_FOV; }

	float GetAspect();
};