#pragma once
#include "gameNode.h"
#include "tileNode.h"

class mapToolScene : public gameNode
{
private:
	///TILE///
	tagMap tile[MAXTILE]; //기본 맵 타일

	//SINGLE//
	tagSample sTile[9]; //선택 맵 타일 <단일>
	//  SET //
	tagSample sTile_set;//선택 맵 타일 <세트>
	///BUTTON///
	RECT dragChangeButton;

	deque<int> backup;

	tagMouse mouse;
	STATE state;

	vector<int> sIndex;

	//button down
	bool isLeftDown;
	bool isLeftUp;
	bool isLeftkey;

	//cam
	float camX, camY;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	void save();
	void load();
	void dragTile();
	void singleTile();

	bool checkPt(POINT pt, POINT _pt);

	//text view
	void textRender();

	//cam
	RECT moveRC(int x, int y, int width, int height) { return RectMake(x + camX, y + camY, width, height); }
};