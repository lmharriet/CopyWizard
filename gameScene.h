#pragma once
#include "gameNode.h"
#include "player.h"
#include "tileNode.h"
#include "colorNode.h"
#include "enemyManager.h"
#include "shop.h"
#include "wall.h"

class gameScene : public gameNode
{
public:
	gameScene();



private:
	shared_ptr<enemyManager> enemy;
	
	player* _player;			//�÷��̾� Ŭ����
	
	image* bgImg;				//��׶��� �̹���
	image* playerImg;			//�÷��̾��̹���
	image* uiImg;

	shop* _shop;

	wall* _wall;

	RECT cam;
	RECT checkArea;

	int atkCount;

	//����

	bool isIngameBGM;
	float fadeIn;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void playerAttack();
	void enemyAttack();
	void enemyUnitRenderInit();
	void viewText();
	//sound
	void soundInit();

};