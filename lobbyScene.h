#pragma once
#include "gameNode.h"
#include "player.h"
class lobbyScene :  public gameNode
{
private:
	player* _player;

private:

	RECT rc;
public:

	HRESULT init();
	void release();
	void update();
	void render();

	void addImage();

	void viewText();

	void warpToGameScene();

};

