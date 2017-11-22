#pragma once

#include "Frojengine.h"

class Temp : public Behaviour
{
public:
	Temp();
	~Temp();

	virtual void Initialize() override;
	virtual void Update() override;
};