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
	player* _player;			//�÷��̾� Ŭ����

	image* bgImg;				//��׶��� �̹���
	image* playerImg;			//�÷��̾��̹���
	image* uiImg;

	tagCollider pixelCheck[4];

	//��Ÿ�� ����
	tagTile tile[MAXTILE];

	//test cam(�����ص���)
	RECT cam;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//�� �ε�
	void loadMap(const char* mapFileName);

	void viewText();

};

