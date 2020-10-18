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

	vector<tagWizardArcana> vArcana;
	tagInvenCol arcanaSlot[6];

	vector<tagItem> vInven;	
	tagInvenCol col[14];
	RECT descriptBox;


public:
	HRESULT init();
	void release();
	void update();
	void render();

	void itemExplanation();

};


