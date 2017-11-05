#include "SceneManager.h"

CScene* SceneManager::pCurrentScene = nullptr;


bool SceneManager::ChangeScene(LPCWSTR i_sceneName)
{
	bool result;

	if (pCurrentScene != nullptr)
	{
		pCurrentScene->Release();
	}

	pCurrentScene = _mapScene[i_sceneName];

	result = pCurrentScene->DataLoading();

	if (!result)
		return false;

	return true;
}

bool SceneManager::ChangeScene(int i_sceneNumber)
{
	return true;
}