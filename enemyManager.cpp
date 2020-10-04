#include "stdafx.h"
#include "enemyManager.h"


HRESULT enemyManager::init(tagTile* _tile)
{
	IMAGEMANAGER->addFrameImage("summoner", "resource/enemy/SummonerSource.bmp", 500, 700, 5, 7);
	IMAGEMANAGER->addFrameImage("golem", "resource/enemy/Golem.bmp", 720, 700, 6, 5);
	IMAGEMANAGER->addFrameImage("knight", "resource/enemy/knight.bmp", 650, 1123, 6, 8);

	IMAGEMANAGER->addFrameImage("smallSlash", "resource/enemyEffect/smallSlash.bmp", 246, 492, 3, 6);
	//미니언 생성, 보스, 일반몬스터
	//따로 함수로 빼서 처리하면 관리가 편하다

	tile = _tile;
	//미니언 생성
	this->setMinion();

	//공용총알 클래스 초기화
	_bullet = new bullet;
	_bullet->init("bullet", 15, 2000);

	return S_OK;
}

void enemyManager::release()
{
	//공용총알 클래스 삭제
	_bullet->release();
	SAFE_DELETE(_bullet);
}

void enemyManager::update()
{
	//공용총알 업데이트
	_bullet->update();

	//벡터에 담긴 미니언들 업데이트
	for (int i = 0; i < _vMinion.size();  )
	{
		_vMinion[i]->update();

		if (_vMinion[i]->getDelete())
		{
			removeMinion(i);
		}
		else
		{
			i++;
		}
		
	}

	
	
	
	
	//미니언 총알발사
	//this->minionBulletFire();
}

void enemyManager::render()
{
	//공용총알 렌더
	_bullet->render();

	//벡터에 담긴 미니언들 렌더
	for (int i = 0; i < _vMinion.size(); i++)
	{
		_vMinion[i]->setPlayerRC(playerRC);
		_vMinion[i]->render();
		
	}

	
}

void enemyManager::setMinion()
{
	monster* _golem1 = new golem;
	_golem1->init(tile, "golem", { 820,320 }, 3.f,50,"smallSlash",false);
	_vMinion.push_back(_golem1);
	
	monster* _golem2 = new golem;
	_golem2->init(tile, "golem", { 220,320 }, 3.f,50, "smallSlash",false); 
	_vMinion.push_back(_golem2);
	
	monster* enem = new summoner;
	enem->init(nullptr,"summoner", { 500,500 },0,10,0);
	_vMinion.push_back(enem);

	monster* _knight = new knight;
	_knight->init(tile, "knight", { 320, 550 }, 4.f,30,"smallSlash");
	_vMinion.push_back(_knight);
	
}

//void enemyManager::minionBulletFire(float aimX, float aimY)
//{
//	_viMinion = _vMinion.begin();
//	for (_viMinion; _viMinion != _vMinion.end(); ++_viMinion)
//	{
//		if ((*_viMinion)->checkAttack() == false)continue;
//
//		float angle = getAngle((*_viMinion)->getX(), (*_viMinion)->getY(), aimX, aimY);
//
//		_bullet->fire((*_viMinion)->getX(), (*_viMinion)->getY(), angle, 3.0f);
//		(*_viMinion)->setAttack(false);
//	}
//}

void enemyManager::removeMinion(int index)
{
	_vMinion.erase(_vMinion.begin() + index);
}

void enemyManager::collision(RECT player)
{
	for (int i = 0; i < _bullet->getBullet().size(); i++)
	{
		if (colCheck(_bullet->getRect(i), player))
		{
			_bullet->removeBullet(i);
		}
	}
}