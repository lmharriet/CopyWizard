#pragma once
#include "gameNode.h"

#define CAMSIZEX 600
#define CAMSIZEY 400

class camera : public gameNode
{
private:
	image* _img;		//��׶��� �̹���
	RECT _rcCam;		//ī�޶� ��Ʈ

public:
	HRESULT init();
	void release();
	void update();
	void render();

	camera() {}
	~camera() {}
};

