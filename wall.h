#pragma once
#include "gameNode.h"

class player;

class wall : public gameNode
{
private:
	//��Ÿ�� ����
	tagTile tile[MAXTILE];
	tagTile subTile[MAXTILE];

	// �޸� ������
	vector<int> vTile;
	vector<int> vWall;

	POINT culPt;
	//��������
	RECT* cam;
	player* _player;
public:
	HRESULT init();
	void loadMap(int index);
	void update();
	void render();

	void collisionTile();



	//
	void getRectAd(RECT* _cam) { cam = _cam; }
	POINT getCulPt() { return culPt; }

	tagTile* getTile() { return tile; }
	tagTile* getSubTile() { return subTile; }

	vector<int> getVtile() { return vTile; }
	vector<int> getVwall() { return vWall; }

	void getPlayerAd(player* player) { _player = player; }
};