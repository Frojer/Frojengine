#include "Light.h"

list<Light*> Light::_lightList;

Light::Light()
	: m_lightType(LIGHT_TYPE_DIRECTION), m_diffuse(1.0f, 1.0f, 1.0f, 1.0f), m_ambient(1.0f, 1.0f, 1.0f, 1.0f), m_specular(1.0f, 1.0f, 1.0f, 1.0f), m_range(0.0f)
{
	_lightList.push_back(this);
}


Light::~Light()
{
	
}



void Light::AfterUpdate()
{

}