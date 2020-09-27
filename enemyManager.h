#pragma once
#include "gameNode.h"
#include "minion.h" //�̴Ͼ� Ŭ������ ����ϱ� ����
#include "bullet.h" //�����Ѿ� Ŭ���� ����ϱ� ����


class enemyManager : public gameNode
{
private:
	
private:
	vector<enemy*> _vMinion;
	vector<enemy*>::iterator _viMinion;
	//vector<enemy*> _vBoss;

	bullet* _bullet; //�����Ѿ�

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//�̴Ͼ� ���� => �ʱ�ȭ�� �ִ´�
	void setMinion();
	//void setBoss();
	//�̴Ͼ� �Ѿ˹߻�
	void minionBulletFire();
	//�̴Ͼ� ����
	void removeMinion(int index);

	//�̴Ͼ� ���� ��������
	vector<enemy*> getMinion() { return _vMinion; }

	//�浹ó�� (�̴Ͼ���� �Ѿ�, �÷��̾�)
	void collision();


	enemyManager() {}
	~enemyManager() {}
};

