#pragma once
#include "gameNode.h"
#include "minion.h" //미니언 클래스를 사용하기 위해
#include "bullet.h" //공용총알 클래스 사용하기 위해

class enemyManager : public gameNode
{
private:
	
private:
	vector<enemy*> _vMinion;
	vector<enemy*>::iterator _viMinion;
	//vector<enemy*> _vBoss;

	bullet* _bullet; //공용총알
public:
	HRESULT init();
	void release();
	void update();
	void render();

	//미니언 세팅 => 초기화에 넣는다
	void setMinion();
	//void setBoss();
	//미니언 총알발사
	void minionBulletFire(float aimX, float aimY);
	//미니언 삭제
	void removeMinion(int index);

	//미니언 벡터 가져오기
	vector<enemy*> getMinion() { return _vMinion; }

	//충돌처리 (미니언들의 총알, 플레이어)
	void collision(RECT player);
	
	enemyManager() {}
	~enemyManager() {}
};