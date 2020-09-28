#pragma once
#include "gameNode.h"
class monster : public gameNode
{
protected:
	image* img;
	float x, y;

public:
	virtual HRESULT init()=0;
	virtual void release()=0;
	virtual void update()=0;
	virtual void render()=0;
};

