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

struct tagEffect
{
	string keyName;		// ����� �̹���
	int imgCount;		// �����ӷ����� �ʿ��� ī��Ʈ
	bool flipImg;		// �����ʿ��� �������� ���?

	bool isEraseTime;	// �ð��� ������ ��������, �������� ������ ��������
	int eraseTime;		// ���� �ڿ� ��������
	int currentTime;	// ���� �ð� (�̰� �Ȱǵ������)

	int maxFrame;		// �̹����� �ִ� ������ (�̰͵� �Ȱǵ������)
	POINT pos;			// �̹����� �����ų ��ġ
};
class effectManager : public singletonBase <effectManager>
{
private:
	vector<tagDashEffect> vDashEft;
	
	//maxFrame�� �Ǹ� erase�Ǵ� ����Ʈ ����
	vector<tagEffect> vEft;

	int time;			// �����ӷ����� �ϱ� ���� �ʿ�, 'time++' �� ���ֱ� ����
public:
	HRESULT init();
	void pRender(HDC hdc);

	void render(HDC hdc);

	void addImage();
	void dashEffect(MOVE direction, POINT pos);

	void setDash(string keyName, int frameY, bool flip, POINT pt);
	void setEffect(string keyName, POINT pt, bool flip = false, bool isEraseTime = false, int eraseTime = 0);
};