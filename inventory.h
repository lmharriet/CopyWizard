#pragma once
#include "gameNode.h"
struct tagInvenCol
{
	RECT rc;
	bool isCol;
	POINT pos;
};

class inventory : public gameNode
{
private:

	RECT rc;
	bool isOpen;

	RECT slot;
	vector<RECT>_vSlot;
	
	vector<tagItem> vInven;	
	tagInvenCol col[14];
	RECT descriptBox;

	//vector<string> vInven;




public:
	HRESULT init();
	void release();
	void update();
	void render();

	void itemExplanation();

};


