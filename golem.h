#pragma once
#include "monster.h"
class golem : public monster
{
public:
	HRESULT init(tagTile* tile)override ;
	void release()override ;
	void update() override;
	void render() override;
	inline POINT getPos() { return pos; } 
	inline RECT getRC() { return rc; }
	inline void setPlayerRC(RECT rc) { playerRC = rc; }
	inline void setCamRC(RECT rc) { camRC = rc; }

};

