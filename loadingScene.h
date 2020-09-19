#pragma once
#include "gameNode.h"
#include "loading.h"

class loadingScene : public gameNode
{
private:
	loading* _loading;

	loadItem* item;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	//�ε��̹��� �Լ� (�̰��� �̹����� ���� �־��)
	void loadingImage();
	//�ε����� �Լ� (�̰��� ���带 ���� �־��)
	void loadingSound();
};

