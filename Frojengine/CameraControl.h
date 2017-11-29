#pragma once

#include "Frojengine.h"

class CameraControl : public Behaviour
{
public:
	Transform* _pHeroTr;
	Camera* cam;

public:
	virtual void Initialize() override;
	virtual void Update() override;
};