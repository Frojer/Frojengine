#pragma once

#include "Frojengine.h"
#include "System.h"

class TripleWindmill2 : public Behaviour
{
public:
	System* system;

	CObject* body;
	CObject* wing[3];
public:
	virtual void Initialize() override;
	virtual void Update() override;
};