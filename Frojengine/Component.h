#pragma once

#include "Frojengine.h"

class IObject;

#define CheckComponentType(checkType, type) ((checkType) & type) == type
enum
{
	COMPONENT_TYPE_UPDATE = 0x80,
	COMPONENT_TYPE_AFTERUPDATE = 0x40,
	COMPONENT_TYPE_RENDER = 0x20
};

class Component : public IObject
{
protected:
	// Update AfterUpdate Render
	// 0      0           0      00000
	unsigned char _type;
	CObject* _pObj;

public:
	Component() : _type(0), _pObj(nullptr) {}
	~Component() {}

protected:
	virtual void Update() {}
	virtual void AfterUpdate() {}
	virtual void Render() {}

	friend class CObject;
};