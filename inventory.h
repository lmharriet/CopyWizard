#pragma once
#include "gameNode.h"
class inventory : public gameNode
{
private:

	RECT rc;
	bool isOpen;
	RECT slot;
	vector<RECT>_vSlot;
public:
	HRESULT init();
	void release();
	void update();
	void render();
};


