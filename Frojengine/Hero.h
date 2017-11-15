#pragma once

#include "Frojengine.h"

class Hero : public CObject
{
public:
	CCamera* cam;

public:
	Hero(VECTOR3& pos, VECTOR3& rot, VECTOR3& scale);
	~Hero();


	void Initialize();

	void Update();
};