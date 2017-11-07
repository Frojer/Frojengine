#pragma once

#include "Frojengine.h"

class CScene;

class SceneManager
{
private:
	static SceneManager* _pInstance;

	vector<CScene*> _vecScene;
	CScene* _pChangeScene;
	bool _bReadyToAsyncChange;

public:
	static CScene* pCurrentScene;

private:
	SceneManager();

public:
	~SceneManager();

	static SceneManager* GetInstance();

	void AddScene(CScene* i_pScene);

	bool LoadScene(LPCWSTR i_sceneName);
	bool LoadScene(UINT i_sceneNumber);

	void ChangeScene();
	bool ChangeAsyncScene(LPCWSTR i_sceneName);
	bool ChangeAsyncScene(int i_sceneNumber);
};