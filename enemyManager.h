#pragma once
#include "gameNode.h"
#include "summoner.h" //�̴Ͼ� Ŭ������ ����ϱ� ����
#include "golem.h"
#include "bullet.h" //�����Ѿ� Ŭ���� ����ϱ� ����
#include "knight.h"
#include "ghoul.h"
#include "ghoulLarge.h"

class enemyManager : public gameNode
{
	
private:
	vector<shared_ptr<monster>> _vMonster;
	vector<shared_ptr<monster>>::iterator _viMonster;
	tagTile* tile;
	RECT playerRC;
	//vector<enemy*> _vBoss;
	

	attack* _attack; //�����Ѿ�
public:
	HRESULT init(tagTile* _tile, tagTile* _subTile, POINT _monPt);
	void release();
	void update();
	void render();



	//�̴Ͼ� ���� => �ʱ�ȭ�� �ִ´�
	void setMonster(tagTile* _subTile, POINT _monPt);
	//void setBoss();
	//�̴Ͼ� �Ѿ˹߻�
	void monsterAttackFire(float aimX, float aimY);
	//�̴Ͼ� ����
	void removeMonster(int index);

	//�÷��̾� ��Ʈ ��������
	void setPlayerRC(RECT _playerRC) { playerRC = _playerRC; }
	//�̴Ͼ� ���� ��������
	vector<shared_ptr<monster>> getMonster() { return _vMonster; }

	//�浹ó�� (�̴Ͼ���� �Ѿ�, �÷��̾�)
	//void collision(RECT player);

	//attack 
	attack* getAttack() { return _attack; }
	void golemAttack(float angle);
	void knightAttack(float angle);
	void summonerAttack(float angle);
	void ghoulLargeAttack(float angle);

	
	enemyManager();
	~enemyManager() {}
};