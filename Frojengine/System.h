#pragma once

#include "Frojengine.h"

class System : public Behaviour
{
private:
	COLOR clearColor;
	const COLOR clearColArr[4] =
	{
		{ 0.45098f, 0.76862f, 1.0f, 1.0f },
		{ 1.0f, 0.55294f, 0.05882f, 1.0f },
		{ 0.15294f, 0.15294f, 0.38823f, 1.0f },
		{ 0.55686f, 0.55686f, 0.82353f, 1.0f },
	};

	const COLOR dirLightColArr[4] =
	{
		{ 1.0f, 1.0f, 1.0f, 1.0f },
		{ 0.94902f, 0.42745f, 0.05882f, 1.0f },
		{ 0.0f, 0.0f, 0.24118f, 1.0f },
		{ 0.55686f, 0.55686f, 0.82353f, 1.0f },
	};

public:
	UINT seasonCount;
	UINT timeCount;
	float cold;
	CMaterial* pBoxMaterial;
	CMaterial* pTerrainMtrl;
	CMaterial* pTreeMtrl;
	CMaterial* pWindmillMtrl;
	Light* pDirectionalLight;

public:
	virtual void Initialize() override;
	virtual void Update() override;
};