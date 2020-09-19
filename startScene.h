#pragma once
#include "gameNode.h"

class startScene : public gameNode
{
private:

	RECT option[3];
	bool isCol;

	int x, y;
public:
	HRESULT init();
	void release();
	void update();
	void render();
};