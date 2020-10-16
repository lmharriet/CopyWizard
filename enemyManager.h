#pragma once
#include "gameNode.h"
#include "summoner.h" //�̴Ͼ� Ŭ������ ����ϱ� ����
#include "golem.h"
#include "bullet.h" //�����Ѿ� Ŭ���� ����ϱ� ����
#include "knight.h"
#include "ghoul.h"

class enemyManager : public gameNode
{
	
private:
	vector<monster*> _vMinion;
	vector<monster*>::iterator _viMinion;
	tagTile* tile;
	RECT playerRC;
	//vector<enemy*> _vBoss;
	

	bullet* _bullet; //�����Ѿ�
public:
	HRESULT init(tagTile* _tile, tagTile* _subTile, POINT _monPt);
	void release();
	void update();
	void render();



	//�̴Ͼ� ���� => �ʱ�ȭ�� �ִ´�
	void setMinion(tagTile* _subTile, POINT _monPt);
	//void setBoss();
	//�̴Ͼ� �Ѿ˹߻�
	void minionBulletFire(float aimX, float aimY);
	//�̴Ͼ� ����
	void removeMinion(int index);

	//�÷��̾� ��Ʈ ��������
	void setPlayerRC(RECT _playerRC) { playerRC = _playerRC; }
	//�̴Ͼ� ���� ��������
	vector<monster*> getMinion() { return _vMinion; }

	//�浹ó�� (�̴Ͼ���� �Ѿ�, �÷��̾�)
	void collision(RECT player);

	//bullet 
	bullet* getBullet() { return _bullet; }
	void golemBullet(float angle);
	void knightBullet(float angle);
	void summonerBullet(float angle);

	
	enemyManager() {}
	~enemyManager() {}
};