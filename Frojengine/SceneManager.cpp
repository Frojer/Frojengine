#include "SceneManager.h"

SceneManager* SceneManager::_pInstance = nullptr;
CScene* SceneManager::pCurrentScene = nullptr;

SceneManager::SceneManager()
	: _pChangeScene(nullptr), _bReadyToAsyncChange(false)
{

}


SceneManager::~SceneManager()
{
	for (UINT i = 0; i < _vecScene.size(); i++)
	{
		_vecScene[i]->Release();
		delete _vecScene[i];
		_vecScene[i] = nullptr;
	}

	_vecScene.clear();
}


void SceneManager::AddScene(CScene* i_pScene)
{
	_vecScene.push_back(i_pScene);
}


SceneManager* SceneManager::GetInstance()
{
	if (_pInstance == nullptr)
	{
		_pInstance = new SceneManager();
	}

	return _pInstance;
}


bool SceneManager::LoadScene(LPCWSTR i_sceneName)
{
	if (_pChangeScene != nullptr)
		return false;

	for (UINT i = 0; i < _vecScene.size(); i++)
	{
		if (wcscmp(_vecScene[i]->m_Name, i_sceneName) == 0)
		{
			_pChangeScene = _vecScene[i];
			break;
		}
	}

	if (_pChangeScene == nullptr)
		return false;

	return true;
}


bool SceneManager::LoadScene(UINT i_sceneNumber)
{
	if (_pChangeScene != nullptr)
		return false;

	if (_vecScene.size() <= i_sceneNumber)
		return false;

	_pChangeScene = _vecScene[i_sceneNumber];

	return true;
}


void SceneManager::ChangeScene()
{
	if (_pChangeScene == nullptr)
		return;

	SAFE_RELEASE(SceneManager::pCurrentScene);

	SceneManager::pCurrentScene = _pChangeScene;
	_pChangeScene = nullptr;

	SceneManager::pCurrentScene->Load();
}