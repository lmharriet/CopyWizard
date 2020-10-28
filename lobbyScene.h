#pragma once
#include "gameNode.h"
#include "player.h"
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

private:

	RECT rc;
	tagStartItem startingItem[MAXSTARTITEM];

	//18/3 = 6
	bool shopActive[MAXSTARTITEM / 3];


	//del

	bool temp;
public:

	HRESULT init();
	void release();
	void update();
	void render();

	void addImage();

	void viewText();

	void warpToGameScene();

	void buyStartingItem();

};

