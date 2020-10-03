#pragma once
#include "gameNode.h"
#include "player.h"
class bossScene : public gameNode
{
	player* _player;
	image* _background;
public:
	HRESULT init();
	void release();
	void update();
	void render();
};