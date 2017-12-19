#pragma once

#include "Frojengine.h"
#include "System.h"

class MirrorScript : public Behaviour
{
public:
	Transform* pModelTr;
	System* pSystem;

private:
	void SettingRenderer(list<CObject*> children);

public:
	virtual void Initialize() override;
	virtual void Update() override;
};