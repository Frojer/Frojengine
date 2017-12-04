#pragma once

#include "Frojengine.h"

class IObject;

#define CheckComponentType(checkType, type) ((checkType) & type) == type
enum
{
	COMPONENT_TYPE_INITIALIZE = 0x80,
	COMPONENT_TYPE_UPDATE = 0x40,
	COMPONENT_TYPE_AFTERUPDATE = 0x20,
	COMPONENT_TYPE_RENDER = 0x10
};

class Component : public IObject
{
private:
	CObject* _pObj;

protected:
	// Init Update AfterUpdate Render
	// 0    0      0           00000
	unsigned char _type;

public:
	Component() : _type(0), _pObj(nullptr) {}
	virtual ~Component() {}

	CObject* GetMyObject() { return _pObj; }

protected:
	virtual void Initialize() {}
	virtual void Update() {}
	virtual void AfterUpdate() {}
	virtual void Render() {}

	friend class CObject;
};