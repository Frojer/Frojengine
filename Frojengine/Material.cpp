#include "Material.h"

CMaterial::CMaterial()
{
	m_diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_ambient = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_specular = XMFLOAT3(1.0f, 1.0f, 1.0f);
}


CMaterial::~CMaterial()
{

}