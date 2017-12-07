#pragma once

#include "Frojengine.h"

class TripleWindmill : public Behaviour
{
public:
	CObject* body;
	CObject* wing[3];
public:
	virtual void Initialize() override;
	virtual void Update() override;
};