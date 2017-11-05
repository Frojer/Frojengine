#pragma once

#include "Frojengine.h"

class CObject
{
private:

protected:

public:
	VECTOR3 m_vPos;
	VECTOR3 m_vRot;
	VECTOR3 m_vScale;

private:

public:
	CObject();
	CObject(VECTOR3& pos, VECTOR3& rot, VECTOR3& scale);
	~CObject();

	virtual void Initialize() = 0;

	void Update();
	void Render();
};