#pragma once

#include "Frojengine.h"

class IObject;

#define CheckComponentType(checkType, type) ((checkType) & type) == type
enum
{
	COMPONENT_TYPE_UPDATE = 0x80,
	COMPONENT_TYPE_RENDER = 0x40
};

class Component : public IObject
{
protected:
	// Update Render
	// 0      0      000000
	unsigned char _type;
	CObject* _pObj;

public:
	Component() : _type(0), _pObj(nullptr) {}
	~Component() {}

protected:
	virtual void Update() = 0;
	virtual void Render() = 0;

	friend class CObject;
};