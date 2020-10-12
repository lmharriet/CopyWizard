#pragma once
#include "gameNode.h"
struct tagShop
{
	string keyName;
	POINT pt;
};
class shop : public gameNode
{
private:
	tagShop npc[3];
public:
	HRESULT init();
	void addImage();
	void generate(POINT arr[3]);
	void render();
};