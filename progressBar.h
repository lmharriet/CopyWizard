#pragma once
#include "gameNode.h"
//=============================================================
//	## progressBar ## (프로그래스바 = 체력바 = 로딩바 등등)
//=============================================================

class progressBar : public gameNode
{
private:
	image* _progressBarFront;		//체력바 앞 이미지(렉트변경됨)
	image* _progressBarBack;		//체력바 뒤 이미지(변경안함)
	RECT _rcProgress;				//체력바 렉트
	float _x, _y;					//체력바 위치좌표
	float _width;					//체력바 가로길이
	float _height;					//체력바 세로길이

public:
	HRESULT init(const char* frontImg, const char* backImg);
	void release();
	void update();
	void render();

	//체력바 위치 세팅하기
	void setPos(float x, float y) { _x = x; _y = y; }
	//체력바 게이지 세팅하기
	void setGauge(float maxHp, float currentHp);

};

