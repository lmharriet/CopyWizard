#pragma once
#include "singletonBase.h"
//#include "gameNode.h"
//=============================================================
//	## sceneManager ## (���Ŵ���)
//=============================================================

//�̱����� ��ӹ��� ���̵��� static���� ����� ���⶧����
//�Ϲ�Ŭ������ ���ӳ�带 ����Ϸ��� �ϸ� ������

//���ӳ�� Ŭ���� ���漱��
class gameNode;

class sceneManager : public singletonBase <sceneManager>
{
private:
	typedef map<string, shared_ptr<gameNode>> mSceneList;
	typedef map<string, shared_ptr<gameNode>>::iterator miSceneList;

private:	
	//map<string, gameNode*> _mSceneList;			//������ ��(ȭ��)���� ��Ƶ� ��
	mSceneList _mSceneList;			//������ ��(ȭ��)���� ��Ƶ� ��
	shared_ptr<gameNode> _currentScene;		//�����

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//���߰�
	shared_ptr<gameNode> addScene(string sceneName, shared_ptr<gameNode> scene);
	//������
	HRESULT loadScene(string sceneName);

	sceneManager() : _currentScene(nullptr) {}
	~sceneManager() {}
};

