#pragma once
#include "gameNode.h"
/*������ ���ΰ����� ������ ���鸸 ������ �Ѵ�*/
/*������ ������ϸ� �̰��� �߰��Ѵ�*/
#include "startScene.h"
#include "gameScene.h"
#include "soundTestScene.h"
#include "pixelCollisionScene.h"
#include "mapToolScene.h"
#include "bossScene.h"

class mainGame : public gameNode
{
private:
	float volume;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	void volumeControl();
};