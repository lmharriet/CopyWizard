#pragma once
#include "gameNode.h"
#include "tileNode.h"

class mapToolScene : public gameNode
{
private:

public:
	HRESULT init();
	void release();
	void update();
	void render();
};