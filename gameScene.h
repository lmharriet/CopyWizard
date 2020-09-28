#pragma once
#include "gameNode.h"
#include "player.h"
#include "tileNode.h"
#include "colorNode.h"
struct tagCollider
{
	RECT rc;
	POINT pos;
	bool isCol;
};

class gameScene : public gameNode
{
private:
	player* _player;			//플레이어 클래스

	image* bgImg;				//백그라운드 이미지
	image* playerImg;			//플레이어이미지
	image* uiImg;

	tagCollider pixelCheck[4];

	//맵타일 정보
	tagTile tile[MAXTILE];

	//test cam(수정해도됨)
	RECT cam;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//맵 로드
	void loadMap(const char* mapFileName);

	void viewText();

};

