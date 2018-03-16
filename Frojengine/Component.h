#pragma once

#include "Frojengine.h"

class IObject;

#define CheckComponentType(checkType, type) (((checkType) & type) == type)
#define IsInitialize(check) (((check) & 0x80) == 0x80)
enum
{
	COMPONENT_TYPE_UPDATE = 0x80,
	COMPONENT_TYPE_AFTERUPDATE = 0x40,
	COMPONENT_TYPE_RENDER = 0x20
};

class Component : public IObject
{
private:
	CObject* _pObj;

protected:
	// Update AfterUpdate Render
	// 0      0           0      0000
	unsigned char _type;
	
	// IsInit? 
	// 0       0000000
	unsigned char _check;

public:
	Component() : _type(0), _check(0), _pObj(nullptr) {}
	virtual ~Component() {}

	CObject* GetMyObject() { return _pObj; }

protected:
	virtual void Initialize() {}
	virtual void Update() {}
	virtual void AfterUpdate() {}
	virtual void Render() {}

	friend class CObject;
};