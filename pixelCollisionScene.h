#pragma once
#include "gameNode.h"

class pixelCollisionScene : public gameNode
{
private:
	image* _mountain;		//백그라운드 이미지
	image* _ball;			//공이미지
	RECT _rc;				//공렉트
	float _x, _y;			//위치좌표
	int _probeY;			//픽셀충돌 탐지할 Y값

public:
	HRESULT init();
	void release();
	void update();
	void render();
};

