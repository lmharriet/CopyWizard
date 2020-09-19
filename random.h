#pragma once
#include "singletonBase.h"

class random : public singletonBase <random>
{
public:
	HRESULT init();
	void release();

	int range(int num);
	int range(int min, int max);
	float range(float num);
	float range(float min, float max);

	random() {}
	~random() {}
};

