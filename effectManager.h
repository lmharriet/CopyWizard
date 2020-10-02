#pragma once
#include "singletonBase.h"
#include "unitNode.h"

struct tagDashEffect
{
	string keyName;		// ����� �̹���
	int imgCount;		// �����ӷ����� �ʿ��� ī��Ʈ
	bool flipImg;		// �̹����� �ݴ���ֳ�? (�����ʿ��� �������� �׷��� �̹����� ������ ��)
	int frameY;			// ������ Y�� ��ġ
	int maxFrame;		// �ִ� X ������ (�̹����� �ִ� �������� �Ǹ� ���Ϳ��� ERASE)
	POINT pos;			// ����� ��ġ
};
class effectManager : public singletonBase <effectManager>
{
private:
	vector<tagDashEffect> vDashEft;

	int time;			// �����ӷ����� �ϱ� ���� �ʿ�, 'time++' �� ���ֱ� ����
public:
	HRESULT init();
	void release();
	void render(HDC hdc);

	void addImage();
	void dashEffect(MOVE direction, POINT pos);

	void setDash(string keyName, int frameY, bool flip, POINT pt);
};