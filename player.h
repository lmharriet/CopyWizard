#pragma once
#include "gameNode.h"
#include "bullet.h"
#include "progressBar.h"
//양쪽 클래스에 서로 헤더를 걸어주면 뻥난다
//따라서 클래스 전방선언을 해줘야 한다
//#include "enemyManager.h" 

//상호참조영 클래스 전방선언
class enemyManager;

class player : public gameNode
{

private:
	enemyManager* _em;			//절대로 동적할당 하지 말아라!!

private:
	image* _rocket;					//로켓(플레이어) 이미지
	missile* _missile;				//미사일 클래스
	bomb* _bomb;					//폭탄 클래스

	progressBar* _hpBar;			//체력바
	float _currentHp, _maxHp;		//플레이어 현재체력, 최대체력
	float _x, _y;					//플레이어 위치좌표

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//폭탄 클래스 가져오기
	bomb* getBomb() { return _bomb; }

	//폭탄삭제
	void removeBomb(int index);

	//충돌처리 (플레이어 총알, 미니언 충돌)
	void collision();

	//피통깍기
	void hitDamage(float damage);

	//상호참조시 적매니져 포인터를 넘겨받을 셋터함수
	void setEMLink(enemyManager* em) { _em = em; }

	//플레이어 이미지 가져오기
	image* getPlayerImg() { return _rocket; }

	//데이터 세이브
	void save();
	//데이터 로드
	void load();

	player() {}
	~player() {}
};

