#pragma once
#include "gameNode.h"
#include "summoner.h" //�̴Ͼ� Ŭ������ ����ϱ� ����
#include "golem.h"
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
	void minionBulletFire(float aimX, float aimY);
	//�̴Ͼ� ����
	void removeMinion(int index);

	//�̴Ͼ� ���� ��������
	vector<enemy*> getMinion() { return _vMinion; }

	//�浹ó�� (�̴Ͼ���� �Ѿ�, �÷��̾�)
	void collision(RECT player);
	
	enemyManager() {}
	~enemyManager() {}
};