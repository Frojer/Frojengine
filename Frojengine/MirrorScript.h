#pragma once

#include "Frojengine.h"

class MirrorScript : public Behaviour
{
public:
	Transform* pModelTr;

public:
	virtual void Initialize() override;
	virtual void Update() override;
};