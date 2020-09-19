#pragma once
#include "gameNode.h"
#include "tileNode.h"

class mapToolScene : public gameNode
{
private:
	///TILE///
	tagMap tile[MAXTILE]; //�⺻ �� Ÿ��

	//SINGLE//
	tagSample sTile[9]; //���� �� Ÿ�� <����>
	//  SET //
	tagSample sTile_set;//���� �� Ÿ�� <��Ʈ>
	///BUTTON///
	RECT dragChangeButton;

	deque<int> backup;

	tagMouse mouse;
	STATE state;

	//button down
	bool isLeftDown;
	bool isLeftUp;
	bool isLeftkey;

	//cam
	RECT camRC;
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