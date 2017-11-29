#pragma once

#include "Frojengine.h"

class Transform final : public Component
{
public:
	VECTOR3 m_vPos;
	VECTOR3 m_vRot;
	VECTOR3 m_vScale;

private:
	MATRIXA GetWorldMatrix();
	MATRIXA GetPositionMatrix();
	MATRIXA GetRotationMatrix();
	MATRIXA GetRotPosMatrix();
	MATRIXA GetScaleMatrix();

	virtual void AfterUpdate() override;

public:
	Transform();
	virtual ~Transform();

	void SetPositionWorld(VECTOR3& pos);
	void SetPositionLocal(VECTOR3& pos);
	void SetRotationDegree(VECTOR3& degree);
	void SetRotationRadian(VECTOR3& radian);
	VECTOR3 GetPositionWorld();
	VECTOR3 GetPositionLocal();
	VECTOR3 GetRotationDegree();
	VECTOR3 GetRotationRadian();

	VECTOR3 GetLookAt();
	VECTOR3 GetUpVector();
	VECTOR3 GetRightVector();

	// Renderer::BufferUpdate()
	friend class Renderer;
};