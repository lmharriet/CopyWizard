#pragma once
#include "gameNode.h"

class iniTestScene : public gameNode
{
public:
	HRESULT init();
	void release();
	void update();
	void render();
};

