#pragma once

#include "Frojengine.h"

class Hero : public Behaviour
{
public:
	int state;
public:
	virtual void Initialize() override;
	virtual void Update() override;
};