#pragma once
#include "gameNode.h"
//=============================================================
//	## progressBar ## (���α׷����� = ü�¹� = �ε��� ���)
//=============================================================

class progressBar : public gameNode
{
private:
	image* _progressBarFront;		//ü�¹� �� �̹���(��Ʈ�����)
	image* _progressBarBack;		//ü�¹� �� �̹���(�������)
	RECT _rcProgress;				//ü�¹� ��Ʈ
	float _x, _y;					//ü�¹� ��ġ��ǥ
	float _width;					//ü�¹� ���α���
	float _height;					//ü�¹� ���α���

public:
	HRESULT init(const char* frontImg, const char* backImg);
	void release();
	void update();
	void render();

	//ü�¹� ��ġ �����ϱ�
	void setPos(float x, float y) { _x = x; _y = y; }
	//ü�¹� ������ �����ϱ�
	void setGauge(float maxHp, float currentHp);

};

