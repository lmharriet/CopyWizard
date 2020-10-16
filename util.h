#pragma once
//=============================================================
//	## namespace ## MY_UTIL
//=============================================================

#define PI 3.1415926f
#define PI2 (PI * 2)

//���߿� �̹��� ȸ���� ����� ��
#define PI_2 (PI / 2) //90��
#define PI_4 (PI / 4) //45��
#define PI_8 (PI / 8) //22.5��

namespace MY_UTIL
{
	//���� ������ �Ÿ�
	float getDistance(float startX, float startY, float endX, float endY);
	
	//���� ������ ����
	float getAngle(float startX, float startY, float endX, float endY);
};

enum class MONSTERKIND {
	GOLEM,
	KNIGHT,
	SUMMONER,
	GHOUL
};
