#pragma once

#include "Frojengine.h"

class Hero : public Behaviour
{
public:
	Hero();
	~Hero();

	virtual void Initialize() override;
	virtual void Update() override;
};