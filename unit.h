#pragma once
#include "singletonBase.h"
#include "zObject.h"

class unit : public singletonBase<unit>
{
private:
	struct tagEnemy {
		int kind;
		POINT currentFrame;
		float posX, posY;
	};
	vector<shared_ptr<zObject>> vUnit;
	vector<tagEnemy> vEnemy;

	vector<int> wall;

	vector<shared_ptr<zObject>> tempObject;

	shared_ptr<zObject> player;

	shared_ptr<zObject> npc[2];
	//
	shared_ptr<zObject> chest;

	tagTile* tile;
public:
	HRESULT init();

	void addUnit(int _index, string _keyName, string type, POINT frame, float _x = 0, float _y = 0);
	void setFramePlayer(POINT frame) { player->setFrame(frame); }
	void setFrameChest(POINT frame) { chest->setFrame(frame); }
	void setIndexChest(int value) { chest->setIndex(value); }
	void setPlayerRect(RECT rc) { player->setRect(rc); }
	void enemyInit(int kind, POINT currentFrame, POINT pos);
	void enemyClear() { vEnemy.clear(); }
	void release();
	void update();
	void render(HDC hdc);

	void sorting();

	vector<shared_ptr<zObject>> getVector() { return vUnit; }
};