#pragma once
#include "gameNode.h"
#include "player.h"
#include "tileNode.h"
#include "colorNode.h"
#include "enemyManager.h"


class gameScene : public gameNode
{
private:
	monster* _golem;  //테스트중.. 몬스터(골렘)
	monster* _golem2;  //테스트중.. 몬스터(골렘)
	enemyManager* enemy;
	
	player* _player;			//플레이어 클래스
	
	image* bgImg;				//백그라운드 이미지
	image* playerImg;			//플레이어이미지
	image* uiImg;



	//맵타일 정보
	tagTile tile[MAXTILE];
	// 메모리 관리용
	vector<int> vTile;

	//test cam(수정해도됨)
	RECT cam;
	RECT checkArea;

	 int atkCount;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void collisionTile();

	void playerAttack();
	//맵 로드
	void loadMap(const char* mapFileName);

	void viewText();

};