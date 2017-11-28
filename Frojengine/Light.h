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
	COLOR	m_diffuse;			// ¡÷ ±§∑Æ : »ÆªÍ±§ Diffuse Light.
	COLOR	m_ambient;			// ∫∏¡∂ ±§∑Æ : ¡÷∫Ø±§ Ambient Light.
	COLOR	m_specular;			// ∫∏¡∂ ±§∑Æ : ¡÷∫Ø±§ Ambient Light.
	FLOAT	m_range;			// ∫˚ µµ¥ﬁ ∞≈∏Æ.

public:
	Light();
	virtual ~Light();

	virtual void AfterUpdate() override;

	friend class CMaterial;
};