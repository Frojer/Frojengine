#pragma once

#include "Behaviour.h"

class Windmill : public Behaviour
{
public:
	virtual void Initialize() override;
	virtual void Update() override;
};