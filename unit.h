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
	vector<zObject*> vUnit;
	vector<tagEnemy> vEnemy;

	vector<int> wall;

	vector<zObject*> tempObject;

	zObject* player;
	zObject npc[3];
	//
	tagTile* tile;
public:
	HRESULT init();

	//object pooling
	zObject* getObject();
	void rtnObject(zObject* _object);

	void addUnit(int _index, string _keyName, string type, POINT frame, float _x = 0, float _y = 0);
	void setFramePlayer(POINT frame) { player->setFrame(frame); }
	void setPlayerRect(RECT rc) { player->setRect(rc); }
	void enemyInit(int kind, POINT currentFrame, POINT pos);
	void enemyClear() { vEnemy.clear(); }
	void release();
	void update();
	void render(HDC hdc);

	void sorting();

	vector<zObject*> getVector() { return vUnit; }
};