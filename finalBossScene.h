#pragma once
#include "gameNode.h"
#include "finalBoss.h"
class finalBossScene : public gameNode
{
private:
	finalBoss* _finalBoss;
public:
	HRESULT init();
	void release();
	void update();
	void render();
};

