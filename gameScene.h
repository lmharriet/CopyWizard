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
	
	player* _player;			//플레이어 클래스
	
	image* bgImg;				//백그라운드 이미지
	image* playerImg;			//플레이어이미지
	image* uiImg;

	shop* _shop;

	wall* _wall;

	unique_ptr<chest> _chest;

	RECT cam;
	RECT checkArea;

	int atkCount;

	//사운드

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

	void checkEnemDie();

	void playerAttack();
	void enemyAttack();
	void enemyUnitRenderInit();
	void attackChest();
	void gainArcana();
	void viewText();
	//sound
	void soundInit();
	void warp();
};