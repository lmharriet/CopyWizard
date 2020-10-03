#include "stdafx.h"
#include "enemyManager.h"


HRESULT enemyManager::init()
{
	IMAGEMANAGER->addFrameImage("summoner", "resource/enemy/SummonerSource.bmp", 500, 700, 5, 7);
	//�̴Ͼ� ����, ����, �Ϲݸ���
	//���� �Լ��� ���� ó���ϸ� ������ ���ϴ�

	//�̴Ͼ� ����
	this->setMinion();

	//�����Ѿ� Ŭ���� �ʱ�ȭ
	_bullet = new bullet;
	_bullet->init("bullet", 15, 2000);

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
	_bullet->update();

	//���Ϳ� ��� �̴Ͼ�� ������Ʈ
	for (int i = 0; i < _vMinion.size(); i++)
	{
		_vMinion[i]->update();
	}

	//�̴Ͼ� �Ѿ˹߻�
	//this->minionBulletFire();
}

void enemyManager::render()
{
	//�����Ѿ� ����
	_bullet->render();

	//���Ϳ� ��� �̴Ͼ�� ����
	for (int i = 0; i < _vMinion.size(); i++)
	{
		_vMinion[i]->render();
	}
}

void enemyManager::setMinion()
{
	

	enemy* enem = new summoner;
	enem->init("summoner", { 500,500 });
	_vMinion.push_back(enem);
}

void enemyManager::minionBulletFire(float aimX, float aimY)
{
	_viMinion = _vMinion.begin();
	for (_viMinion; _viMinion != _vMinion.end(); ++_viMinion)
	{
		if ((*_viMinion)->checkAttack() == false)continue;

		float angle = getAngle((*_viMinion)->getX(), (*_viMinion)->getY(), aimX, aimY);

		_bullet->fire((*_viMinion)->getX(), (*_viMinion)->getY(), angle, 3.0f);
		(*_viMinion)->setAttack(false);
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