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

	bool isFrame;		// ������ �̹��� ����?

	int imgCount;		// �����ӷ����� �ʿ��� ī��Ʈ
	int frameDelay;		// ������ �����̸� �� ����
	bool flipImg;		// �����ʿ��� �������� ���?

	bool isEraseSize;	// ���� ����� �Ǹ� ��������
	float increaseSize;	// update �ѹ��� ������ ������
	float currentSize;	// ���� ������
	float endSize;		// ���� ���� ������

	bool isEraseTime;	// �ð��� ������ ��������, �������� ������ ��������
	int eraseTime;		// ���� �ڿ� ��������
	int currentTime;	// ���� �ð� (�̰� �Ȱǵ������)
	int opacity;

	int maxFrame;		// �̹����� �ִ� ������ (�̰͵� �Ȱǵ������)
	POINT pos;			// �̹����� �����ų ��ġ
};

struct tagDamageEffect
{
	int frameX;			// ������ X ��ǥ
	int frameY;			// ������ Y ��ǥ

	int maxFrame;		// �̹����� �ִ� ������ (�̰͵� �Ȱǵ������)
	POINT pos;			// �̹����� �����ų ��ġ
};

struct tagPortalEffect
{
	int frameX;
	int maxFrame;

	int opacity[8];
	POINT pos;

	bool isActive;
};

struct tagCursePoint
{
	int curTime;

	POINT pt[3];
	int renderIndex;
	int createDelay;

	bool isActive;
};

struct tagCurseEffect
{
	int curTime;

	POINT frame;
	int maxFrame;

	POINT pos;
};

class effectManager : public singletonBase <effectManager>
{
private:
	//�뽬 ����Ʈ ����
	vector<tagDashEffect> vDashEft;
	//maxFrame�� �Ǹ� erase�Ǵ� ����Ʈ ����
	vector<tagEffect> vEft;
	//������ ����Ʈ ����
	vector<tagDamageEffect> dEft;
	//��Ż ����Ʈ 
	tagPortalEffect pEft;

	//���� ����Ʈ
	tagCursePoint cursePoint;
	vector<tagCurseEffect> curseBackEft;
	vector<tagCurseEffect> curseFrontEft;

	int time;			// �����ӷ����� �ϱ� ���� �ʿ�, 'time++' �� ���ֱ� ����
public:
	HRESULT init();
	void pRender(HDC hdc);

	void render(HDC hdc);

	void portalRender(HDC hdc);

	void dRender(HDC hdc);

	void addImage();
	void dashEffect(MOVE direction, POINT pos);

	void setDash(string keyName, int frameY, bool flip, POINT pt);
	void setEffect(string keyName, POINT pt, bool flip = false, bool isEraseTime = false, int eraseTime = 0, int frameDelay = 5);

	void setEffect(string keyName, POINT pt, int minOpacity, int maxOpacity);

	void setPortalEffect(POINT pt);

	void setEffect(string keyName, POINT pt, bool isFrameImg , int frameDelay ,bool flip, float increaseSize, float startSize, float endSize);

	void damageEffect(POINT pt);
	void lightEffect(POINT pt, int maxEffect);

	void SetCursePoint(POINT _pt0, POINT _pt1, POINT _pt2);
	void cursePointActive();
	void curseRenderBack(HDC hdc);
	void curseRenderFront(HDC hdc);
};