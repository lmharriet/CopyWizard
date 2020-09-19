#pragma once
#include "gameNode.h"
#include "tileNode.h"

class mapToolScene : public gameNode
{
private:
	RECT option[3];
public:
	HRESULT init();
	void release();
	void update();
	void render();

	void imageRender(string keyName, POINT pt);
	void textRender();
};