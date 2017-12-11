#pragma once

#include "Frojengine.h"
#include "System.h"

class Windmill : public Behaviour
{
public:
	System* system;

	CObject* body;
	CObject* wing;

public:
	virtual void Initialize() override;
	virtual void Update() override;
};