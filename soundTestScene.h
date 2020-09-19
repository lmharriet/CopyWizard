#pragma once
#include "gameNode.h"

class soundTestScene : public gameNode
{
public:
	HRESULT init();
	void release();
	void update();
	void render();
};

