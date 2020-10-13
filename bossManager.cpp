#include "stdafx.h"
#include "bossManager.h"

HRESULT bossManager::init(float x, float y, int deleteTime, int type)
{
	bossSkill* _bossSkill = new bossSkill;
	_bossSkill->init(x, y, deleteTime, type);
	_vBossSkill.push_back(_bossSkill);

	_type = type;

	return S_OK;
}

void bossManager::release()
{
}

void bossManager::update()
{
	for (int i = 0; i < _vBossSkill.size(); i++) {
		_vBossSkill[i]->update();
		if (_vBossSkill[i]->getCount() == _vBossSkill[i]->getDeleteTime()) {
			if (_type == 0) {
				PARTICLE->crashRockParticlePlay(_vBossSkill[i]->getRect().left + 24, _vBossSkill[i]->getRect().bottom - 95);
			}
			_vBossSkill.erase(_vBossSkill.begin() + i);
		}
	}
}

void bossManager::render(HDC hdc)
{
	this->sorting();
	for (int i = 0; i < _vBossSkill.size(); i++) {
		_vBossSkill[i]->render(hdc);
	}
}

bool compare(bossSkill* i, bossSkill* j) {
	return (*i).getRect().top < (*j).getRect().top;
}

void bossManager::sorting()
{
	sort(_vBossSkill.begin(), _vBossSkill.end(), compare);
}

