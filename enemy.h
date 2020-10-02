#pragma once
#include "gameNode.h"

//�θ�Ŭ���� => �̳��� ��ӹ޾Ƽ� ����, �Ϲݸ��͸� �����
class enemy : public gameNode
{
protected:
	image* _image;		//���ʹ� �̹���
	float posX, posY;	//��ġ
	RECT _rc;			//�浹�� ��Ʈ

	RECT area;			//������ ��Ʈ
	bool isDetect, isAttack;

	//������ �̹����� ������ ���� ������
	int _count;
	int _currentFrameX;
	int _currentFrameY;

	//�������� �Ѿ˹߻� ��Ÿ�� �ֱ�
	int _fireCount;
	int _rndFireCount;


	int time;
public:
	HRESULT init(const char* imageName, POINT position);
	void release();
	void update();
	void render();

	virtual void move();			//������Ʈ
	virtual void draw();			//����
	virtual void animation();		//������Ʈ

	bool bulletCountFire();

	//�浹�� ��Ʈ ��������
	RECT getRect() { return _rc; }
	RECT getArea() { return area; }
	float getX() { return posX; }
	float getY() { return posY; }

	bool getFind() { return isDetect; }
	void setFind(bool tmp) { isDetect = tmp; }

	bool checkAttack() { return isAttack; }
	bool setAttack(bool tmp) { return isAttack = tmp; }

	enemy() {}
	~enemy() {}
};