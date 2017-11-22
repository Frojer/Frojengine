#include "Behaviour.h"

Behaviour::Behaviour()
{
	_type = COMPONENT_TYPE_INITIALIZE | COMPONENT_TYPE_UPDATE;
	_bEnable = 0x11;
}

Behaviour::~Behaviour()
{

}

bool Behaviour::IsChanged()
{
	if ((_bEnable & 0x01) != (_bEnable & 0x10) >> 4)
	{
		_bEnable |= (_bEnable & 0x01) << 4;
		return true;
	}

	return false;
}


void Behaviour::SetEnable(bool enable)
{
	if (enable)
		_bEnable |= 0x01;
	else
		_bEnable &= 0x10;
}


bool Behaviour::GetEnable()
{
	return (_bEnable & 0x01) == 0x01;
}