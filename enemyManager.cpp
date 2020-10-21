#include "stdafx.h"
#include "enemyManager.h"

enemyManager::enemyManager():_bullet(nullptr){}


HRESULT enemyManager::init(tagTile* _tile, tagTile* _subTile, POINT _monPt)
{
	
	
	
	tile = _tile;
	//미니언 생성
	this->setMinion(_subTile, _monPt);

	//공용총알 클래스 초기화
	if(_bullet==nullptr)
		_bullet = new bullet;
	_bullet->init("stoneFly", 15, 500,110);

	return S_OK;
}

void enemyManager::release()
{
	//공용총알 클래스 삭제
	_bullet->release();
	SAFE_DELETE(_bullet);
	/*for (int i = 0; i < _vMinion.size(); i++)
	{
		SAFE_DELETE(_vMinion[i]);
	}*/
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
	for (int i = 0; i < _vMinion.size(); i++)
	{
		_vMinion[i]->render();
	}


}

void enemyManager::setMinion(tagTile* _subTile, POINT _monPt)
{
	for (int i = 0; i < _vMinion.size(); i++)
	{
		_vMinion[i]->release();
	}
		_vMinion.clear();

	for (int i = 0; i < MAXTILE; i++)
	{
		POINT pos = { _subTile[i].pos.x - _monPt.x, _subTile[i].pos.y - _monPt.y };
		switch (_subTile[i].uKind)
		{
		case UNIT_KIND::KNIGHT:
		{shared_ptr<monster> _knight = make_shared<knight>();
		_knight->init(tile, pos);
		_vMinion.push_back(_knight); }
			break;

		case UNIT_KIND::MAGE:
		{shared_ptr<monster> _summoner = make_shared <summoner>();
		_summoner->init(tile, pos);
		_vMinion.push_back(_summoner); }
			break;
		case UNIT_KIND::GOLEM:
		{shared_ptr<monster> _golem = make_shared <golem>();
		_golem->init(tile, pos);
		_vMinion.push_back(_golem); }
			break;
		case UNIT_KIND::GHOUL:
		{shared_ptr<monster> _ghoul = make_shared <ghoul>();
		_ghoul->init(tile, pos);
		_vMinion.push_back(_ghoul); }
			break;
		case UNIT_KIND::GHOULLARGE:
		{shared_ptr<monster> _ghoulLarge = make_shared <ghoulLarge>();
		_ghoulLarge->init(tile, pos);
		_vMinion.push_back(_ghoulLarge); }
			break;
		
		}
		
	}
	
	
	
}

void enemyManager::minionBulletFire(float aimX, float aimY)
{
	_viMinion = _vMinion.begin();
	for (_viMinion; _viMinion != _vMinion.end(); ++_viMinion)
	{
		if ((*_viMinion)->getFx() == false)continue;
		
		float angle = getAngle((float)(*_viMinion)->getCulCenterX(), (float)(*_viMinion)->getCulCenterY(), (float)aimX, (float)aimY);
		switch ((*_viMinion)->getMonsterKind())
		{
		case MONSTERKIND::GOLEM:
			golemBullet( angle);
			break;
		case MONSTERKIND::KNIGHT:
			angle = getAngle((float)(*_viMinion)->getCulCenterX(), (float)(*_viMinion)->getCulCenterY()+50, (float)aimX, (float)aimY);
			knightBullet(angle);
			break;
		case MONSTERKIND::SUMMONER:
			angle = getAngle((float)(*_viMinion)->getCulPos().x+50, (float)(*_viMinion)->getCulPos().y-52, 
				(float)aimX, (float)aimY);
			summonerBullet(angle);
			SOUNDMANAGER->play("summonerFire", false,-0.3f);
			(*_viMinion)->setFx(false);
			break;
		case MONSTERKIND::GHOUL:
			angle = getAngle((float)(*_viMinion)->getCulCenterX(), (float)(*_viMinion)->getCulCenterY() + 50, (float)aimX, (float)aimY);
			knightBullet(angle);
			break;
		case MONSTERKIND::GHOULLARGE:
			ghoulLargeBullet((*_viMinion)->getAngle());
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
		_bullet->fire((float)(*_viMinion)->getPos().x , (float)(*_viMinion)->getPos().y - 72,
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
		_bullet->fire((float)(*_viMinion)->getPos().x + 80, (float)(*_viMinion)->getPos().y + 70,
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

void enemyManager::ghoulLargeBullet(float angle)
{
	switch ((*_viMinion)->getBulletDirection())
	{
	case MONSTER_UP:
		_bullet->fire((float)(*_viMinion)->getCenterX(), (float)(*_viMinion)->getCenterY(),
			angle, 9.f, (*_viMinion)->getAttack(), (*_viMinion)->getMonsterKind());
		(*_viMinion)->setFx(false);
		break;
	case MONSTER_DOWN:
		_bullet->fire((float)(*_viMinion)->getCenterX(), (float)(*_viMinion)->getCenterY(),
			angle, 9.0f, (*_viMinion)->getAttack(), (*_viMinion)->getMonsterKind());
		(*_viMinion)->setFx(false);
		break;
	case MONSTER_LEFT:
		_bullet->fire((float)(*_viMinion)->getCenterX(), (float)(*_viMinion)->getCenterY(),
			angle, 9.0f, (*_viMinion)->getAttack(), (*_viMinion)->getMonsterKind());
		(*_viMinion)->setFx(false);
		break;
	case MONSTER_RIGHT:
		_bullet->fire((float)(*_viMinion)->getCenterX(), (float)(*_viMinion)->getCenterY(),
			angle, 9.0f, (*_viMinion)->getAttack(), (*_viMinion)->getMonsterKind());
		(*_viMinion)->setFx(false);
		break;
	}
}


