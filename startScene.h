#pragma once
#include "gameNode.h"

class startScene : public gameNode
{
private:
	RECT option[3];
	bool isCol;
	//BGM 
	bool isTitleBGM;
	float fadeIn;


	int x, y;
public:
	HRESULT init();
	void release();
	void update();
	void render();

};