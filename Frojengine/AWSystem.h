#pragma once

#include "Frojengine.h"

class AWSystem : public Behaviour
{
public:
	Transform* pBoxTr;

public:
	AWSystem();
	virtual ~AWSystem();

	virtual void Initialize() override;
	virtual void Update() override;
};