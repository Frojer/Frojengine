#pragma once

#include "Frojengine.h"

class MainScene : public CScene
{
public:

public:
	MainScene(LPDEVICE i_pDevice, LPDXDC i_pDXDC);
	~MainScene();

	virtual bool Load();
};