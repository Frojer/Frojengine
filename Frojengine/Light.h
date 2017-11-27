#pragma once

#include "Frojengine.h"

enum
{
	LIGHT_TYPE_DIRECTION,
	LIGHT_TYPE_POINT,
	LIGHT_TYPE_SPOT
};

class Behaviour;

class Light : public Behaviour
{
private:
	static list<Light*> _lightList;
public:
	UINT	m_lightType;
	COLOR	m_diffuse;			// ÁÖ ±¤·® : È®»ê±¤ Diffuse Light.
	COLOR	m_ambient;			// º¸Á¶ ±¤·® : ÁÖº¯±¤ Ambient Light.
	FLOAT	m_range;			// ºû µµ´Þ °Å¸®.

public:
	Light();
	virtual ~Light();

	virtual void AfterUpdate() override;

	friend class CMaterial;
};