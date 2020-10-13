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
	POINT frame;

	RECT rc;

	int price;
	bool isSell;
	bool isCol;
};
class shop : public gameNode
{
private:
	tagShop npc[3];

	//Andres
	tagSellItem AndresShop[5];

	//nox
	tagSellItem NoxShop[3];

	//
	int colItem;
public:
	HRESULT init();
	void addImage();
	void generate(POINT arr[3]);
	void render();

	void shopCollider(RECT rc);

	void buyItem();
};