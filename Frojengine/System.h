#pragma once

#include "Frojengine.h"

class System : public Behaviour
{
public:
	virtual void Initialize() override;
	virtual void Update() override;
};