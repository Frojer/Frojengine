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

	void AddScene(CScene* i_pScene);

	void ChangeScene();
	bool ChangeAsyncScene(LPCWSTR i_sceneName);
	bool ChangeAsyncScene(int i_sceneNumber);

public:
	~SceneManager();

	static SceneManager* GetInstance();

	bool LoadScene(LPCWSTR i_sceneName);
	bool LoadScene(UINT i_sceneNumber);

	// void FJSystemEngine::LoadData()
	// void FJSystemEngine::Run()
	friend class FJSystemEngine;
};