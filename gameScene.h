#pragma once
#include "gameNode.h"
#include "player.h"
#include "tileNode.h"
#include "colorNode.h"
#include "golem.h" //�׽�Ʈ�� (���Ͱ�)
#include "enemyManager.h"


class gameScene : public gameNode
{
private:
	monster* _golem;  //�׽�Ʈ��.. ����(��)
	monster* _golem2;  //�׽�Ʈ��.. ����(��)
	enemyManager* enemy;
	
	player* _player;			//�÷��̾� Ŭ����
	
	image* bgImg;				//��׶��� �̹���
	image* playerImg;			//�÷��̾��̹���
	image* uiImg;



	//��Ÿ�� ����
	tagTile tile[MAXTILE];

	//test cam(�����ص���)
	RECT cam;
	RECT checkArea;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//�� �ε�
	void loadMap(const char* mapFileName);

	void viewText();

};