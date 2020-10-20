#pragma once
#include "gameNode.h"
class finalBoss : public gameNode
{
private:
public:
	HRESULT init(int _posX, int _posY);
	void release();
	void update();
	void render();
};

