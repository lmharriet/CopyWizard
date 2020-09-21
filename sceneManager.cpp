#include "stdafx.h"
#include "sceneManager.h"
#include "gameNode.h"

HRESULT sceneManager::init()
{
	return S_OK;
}

void sceneManager::release()
{
	/*
	//�������� ���
	miSceneList iter = _mSceneList.begin();
	for (; iter != _mSceneList.end();)
	{
		if (iter->second != NULL)
		{
			//���⼭ ����
			if (iter->second == _currentScene) iter->second->release();
			SAFE_DELETE(iter->second);
			iter = _mSceneList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	//����ü�� Ŭ���� �ϱ�
	_mSceneList.clear();
	*/

	//C++ ����� ����
	//for(auto scene in _mSceneList) => ������ ���Ұ�
	
	///////// 21 SEP 20 ������ ��� off//////////////

	for (auto scene : _mSceneList)
	{
		cout << scene.second << endl;
		scene.second->release();
		//SAFE_DELETE(scene.second);
	}
	// SAFE_DELETE�� �ϴ� off �ص�
	/////////////////////////////////////////////
	
	//int a = 10;
	//auto b = 100.5f;
	//var c = 10; //����Ƽ���� ���....
	
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
