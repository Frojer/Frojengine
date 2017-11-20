#pragma once

#include "Frojengine.h"

class Component : public IObject
{
protected:
	unsigned char _type;

public:
	Component() { _type = 0; }
	~Component() {}

protected:
	virtual void Update() = 0;
	virtual void Render() = 0;

	friend class CObject;
};