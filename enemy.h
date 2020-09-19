#pragma once
#include "gameNode.h"

//�θ�Ŭ���� => �̳��� ��ӹ޾Ƽ� ����, �Ϲݸ��͸� �����
class enemy : public gameNode
{
private:
	image* _image;		//���ʹ� �̹���
	RECT _rc;			//�浹�� ��Ʈ

	//������ �̹����� ������ ���� ������
	int _count;
	int _currentFrameX;
	int _currentFrameY;

	//�������� �Ѿ˹߻� ��Ÿ�� �ֱ�
	int _fireCount;
	int _rndFireCount;

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

	enemy() {}
	~enemy() {}
};

