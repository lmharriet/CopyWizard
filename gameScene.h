#pragma once
#include "gameNode.h"
#include "player.h"
#include "tileNode.h"
#include "colorNode.h"
#include "enemyManager.h"
#include "shop.h"

class gameScene : public gameNode
{
public:
	gameScene();



private:
	
	enemyManager* enemy;
	
	player* _player;			//플레이어 클래스
	
	image* bgImg;				//백그라운드 이미지
	image* playerImg;			//플레이어이미지
	image* uiImg;

	shop* _shop;

	//맵타일 정보
	tagTile tile[MAXTILE];
	tagTile subTile[MAXTILE];

	// 메모리 관리용
	vector<int> vTile;

	vector<int> vWall;

	//test cam(수정해도됨)
	RECT cam;
	RECT checkArea;

	POINT culPt;
	bool oneTime;

	 int atkCount;

	 //사운드

	 bool isIngameBGM;
	 float fadeIn;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void collisionTile();

	void playerAttack();
	void enemyAttack();
	//맵 로드
	void loadMap(int index);
	//void loadMap(const char* mapFileName);

	void viewText();
	//sound
	void soundInit();

};