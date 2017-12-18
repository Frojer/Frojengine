#pragma once

#include "Frojengine.h"
#include "System.h"

class TripleWindmill : public Behaviour
{
public:
	System* system;

	CObject* body;
	CObject* wing[3];

	bool isMirror;
public:
	virtual void Initialize() override;
	virtual void Update() override;
};