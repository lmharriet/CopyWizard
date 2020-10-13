#pragma once
#include "gameNode.h"
struct tagShop
{
	string keyName;
	POINT pt;
};

struct tagSellItem
{
	string keyName;
	POINT pt;
	int price;
	bool isSell;
};
class shop : public gameNode
{
private:
	tagShop npc[3];

	//Andres
	tagSellItem AndresShop[5];

	//nox
	tagSellItem NoxShop[3];
public:
	HRESULT init();
	void addImage();
	void generate(POINT arr[3]);
	void render();
};