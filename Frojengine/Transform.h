#pragma once

#include "Frojengine.h"

enum Space { World, Local };

class Transform final : public Component
{
private:
	VECTOR3 m_vPos;
	VECTOR3 m_vRot;
public:
	VECTOR3 m_vScale;

public:
	MATRIXA GetWorldMatrix();
	MATRIXA GetPositionMatrix();
	MATRIXA GetRotationMatrix();
	MATRIXA GetRotPosMatrix();
	MATRIXA GetScaleMatrix();

public:
	Transform();
	virtual ~Transform();

	void Translate(const VECTOR3& translation, Space space = Space::Local);
	void Rotate(const VECTOR3& eulerAngles, Space space = Space::Local);

	// Get(), Set()
	void SetPositionWorld(const VECTOR3& pos);
	void SetPositionLocal(const VECTOR3& pos);
	void SetRotationDegree(const VECTOR3& degree);
	void SetRotationRadian(const VECTOR3& radian);
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