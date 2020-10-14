#pragma once
#include "tileNode.h"

class zObject
{
private:
	float x, y;

	string type;
	POINT frame;

	int Index;
	string keyName;

	tagTile* tile;
	RECT rc;
public:
	HRESULT init(int _index, string _keyName, string _type, POINT _frame, float _x, float _y);
	void release();
	void render(HDC hdc);

	RECT getRect() { return rc; }
	void setFrame(POINT _frame) { frame = _frame; }
};