#pragma once

#include "Frojengine.h"
#include "System.h"

class Plane : public Behaviour
{
	CObject* proParent;
	CObject* plane;
	CObject* propeller;

public:
	System* pSystem;

public:
	virtual void Initialize() override;
	virtual void Update() override;
};