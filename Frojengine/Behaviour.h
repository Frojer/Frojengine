#pragma once

#include "Frojengine.h"

class Behaviour : public Component
{
private:
	byte _bEnable;

private:
	bool IsChanged();

public:
	Behaviour();
	virtual ~Behaviour();

	void SetEnable(bool enable);
	bool GetEnable();
};