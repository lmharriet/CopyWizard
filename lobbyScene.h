#pragma once
#include "gameNode.h"
#include "player.h"
#define MAXSTARTITEM 18

struct tagStartItem
{
	tagItem item;	  // 이름, 정보, 가격
	RECT rc;		  // 충돌 처리
	bool isPurchased; // 이 아이템을 한번이라도 산 적이 있다면
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

