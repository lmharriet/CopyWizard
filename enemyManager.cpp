#include "stdafx.h"
#include "enemyManager.h"

enemyManager::enemyManager():_attack(nullptr){}


HRESULT enemyManager::init(tagTile* _tile, tagTile* _subTile, POINT _monPt)
{
	
	
	
	tile = _tile;
	//몬스터 생성
	this->setMonster(_subTile, _monPt);

	//공용총알 클래스 초기화
	if(_attack==nullptr)
		_attack = new attack;
	_attack->init("stoneFly", 15, 500,110);

	return S_OK;
}

void enemyManager::release()
{
	//공용총알 클래스 삭제
	_attack->release();
	SAFE_DELETE(_attack);
	/*for (int i = 0; i < _vMonster.size(); i++)
	{
		SAFE_DELETE(_vMonster[i]);
	}*/
}

void enemyManager::update()
{
	//공용총알 업데이트
	
	_attack->update(); 

	//벡터에 담긴 몬스터들 업데이트
	for (int i = 0; i < _vMonster.size();  )
	{
		_vMonster[i]->setPlayerRC(playerRC);
		_vMonster[i]->commonUpdate();

		if (_vMonster[i]->getDelete())
		{
			removeMonster(i);
		}
		else
		{
			i++;
		}
		
	}
	
	//몬스터 총알발사
	this->monsterAttackFire(CAMERAMANAGER->GetRelativeX(playerRC.left), CAMERAMANAGER->GetRelativeY(playerRC.top));

	
}

void enemyManager::render()
{
	//공용총알 렌더
	_attack->render();
	int sizeRange = _vMonster.size();
	for (int i = 0; i < sizeRange; i++)
	{
		_vMonster[i]->render();
	}


}

void enemyManager::setMonster(tagTile* _subTile, POINT _monPt)
{
	int sizeRange = _vMonster.size();
	for (int i = 0; i < sizeRange; i++)
	{
		_vMonster[i]->release();
	}
		_vMonster.clear();

	for (int i = 0; i < MAXTILE; i++)
	{
		POINT pos = { _subTile[i].pos.x - _monPt.x, _subTile[i].pos.y - _monPt.y };
		switch (_subTile[i].uKind)
		{
		case UNIT_KIND::KNIGHT:
		{shared_ptr<monster> _knight = make_shared<knight>();
		_knight->init(tile, pos);
		_vMonster.push_back(_knight); }
			break;

		case UNIT_KIND::MAGE:
		{shared_ptr<monster> _summoner = make_shared <summoner>();
		_summoner->init(tile, pos);
		_vMonster.push_back(_summoner); }
			break;
		case UNIT_KIND::GOLEM:
		{shared_ptr<monster> _golem = make_shared <golem>();
		_golem->init(tile, pos);
		_vMonster.push_back(_golem); }
			break;
		case UNIT_KIND::GHOUL:
		{shared_ptr<monster> _ghoul = make_shared <ghoul>();
		_ghoul->init(tile, pos);
		_vMonster.push_back(_ghoul); }
			break;
		case UNIT_KIND::GHOULLARGE:
		{shared_ptr<monster> _ghoulLarge = make_shared <ghoulLarge>();
		_ghoulLarge->init(tile, pos);
		_vMonster.push_back(_ghoulLarge); }
			break;
		
		}
		
	}
	
	
	
}

void enemyManager::monsterAttackFire(float aimX, float aimY)
{
	_viMonster = _vMonster.begin();
	for (_viMonster; _viMonster != _vMonster.end(); ++_viMonster)
	{
		if ((*_viMonster)->getFx() == false)continue;
		
		float angle = getAngle((float)(*_viMonster)->getCulCenterX(), (float)(*_viMonster)->getCulCenterY(), (float)aimX, (float)aimY);
		switch ((*_viMonster)->getMonsterKind())
		{
		case MONSTERKIND::GOLEM:
			golemAttack( angle);
			break;
		case MONSTERKIND::KNIGHT:
			angle = getAngle((float)(*_viMonster)->getCulCenterX(), (float)(*_viMonster)->getCulCenterY()+50, (float)aimX, (float)aimY);
			knightAttack(angle);
			break;
		case MONSTERKIND::SUMMONER:
			angle = getAngle((float)(*_viMonster)->getCulPos().x+50, (float)(*_viMonster)->getCulPos().y-52, 
				(float)aimX, (float)aimY);
			summonerAttack(angle);
			SOUNDMANAGER->play("summonerFire", false,-0.3f);
			(*_viMonster)->setFx(false);
			break;
		case MONSTERKIND::GHOUL:
			angle = getAngle((float)(*_viMonster)->getCulCenterX(), (float)(*_viMonster)->getCulCenterY() + 50, (float)aimX, (float)aimY);
			knightAttack(angle);
			break;
		case MONSTERKIND::GHOULLARGE:
			ghoulLargeAttack((*_viMonster)->getAngle());
			break;
		}
		
	}
}

void enemyManager::removeMonster(int index)
{
	_vMonster.erase(_vMonster.begin() + index);
}

void enemyManager::golemAttack(float angle)
{
	switch ((*_viMonster)->getBulletDirection())
	{
	case MONSTER_UP:
		_attack->fire((float)(*_viMonster)->getPos().x + 20, (float)(*_viMonster)->getPos().y - 72,
			angle, 0.f, (*_viMonster)->getAttack(), (*_viMonster)->getMonsterKind());
		(*_viMonster)->setFx(false);
		break;
	case MONSTER_DOWN:
		_attack->fire((float)(*_viMonster)->getPos().x + 20, (float)(*_viMonster)->getPos().y + 72,
			angle, 0.0f, (*_viMonster)->getAttack(), (*_viMonster)->getMonsterKind());
		(*_viMonster)->setFx(false);
		break;
	case MONSTER_LEFT:
		_attack->fire((float)(*_viMonster)->getPos().x - 20, (float)(*_viMonster)->getPos().y,
			angle, 0.0f, (*_viMonster)->getAttack(), (*_viMonster)->getMonsterKind());
		(*_viMonster)->setFx(false);
		break;
	case MONSTER_RIGHT:
		_attack->fire((float)(*_viMonster)->getPos().x + 100, (float)(*_viMonster)->getPos().y + 50,
			angle, 0.0f, (*_viMonster)->getAttack(), (*_viMonster)->getMonsterKind());
		(*_viMonster)->setFx(false);
		break;
	}
}

void enemyManager::knightAttack(float angle)
{
	switch ((*_viMonster)->getBulletDirection())
	{
	case MONSTER_UP:
		_attack->fire((float)(*_viMonster)->getPos().x , (float)(*_viMonster)->getPos().y - 40,
			angle, 0.0f, (*_viMonster)->getAttack(), (*_viMonster)->getMonsterKind());
		(*_viMonster)->setFx(false);
		break;
	case MONSTER_DOWN:
		_attack->fire((float)(*_viMonster)->getPos().x + 20, (float)(*_viMonster)->getPos().y + 72,
			angle, 0.0f, (*_viMonster)->getAttack(), (*_viMonster)->getMonsterKind());
		(*_viMonster)->setFx(false);
		break;
	case MONSTER_LEFT:
		_attack->fire((float)(*_viMonster)->getPos().x - 20, (float)(*_viMonster)->getPos().y + 30,
			angle, 0.0f, (*_viMonster)->getAttack(), (*_viMonster)->getMonsterKind());
		(*_viMonster)->setFx(false);
		break;
	case MONSTER_RIGHT:
		_attack->fire((float)(*_viMonster)->getPos().x + 80, (float)(*_viMonster)->getPos().y + 70,
			angle, 0.0f, (*_viMonster)->getAttack(), (*_viMonster)->getMonsterKind());
		(*_viMonster)->setFx(false);
		break;
	}
}

void enemyManager::summonerAttack(float angle)
{
	_attack->fire((float)(*_viMonster)->getPos().x + 50, (float)(*_viMonster)->getPos().y - 52,
		angle, 9.0f, (*_viMonster)->getAttack(), (*_viMonster)->getMonsterKind());
	_attack->fire((float)(*_viMonster)->getPos().x + 50, (float)(*_viMonster)->getPos().y - 52,
		angle + PI / 4, 9.0f, (*_viMonster)->getAttack(), (*_viMonster)->getMonsterKind());
	_attack->fire((float)(*_viMonster)->getPos().x + 50, (float)(*_viMonster)->getPos().y - 52,
		angle - PI / 4, 9.0f, (*_viMonster)->getAttack(), (*_viMonster)->getMonsterKind());
	(*_viMonster)->setFx(false);
}

void enemyManager::ghoulLargeAttack(float angle)
{
	switch ((*_viMonster)->getBulletDirection())
	{
	case MONSTER_UP:
		_attack->fire((float)(*_viMonster)->getCenterX(), (float)(*_viMonster)->getCenterY(),
			angle, 9.f, (*_viMonster)->getAttack(), (*_viMonster)->getMonsterKind());
		(*_viMonster)->setFx(false);
		break;
	case MONSTER_DOWN:
		_attack->fire((float)(*_viMonster)->getCenterX(), (float)(*_viMonster)->getCenterY(),
			angle, 9.0f, (*_viMonster)->getAttack(), (*_viMonster)->getMonsterKind());
		(*_viMonster)->setFx(false);
		break;
	case MONSTER_LEFT:
		_attack->fire((float)(*_viMonster)->getCenterX(), (float)(*_viMonster)->getCenterY(),
			angle, 9.0f, (*_viMonster)->getAttack(), (*_viMonster)->getMonsterKind());
		(*_viMonster)->setFx(false);
		break;
	case MONSTER_RIGHT:
		_attack->fire((float)(*_viMonster)->getCenterX(), (float)(*_viMonster)->getCenterY(),
			angle, 9.0f, (*_viMonster)->getAttack(), (*_viMonster)->getMonsterKind());
		(*_viMonster)->setFx(false);
		break;
	}
}


