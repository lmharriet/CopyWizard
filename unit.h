#pragma once
#include "singletonBase.h"
#include "zObject.h"

class unit : public singletonBase<unit>
{
private:
	vector<zObject*> vUnit;

	vector<int> wall;
	zObject* player;
public:
	void addUnit(int _index, string _keyName, string type, POINT frame, float _x = 0, float _y = 0);
	void setFramePlayer(POINT frame) { player->setFrame(frame); }

	void release();
	void update();
	void render(HDC hdc);

	void sorting();

	vector<zObject*> getVector() { return vUnit; }
};