#pragma once

#include "Frojengine.h"

class CScene;

class SceneManager
{
private:
	unordered_map<LPCWSTR, CScene*> _mapScene;
	bool _bReadyToAsyncChange;

public:
	static CScene* pCurrentScene;

private:

public:
	bool ChangeScene(LPCWSTR i_sceneName);
	bool ChangeScene(int i_sceneNumber);
	bool ChangeAsyncScene(LPCWSTR i_sceneName);
	bool ChangeAsyncScene(int i_sceneNumber);
};