#pragma once
#include "gameNode.h"

#define CAMSIZEX 600
#define CAMSIZEY 400

class camera : public gameNode
{
private:
	image* _img;		//백그라운드 이미지
	RECT _rcCam;		//카메라 렉트

public:
	HRESULT init();
	void release();
	void update();
	void render();

	camera() {}
	~camera() {}
};

