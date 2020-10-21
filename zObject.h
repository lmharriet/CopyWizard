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

	//tagTile* tile;
	RECT rc;
public:
	HRESULT init(int _index, string _keyName, string _type, POINT _frame, float _x, float _y);
	void release();
	void render(HDC hdc);

	string getKeyName() { return keyName; }
	RECT getRect() { return rc; }
	void setFrame(POINT _frame) { frame = _frame; }
	void setIndex(int value) { Index = value; }
	void setRect(RECT _rc) { rc = RectMakeCenter(_rc.left, _rc.top + 50, 50, 50); } // 캐릭터 발 아래에 생성하기 위함
};