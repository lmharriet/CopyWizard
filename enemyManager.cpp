#include "stdafx.h"
#include "enemyManager.h"


HRESULT enemyManager::init(tagTile* _tile)
{
	IMAGEMANAGER->addFrameImage("summoner", "resource/enemy/SummonerSource.bmp", 500, 700, 5, 7);
	IMAGEMANAGER->addFrameImage("golem", "resource/enemy/Golem.bmp", 720, 700, 6, 5);
	IMAGEMANAGER->addFrameImage("knight", "resource/enemy/knight.bmp", 650, 1123, 6, 8);

	IMAGEMANAGER->addFrameImage("smallSlash", "resource/enemyEffect/smallSlash.bmp", 300, 600, 3, 6);
	IMAGEMANAGER->addFrameImage("knightSlashL", "resource/enemyEffect/knightSlashL.bmp", 246, 82, 3, 1);
	IMAGEMANAGER->addFrameImage("knightSlashR", "resource/enemyEffect/knightSlashR.bmp", 246, 82, 3, 1);
	IMAGEMANAGER->addFrameImage("knightSlashUp", "resource/enemyEffect/knightSlashUp.bmp", 246, 82, 3, 1);
	IMAGEMANAGER->addFrameImage("knightSlashDown", "resource/enemyEffect/knightSlashDown.bmp", 246, 82, 3, 1);
	IMAGEMANAGER->addFrameImage("stoneFly", "resource/enemyEffect/stoneFrame.bmp", 144, 72, 2, 1);
	//미니언 생성, 보스, 일반몬스터
	//따로 함수로 빼서 처리하면 관리가 편하다

	tile = _tile;
	//미니언 생성
	this->setMinion();

	//공용총알 클래스 초기화
	_bullet = new bullet;
	_bullet->init("stoneFly", 15, 700);

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
	_viMinion = _vMinion.begin();
	for (_viMinion; _viMinion != _vMinion.end(); ++_viMinion)
	{

		if ((*_viMinion)->getMonsterKind() != MONSTERKIND::SUMMONER) continue;
		if ((*_viMinion)->getBulletFire() == false)continue;

		_bullet->update();
	}

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
	this->minionBulletFire(CAMERAMANAGER->GetRelativeX(playerRC.left), CAMERAMANAGER->GetRelativeY(playerRC.top));

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
	monster* _golem = new golem;
	_golem->init(tile, "golem", { 820,320 }, 3.f,MONSTERKIND::GOLEM,50,"smallSlash",false);
	_vMinion.push_back(_golem);
	
	monster* _golem1 = new golem;
	_golem1->init(tile, "golem", { 220,320 }, 3.f, MONSTERKIND::GOLEM,50, "smallSlash",false);
	_vMinion.push_back(_golem1);
	
	monster* _summoner = new summoner;
	_summoner->init(nullptr,"summoner", { 500,500 },0, MONSTERKIND::SUMMONER,10,0,true);
	_vMinion.push_back(_summoner);

	monster* _knight = new knight;
	_knight->init(tile, "knight", { 320, 550 }, 4.f, MONSTERKIND::KNIGHT,30,"smallSlash");
	_vMinion.push_back(_knight);
	
}

void enemyManager::minionBulletFire(float aimX, float aimY)
{
	_viMinion = _vMinion.begin();
	for (_viMinion; _viMinion != _vMinion.end(); ++_viMinion)
	{
		if ((*_viMinion)->getMonsterKind() != MONSTERKIND::SUMMONER) continue;
		if ((*_viMinion)->getFx() == false)continue;

		
		float angle = getAngle((float)(*_viMinion)->getCulPos().x,(float) (*_viMinion)->getCulPos().y,(float) aimX, (float)aimY);

		_bullet->fire((float)(*_viMinion)->getPos().x, (float)(*_viMinion)->getPos().y-72, angle, 9.0f);
		_bullet->fire((float)(*_viMinion)->getPos().x, (float)(*_viMinion)->getPos().y-72, angle+PI/4, 9.0f);
		_bullet->fire((float)(*_viMinion)->getPos().x, (float)(*_viMinion)->getPos().y-72, angle-PI/4, 9.0f);
		(*_viMinion)->setFx(false);
	}
}

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