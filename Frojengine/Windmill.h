#pragma once

#include "Frojengine.h"

class Windmill : public Behaviour
{
public:
	CObject* body;
	CObject* wing;

public:
	virtual void Initialize() override;
	virtual void Update() override;
};