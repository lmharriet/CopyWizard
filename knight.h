#pragma once
#include "monster.h"
class knight	:    public monster
{
private:
	bool atkDirection[MAX];

public:
	void addInit()override;
	//void release()override ;
	void update() override;
	void render() override;

	void stateImageRender();
	void stateImage(int indexX_L, int indexY_L, int indexX_R, int indexY_R);
};

