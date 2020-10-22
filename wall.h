#pragma once
#include "gameNode.h"

class player;

class wall : public gameNode
{
private:
	//맵타일 정보
	tagTile tile[MAXTILE];
	tagTile subTile[MAXTILE];

	// 메모리 관리용
	vector<int> vTile;
	vector<int> vWall;

	POINT culPt;
	//가져오기
	RECT* cam;
	player* _player;
	bool isLoad;

	bool isFrameRect;
	//memory
	int size;
	int num;
public:
	HRESULT init();
	void loadMap(int index);
	void update();
	void render();

	void render2();

	void collisionTile();

	bool getFrameRect() { return isFrameRect; }
	void setFrameRect(bool temp) { isFrameRect = temp; }
	//
	void getRectAd(RECT* _cam) { cam = _cam; }
	POINT getCulPt() { return culPt; }

	tagTile* getTile() { return tile; }
	tagTile* getSubTile() { return subTile; }

	vector<int> getVtile() { return vTile; }
	vector<int> getVwall() { return vWall; }

	void getPlayerAd(player* player) { _player = player; }
};