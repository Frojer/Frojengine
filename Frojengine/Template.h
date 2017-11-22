#pragma once

#include "Behaviour.h"

class Template : public Behaviour
{
public:
	virtual void Initialize() override;
	virtual void Update() override;
};