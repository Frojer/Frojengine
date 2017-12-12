#pragma once

#include "Frojengine.h"
#include "System.h"

class Hero : public Behaviour
{
public:
	int state;
	System* pSystem;

public:
	virtual void Initialize() override;
	virtual void Update() override;
};