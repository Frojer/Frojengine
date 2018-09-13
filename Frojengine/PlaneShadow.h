#pragma once

#include "Frojengine.h"
#include "System.h"

class PlaneShadow : public Behaviour
{
public:
	Transform* m_pModelTr;
	CTexture2D* tex = nullptr;
	System* sys;
private:
	void InitRenderer(list<CObject*> children);
	void UpdateRenderer(list<CObject*> children);

public:
	virtual void Initialize() override;
	virtual void Update() override;
};