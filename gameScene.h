#pragma once
#include "gameNode.h"
#include "player.h"
#include "tileNode.h"
#include "colorNode.h"
#include "enemyManager.h"
#include "shop.h"
#include "wall.h"
#include "chest.h"

class gameScene : public gameNode
{
public:
	gameScene();



private:
	shared_ptr<enemyManager> enemy;
	vector<int> camInEnemy;
	
	player* _player;			//�÷��̾� Ŭ����
	
	image* bgImg;				//��׶��� �̹���
	image* playerImg;			//�÷��̾��̹���
	image* uiImg;

	shop* _shop;

	wall* _wall;

	chest* _chest;

	RECT cam;
	RECT checkArea;

	int atkCount;

	//����

	bool isIngameBGM;
	float fadeIn;

	int warpDelay;
	bool isWarp;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	void enemyCheckCol();

	void playerAttack();
	void enemyAttack();
	void enemyUnitRenderInit();
	void attackChest();
	void viewText();
	//sound
	void soundInit();
	void warp();
};