#include "stdafx.h"
#include "enemyManager.h"



HRESULT enemyManager::init(tagTile* _tile, tagTile* _subTile, POINT _monPt)
{
	//몬스터 이미지
	IMAGEMANAGER->addFrameImage("summoner", "resource/enemy/SummonerSource.bmp", 500, 800, 5, 8);
	IMAGEMANAGER->addFrameImage("golem", "resource/enemy/Golem.bmp", 1080, 1050, 6, 5);
	IMAGEMANAGER->addFrameImage("knight", "resource/enemy/knight.bmp", 650, 1123, 6, 8);

	//스킬 이펙트 이미지
	IMAGEMANAGER->addFrameImage("smallSlash", "resource/enemyEffect/smallSlash.bmp", 600, 1200, 3, 6);
	IMAGEMANAGER->addFrameImage("knightSlashL", "resource/enemyEffect/knightSlashL.bmp", 246, 82, 3, 1);
	IMAGEMANAGER->addFrameImage("knightSlashR", "resource/enemyEffect/knightSlashR.bmp", 246, 82, 3, 1);
	IMAGEMANAGER->addFrameImage("knightSlashUp", "resource/enemyEffect/knightSlashUp.bmp", 246, 82, 3, 1);
	IMAGEMANAGER->addFrameImage("knightSlashDown", "resource/enemyEffect/knightSlashDown.bmp", 246, 82, 3, 1);
	IMAGEMANAGER->addFrameImage("stoneFly", "resource/enemyEffect/stoneFrame.bmp", 144, 72, 2, 1);
	

	tile = _tile;
	//미니언 생성
	this->setMinion(_subTile, _monPt);

	//공용총알 클래스 초기화
	_bullet = new bullet;
	_bullet->init("stoneFly", 15, 500);

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
		_vMinion[i]->setPlayerRC(playerRC);
		_vMinion[i]->commonUpdate();

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
}

void enemyManager::setMinion(tagTile* _subTile, POINT _monPt)
{
	for (int i = 0; i < MAXTILE; i++)
	{
		switch (_subTile[i].uKind)
		{
		case UNIT_KIND::KNIGHT:
		{monster* _knight = new knight;
		_knight->init(tile, {_subTile[i].pos.x - _monPt.x, _subTile[i].pos.y - _monPt.y});
		_vMinion.push_back(_knight); }
			break;

		case UNIT_KIND::MAGE:
		{monster* _summoner = new summoner;
		_summoner->init(nullptr, { _subTile[i].pos.x - _monPt.x, _subTile[i].pos.y - _monPt.y });
		_vMinion.push_back(_summoner); }
			break;
		case UNIT_KIND::GOLEM:
		{monster* _golem = new golem;
		_golem->init(tile, { _subTile[i].pos.x - _monPt.x, _subTile[i].pos.y - _monPt.y });
		_vMinion.push_back(_golem); }
			break;
		//case UNIT_KIND::GHOUL:
		//	break;
		//case UNIT_KIND::SLIMEKING:
		//	break;
		
		}
		
	}
	
	
	
}

void enemyManager::minionBulletFire(float aimX, float aimY)
{
	_viMinion = _vMinion.begin();
	for (_viMinion; _viMinion != _vMinion.end(); ++_viMinion)
	{
		if ((*_viMinion)->getFx() == false)continue;
		
		float angle = getAngle((float)(*_viMinion)->getCulPos().x, (float)(*_viMinion)->getCulPos().y, (float)aimX, (float)aimY);
		switch ((*_viMinion)->getMonsterKind())
		{
			
		case MONSTERKIND::GOLEM:
			golemBullet( angle);
			break;
		case MONSTERKIND::KNIGHT:
			knightBullet(angle);
			break;
			
		case MONSTERKIND::SUMMONER:
			angle = getAngle((float)(*_viMinion)->getCulPos().x+50, (float)(*_viMinion)->getCulPos().y-52, 
				(float)aimX, (float)aimY);
			summonerBullet(angle);
			break;
		}
		
	}
}

void enemyManager::removeMinion(int index)
{
	_vMinion.erase(_vMinion.begin() + index);
}

void enemyManager::golemBullet(float angle)
{
	switch ((*_viMinion)->getBulletDirection())
	{
	case MONSTER_UP:
		_bullet->fire((float)(*_viMinion)->getPos().x + 20, (float)(*_viMinion)->getPos().y - 72,
			angle, 0.f, (*_viMinion)->getAttack(), (*_viMinion)->getMonsterKind());
		(*_viMinion)->setFx(false);
		break;
	case MONSTER_DOWN:
		_bullet->fire((float)(*_viMinion)->getPos().x + 20, (float)(*_viMinion)->getPos().y + 72,
			angle, 0.0f, (*_viMinion)->getAttack(), (*_viMinion)->getMonsterKind());
		(*_viMinion)->setFx(false);
		break;
	case MONSTER_LEFT:
		_bullet->fire((float)(*_viMinion)->getPos().x - 20, (float)(*_viMinion)->getPos().y,
			angle, 0.0f, (*_viMinion)->getAttack(), (*_viMinion)->getMonsterKind());
		(*_viMinion)->setFx(false);
		break;
	case MONSTER_RIGHT:
		_bullet->fire((float)(*_viMinion)->getPos().x + 100, (float)(*_viMinion)->getPos().y + 50,
			angle, 0.0f, (*_viMinion)->getAttack(), (*_viMinion)->getMonsterKind());
		(*_viMinion)->setFx(false);
		break;
	}
}

void enemyManager::knightBullet(float angle)
{
	switch ((*_viMinion)->getBulletDirection())
	{
	case MONSTER_UP:
		_bullet->fire((float)(*_viMinion)->getPos().x + 20, (float)(*_viMinion)->getPos().y - 72,
			angle, 0.0f, (*_viMinion)->getAttack(), (*_viMinion)->getMonsterKind());
		(*_viMinion)->setFx(false);
		break;
	case MONSTER_DOWN:
		_bullet->fire((float)(*_viMinion)->getPos().x + 20, (float)(*_viMinion)->getPos().y + 72,
			angle, 0.0f, (*_viMinion)->getAttack(), (*_viMinion)->getMonsterKind());
		(*_viMinion)->setFx(false);
		break;
	case MONSTER_LEFT:
		_bullet->fire((float)(*_viMinion)->getPos().x - 20, (float)(*_viMinion)->getPos().y + 30,
			angle, 0.0f, (*_viMinion)->getAttack(), (*_viMinion)->getMonsterKind());
		(*_viMinion)->setFx(false);
		break;
	case MONSTER_RIGHT:
		_bullet->fire((float)(*_viMinion)->getPos().x + 100, (float)(*_viMinion)->getPos().y + 50,
			angle, 0.0f, (*_viMinion)->getAttack(), (*_viMinion)->getMonsterKind());
		(*_viMinion)->setFx(false);
		break;
	}
}

void enemyManager::summonerBullet(float angle)
{
	_bullet->fire((float)(*_viMinion)->getPos().x + 50, (float)(*_viMinion)->getPos().y - 52,
		angle, 9.0f, (*_viMinion)->getAttack(), (*_viMinion)->getMonsterKind());
	_bullet->fire((float)(*_viMinion)->getPos().x + 50, (float)(*_viMinion)->getPos().y - 52,
		angle + PI / 4, 9.0f, (*_viMinion)->getAttack(), (*_viMinion)->getMonsterKind());
	_bullet->fire((float)(*_viMinion)->getPos().x + 50, (float)(*_viMinion)->getPos().y - 52,
		angle - PI / 4, 9.0f, (*_viMinion)->getAttack(), (*_viMinion)->getMonsterKind());
	(*_viMinion)->setFx(false);
}
