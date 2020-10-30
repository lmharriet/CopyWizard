#pragma once
#include "gameNode.h"
#include "summoner.h" //미니언 클래스를 사용하기 위해
#include "golem.h"
#include "bullet.h" //공용총알 클래스 사용하기 위해
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
	

	attack* _attack; //공용총알
public:
	HRESULT init(tagTile* _tile, tagTile* _subTile, POINT _monPt);
	void release();
	void update();
	void render();



	//미니언 세팅 => 초기화에 넣는다
	void setMonster(tagTile* _subTile, POINT _monPt);
	//void setBoss();
	//미니언 총알발사
	void monsterAttackFire(float aimX, float aimY);
	//미니언 삭제
	void removeMonster(int index);

	//플레이어 렉트 가져오기
	void setPlayerRC(RECT _playerRC) { playerRC = _playerRC; }
	//미니언 벡터 가져오기
	vector<shared_ptr<monster>> getMonster() { return _vMonster; }

	//충돌처리 (미니언들의 총알, 플레이어)
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