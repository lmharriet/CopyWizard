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
		if (scene.first != "�ΰ���")
		{
			SAFE_DELETE(scene.second);
		}
	}
	
	
}

void sceneManager::update()
{
	//������� �����Ѵٸ� �ش� ���� ������Ʈ�ض�
	if (_currentScene) _currentScene->update();
}

void sceneManager::render()
{
	//������� �����Ѵٸ� �ش� ���� ����ض�
	if (_currentScene) _currentScene->render();
}

gameNode * sceneManager::addScene(string sceneName, gameNode * scene)
{
	//���� ������ �׳� �� ����
	if (!scene) return NULL;

	//���� �ִٸ� �׾��� ���Ͻ�Ű�� �ʿ� ��Ƶα�
	_mSceneList.insert(make_pair(sceneName, scene));
	//_mSceneList.insert(pair<string, gameNode*>(sceneName, scene));

	return scene;
}

HRESULT sceneManager::loadScene(string sceneName)
{
	miSceneList find = _mSceneList.find(sceneName);

	//��ã�Ҵٸ� E_FAIL
	if (find == _mSceneList.end()) return E_FAIL;

	//�ٲٷ��� ���� ������� ���ٸ� E_FAIL
	if (find->second == _currentScene) return E_FAIL;

	//������� ������ ũ�� ������ ������ ���� ����ó������
	if (SUCCEEDED(find->second->init()))
	{
		_currentScene = find->second;
		return S_OK;
	}

	return E_NOTIMPL;
}
