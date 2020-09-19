#pragma once
#include "gameNode.h"
#include "player.h"
#include "enemyManager.h"

class gameScene : public gameNode
{
private:
	player* _player;				//�÷��̾� Ŭ����
	enemyManager* _enemyManager;	//���ʹ̸Ŵ��� Ŭ����

public:
	HRESULT init();
	void release();
	void update();
	void render();

};

