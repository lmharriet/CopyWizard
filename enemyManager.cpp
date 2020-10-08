#include "stdafx.h"
#include "enemyManager.h"


HRESULT enemyManager::init(tagTile* _tile)
{
	IMAGEMANAGER->addFrameImage("summoner", "resource/enemy/SummonerSource.bmp", 500, 800, 5, 8);
	IMAGEMANAGER->addFrameImage("golem", "resource/enemy/Golem.bmp", 720, 700, 6, 5);
	
	IMAGEMANAGER->addFrameImage("knight", "resource/enemy/knight.bmp", 650, 1123, 6, 8);

	IMAGEMANAGER->addFrameImage("smallSlash", "resource/enemyEffect/smallSlash.bmp", 300, 600, 3, 6);
	IMAGEMANAGER->addFrameImage("knightSlashL", "resource/enemyEffect/knightSlashL.bmp", 246, 82, 3, 1);
	IMAGEMANAGER->addFrameImage("knightSlashR", "resource/enemyEffect/knightSlashR.bmp", 246, 82, 3, 1);
	IMAGEMANAGER->addFrameImage("knightSlashUp", "resource/enemyEffect/knightSlashUp.bmp", 246, 82, 3, 1);
	IMAGEMANAGER->addFrameImage("knightSlashDown", "resource/enemyEffect/knightSlashDown.bmp", 246, 82, 3, 1);
	IMAGEMANAGER->addFrameImage("stoneFly", "resource/enemyEffect/stoneFrame.bmp", 144, 72, 2, 1);
	//�̴Ͼ� ����, ����, �Ϲݸ���
	//���� �Լ��� ���� ó���ϸ� ������ ���ϴ�

	tile = _tile;
	//�̴Ͼ� ����
	this->setMinion();

	//�����Ѿ� Ŭ���� �ʱ�ȭ
	_bullet = new bullet;
	_bullet->init("stoneFly", 15, 500);

	return S_OK;
}

void enemyManager::release()
{
	//�����Ѿ� Ŭ���� ����
	_bullet->release();
	SAFE_DELETE(_bullet);
}

void enemyManager::update()
{
	//�����Ѿ� ������Ʈ
	/*int num;
	_viMinion = _vMinion.begin();
	for (_viMinion; _viMinion != _vMinion.end(); ++_viMinion)
	{
		if ((*_viMinion)->getMonsterKind() != MONSTERKIND::SUMMONER) continue;
		if ((*_viMinion)->getBulletFire() == false)continue;

		
	}*/
	_bullet->update(); //�ӹ����� �ϴ°� ���߿� �ϱ�..

	//���Ϳ� ��� �̴Ͼ�� ������Ʈ
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
	
	//�̴Ͼ� �Ѿ˹߻�
	this->minionBulletFire(CAMERAMANAGER->GetRelativeX(playerRC.left)-20, CAMERAMANAGER->GetRelativeY(playerRC.top));

	
}

void enemyManager::render()
{
	//�����Ѿ� ����
	_bullet->render();
}

void enemyManager::setMinion()
{
	monster* _golem = new golem;
	_golem->init(tile, "golem", { 820,320 }, 3.f,MONSTERKIND::GOLEM,50,"smallSlash",false);
	_vMinion.push_back(_golem);
	
	monster* _golem1 = new golem;
	_golem1->init(tile, "golem", { 220,320 }, 3.f, MONSTERKIND::GOLEM,50, "smallSlash",false);
	_vMinion.push_back(_golem1);
	
	monster* _knight = new knight;
	_knight->init(tile, "knight", { 320, 350 }, 6.f, MONSTERKIND::KNIGHT, 80, NULL);
	_vMinion.push_back(_knight);

	monster* _summoner = new summoner;
	_summoner->init(nullptr,"summoner", { 500,300 },0, MONSTERKIND::SUMMONER,50,0,true,true);
	_vMinion.push_back(_summoner);

	monster* _summoner1 = new summoner;
	_summoner1->init(nullptr, "summoner", { 800,300 }, 0, MONSTERKIND::SUMMONER, 50, 0, true, true);
	_vMinion.push_back(_summoner1);

	
	
}

void enemyManager::minionBulletFire(float aimX, float aimY)
{
	_viMinion = _vMinion.begin();
	for (_viMinion; _viMinion != _vMinion.end(); ++_viMinion)
	{
		//if ((*_viMinion)->getMonsterKind() != MONSTERKIND::SUMMONER) continue;
		if ((*_viMinion)->getFx() == false)continue;
		//if (!_bullet->bulletEmpty()) continue;
		
		float angle = getAngle((float)(*_viMinion)->getCulPos().x, (float)(*_viMinion)->getCulPos().y, (float)aimX, (float)aimY);
			//int direction = (*_viMinion)->getBulletDirection();
		switch ((*_viMinion)->getMonsterKind())
		{
			
		case MONSTERKIND::GOLEM:
			switch ((*_viMinion)->getBulletDirection())
			{
			case MONSTER_UP:
				cout << "up" << endl;
				break;
			case MONSTER_DOWN:
				cout << "down" << endl;
				break;
			case MONSTER_LEFT:
				cout << "left" << endl;
				break;
			case MONSTER_RIGHT:
				cout << "right" << endl;
				break;
			
			}
			//_bullet->fire((float)(*_viMinion)->getPos().x + 20, (float)(*_viMinion)->getPos().y - 72, angle, 0.0f, 50);
			break;
		case MONSTERKIND::KNIGHT:
			//_bullet->fire((float)(*_viMinion)->getPos().x + 20, (float)(*_viMinion)->getPos().y - 72, angle, 0.0f, 50);

			break;
		case MONSTERKIND::SUMMONER:
			_bullet->fire((float)(*_viMinion)->getPos().x + 20, (float)(*_viMinion)->getPos().y - 72, angle, 9.0f,50);
			_bullet->fire((float)(*_viMinion)->getPos().x + 20, (float)(*_viMinion)->getPos().y - 72, angle + PI / 4, 9.0f,50);
			_bullet->fire((float)(*_viMinion)->getPos().x + 20, (float)(*_viMinion)->getPos().y - 72, angle - PI / 4, 9.0f,50);
			(*_viMinion)->setFx(false);
			break;
		}
		
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