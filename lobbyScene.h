#pragma once
#include "gameNode.h"
#include "player.h"
#include "shop.h"
#define MAXSTARTITEM 18

struct tagStartItem
{
	tagItem item;	  // �̸�, ����, ����
	RECT rc;		  // �浹 ó��
	bool isPurchased; // �� �������� �ѹ��̶� �� ���� �ִٸ�
};

class lobbyScene :  public gameNode
{
private:
	player* _player;
	shop* _shop;
private:

	RECT rc;
	tagStartItem startingItem[MAXSTARTITEM];

	//18/3 = 6
	bool shopActive[MAXSTARTITEM / 3];

	//warp
	bool sceneWarp;
	int sceneCount;
public:

	HRESULT init();
	void release();
	void update();
	void render();

	void addImage();

	void viewText();

	void warpToGameScene();

	void itemInfo();

	void buyStartingItem();

};

