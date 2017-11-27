#pragma once

#include "Frojengine.h"

class Transform : public Component
{
public:
	VECTOR3 m_vPos;
	VECTOR3 m_vRot;
	VECTOR3 m_vScale;

private:
	MATRIXA GetWorldMatrix();

	virtual void AfterUpdate() override;

public:
	Transform();
	virtual ~Transform();

	VECTOR3 GetWorldPositioni();

	// Renderer::BufferUpdate()
	friend class Renderer;
};