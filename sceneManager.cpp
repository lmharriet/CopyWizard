#include "stdafx.h"
#include "sceneManager.h"
#include "gameNode.h"

HRESULT sceneManager::init()
{
	return S_OK;
}

void sceneManager::release()
{
	

	for (auto scene : _mSceneList)
	{
		//cout << scene.second << endl;
		scene.second->release();
		if (scene.first != "인게임")
		{
			SAFE_DELETE(scene.second);
		}
	}
	
	
}

void sceneManager::update()
{
	//현재씬이 존재한다면 해당 씬을 업데이트해라
	if (_currentScene) _currentScene->update();
}

void sceneManager::render()
{
	//현재씬이 존재한다면 해당 씬을 출력해라
	if (_currentScene) _currentScene->render();
}

gameNode * sceneManager::addScene(string sceneName, gameNode * scene)
{
	//씬이 없으면 그냥 널 리턴
	if (!scene) return NULL;

	//씬이 있다면 그씬을 리턴시키고 맵에 담아두기
	_mSceneList.insert(make_pair(sceneName, scene));
	//_mSceneList.insert(pair<string, gameNode*>(sceneName, scene));

	return scene;
}

HRESULT sceneManager::loadScene(string sceneName)
{
	miSceneList find = _mSceneList.find(sceneName);

	//못찾았다면 E_FAIL
	if (find == _mSceneList.end()) return E_FAIL;

	//바꾸려는 씬과 현재씬이 같다면 E_FAIL
	if (find->second == _currentScene) return E_FAIL;

	//여기까지 왔으면 크게 문제가 없으니 씬을 변경처리하자
	if (SUCCEEDED(find->second->init()))
	{
		_currentScene = find->second;
		return S_OK;
	}

	return E_NOTIMPL;
}
