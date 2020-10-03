#pragma once
#include "monster.h"
class golem : public monster
{
public:
	HRESULT init(tagTile* tile, POINT _pos)override ;
	void release()override ;
	void update() override;
	void render() override;

	void stateImageRender() override;
	void stateImage(int indexX_L, int indexY_L, int indexX_R, int indexY_R) override;

	inline POINT getPos() { return pos; } 
	inline RECT getRC() { return rc; }
	inline void setPlayerRC(RECT rc) { playerRC = rc; }
	inline void setCamRC(RECT rc) { camRC = rc; }

};

