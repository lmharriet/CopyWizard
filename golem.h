#pragma once
#include "monster.h"
class golem : public monster
{
public:
	HRESULT init()override ;
	void release()override ;
	void update() override;
	void render() override;
};

