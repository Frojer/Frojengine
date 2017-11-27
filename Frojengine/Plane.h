#pragma once

#include "Frojengine.h"

class Plane : public Behaviour
{
	CObject* proParent;
	CObject* plane;
	CObject* propeller;
public:
	virtual void Initialize() override;
	virtual void Update() override;
};