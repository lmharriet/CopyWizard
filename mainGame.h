#pragma once
#include "gameNode.h"
/*������ ���ΰ����� ������ ���鸸 ������ �Ѵ�*/
/*������ ������ϸ� �̰��� �߰��Ѵ�*/
#include "startScene.h"
#include "iniTestScene.h"
#include "soundTestScene.h"
#include "pixelCollisionScene.h"
#include "loadingScene.h"
#include "mapToolScene.h"

class mainGame : public gameNode
{
private:
	float volume;
public:
	HRESULT init();
	void release();
	void update();
	void render();
};