#pragma once
#include "monster.h"

class golem : public monster
{
	

private:
	bool atkDirection[MAX];
	POINT smallSlashIndex;
	

public:
	void addInit()override;
	void update() override;
	void render() override;

	void stateImageRender();


	void stateIDLE() override;
	void stateATK()	override;
	void stateDIE()	override;

//	void stateImage(int indexX_L, int indexY_L, int indexX_R, int indexY_R);
	

	//inline POINT getPos() { return pos; } 
	//inline void setPlayerRC(RECT rc) { playerRC = rc; }
	//inline void setCamRC(RECT rc) { camRC = rc; }

};

