#include "stdafx.h"
#include "player.h"
#include "enemyManager.h"

HRESULT player::init()
{
	//플레이어 이미지 초기화
	_rocket = IMAGEMANAGER->addImage("rocket", "Images/rocket.bmp", WINSIZEX / 2, WINSIZEY - 100, 52, 64, true, RGB(255, 0, 255));

	//체력바 앞뒤이미지 초기화
	IMAGEMANAGER->addImage("progressBarFront", "progressBarFront.bmp", 50, 10);
	IMAGEMANAGER->addImage("progressBarBack", "progressBarBack.bmp", 50, 10);


	//미사일 클래스 초기화
	_missile = new missile;
	_missile->init(10, 500);

	//폭탄 클래스 초기화
	_bomb = new bomb;
	_bomb->init(5, 600);

	//체력바 클래스 초기화
	_hpBar = new progressBar;
	_hpBar->init("progressBarFront", "progressBarBack");
	//체력 초기화
	_maxHp = _currentHp = 100;

	return S_OK;
}

void player::release()
{
	//미사일 클래스 해제
	_missile->release();
	SAFE_DELETE(_missile);
	//폭탄 클래스 해제
	SAFE_DELETE(_bomb);
	//체력바 클래스 해제
	SAFE_DELETE(_hpBar);
}

void player::update()
{
	//플레이어 움직이기 & 화면밖으로 나가지 못하게 만들기
	if (INPUT->GetKey(VK_LEFT) && _rocket->getX() > 0)
	{
		_rocket->setX(_rocket->getX() - 5);
	}
	if (INPUT->GetKey(VK_RIGHT) && _rocket->getX() + _rocket->getWidth() < WINSIZEX)
	{
		_rocket->setX(_rocket->getX() + 5);
	}
	//총알발사
	if (INPUT->GetKeyDown(VK_SPACE))
	{
		_missile->fire(_rocket->getX() + _rocket->getWidth() / 2, _rocket->getY() - 50);
	}
	//폭탄발사
	if (INPUT->GetKeyDown('B'))
	{
		_bomb->fire(_rocket->getX() + _rocket->getWidth() / 2, _rocket->getY() - 50);
	}

	//미사일 클래스 업데이트
	_missile->update();
	//폭탄 클래스 업데이트
	_bomb->update();

	//충돌처리
	this->collision();

	//체력바 업데이트
	_hpBar->update();
	//체력바 위치 피통도 업데이트 해줘야 한다
	_hpBar->setPos(_rocket->getX(), _rocket->getY() - 20);
	_hpBar->setGauge(_maxHp, _currentHp);

	//플레이어 데이터로 세이브 로드
	if (INPUT->GetKeyDown(VK_LBUTTON))
	{
		this->save();
	}
	if (INPUT->GetKeyDown(VK_RBUTTON))
	{
		this->load();
	}
}

void player::render()
{
	//플레이어 렌더
	_rocket->render(getMemDC(), _rocket->getX(), _rocket->getY());

	//미사일클래스 렌더
	_missile->render();

	//폭탄클래스 렌더
	_bomb->render();

	//체력바 클래스 렌더
	_hpBar->render();

}

//폭탄삭제
void player::removeBomb(int index)
{
	//폭탄클래스한테 이양시키기
	_bomb->removeBomb(index);
}
//충돌처리 (플레이어 총알, 미니언 충돌)
void player::collision()
{
	//for (int i = 0; i < _player->getBomb()->getBullet().size(); i++) //플레이어 폭탄
	for (int i = 0; i < _bomb->getBullet().size(); i++)
	{
		//for (int j = 0; j < _enemyManager->getMinion().size(); j++) //미니언
		for (int j = 0; j < _em->getMinion().size(); j++)
		{
			//if (IntersectRect(&rc, &_player->getBomb()->getBullet()[i].rc,
			//	&_enemyManager->getMinion()[j]->getRect()))
			RECT rc;
			if (IntersectRect(&rc, &_bomb->getBullet()[i].rc,
				&_em->getMinion()[j]->getRect()))
			{
				_bomb->removeBomb(i);
				_em->removeMinion(j);
				break;
			}
		}
	}
}

void player::hitDamage(float damage)
{
	_currentHp -= damage;
}

//데이터 세이브
void player::save()
{
	PLAYERDATA->setHp(_currentHp);
	PLAYERDATA->setX(_x);
	PLAYERDATA->setY(_y);
}

//데이터 로드
void player::load()
{
	_currentHp = PLAYERDATA->getHp();
	_x = PLAYERDATA->getX();
	_y = PLAYERDATA->getY();
}

