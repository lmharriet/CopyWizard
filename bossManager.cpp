#include "stdafx.h"
#include "bossManager.h"

HRESULT bossManager::init(float x, float y, int deleteTime, int type)
{
	bossSkill* _bossSkill = new bossSkill;
	_bossSkill->init(x, y, deleteTime, type);
	_vBossSkill.push_back(_bossSkill);

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
			_vBossSkill.erase(_vBossSkill.begin() + i);
		}
	}
}

void bossManager::render(HDC hdc)
{
	for (int i = 0; i < _vBossSkill.size(); i++) {
		_vBossSkill[i]->render(hdc);
	}
}

