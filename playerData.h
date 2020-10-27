#pragma once
#include "singletonBase.h"
#include "tileNode.h"
#include "unitNode.h"

struct tagStat
{
	float damage;			//데미지
	float criDamage;		//크리티컬 데미지
	float criChance;		//크리티컬 확률

	//normal item
	float potionDropChance;	//포션 드랍율
	float defenceChance;	//공격 방어율
	float burnChance;		//불태울 확률

	bool goldPig;			//추가 코인 획득
	int CoolTimeReduction;	//스킬 쿨타임 감소
	bool glassMirror;		//유리 장막 유무

	//curse item
	bool vampireBlood;		//흡혈기능

	bool doubleDamage;		//2배의 피해
	bool ManaRejection;		//몬스터를 공격했을 때 마나 획득 유무
	bool criRejection;		//치명타 발생 유무
	bool reducedVisibility;	//시야 감소
	bool halfFace;			//체력이 50% 이하일 때 공격력 증가 유무
	bool bloodGold;			//보유 코인에 따라 공격력 증가 유무
};
struct tagGlass
{
	bool isActives;
	int currentTime;
	int endTime;

	float curSize;
	float endSize;
};
class playerData : public singletonBase <playerData>
{
private:
	//stat//
	int hp;
	int maxHp;
	float skillGauge;
	bool upgradeReady;

	tagStat stat;
	////////
	int coin;
	float _x, _y;

	bool isGauging;
	int _gaugeTime;

	tagGlass gShroud;

	vector<tagItem> vInven;
	vector<tagWizardArcana> vArcanaInfo;
	/// //////////
	tagTile* tile;
	vector<int> vTile;

	tagTile* wallTile;
	vector<int> vWall;

	//
	STAGEKIND currentStage;
public:
	HRESULT init();
	void release();
	void update();

	void shroudRender(HDC hdc);
	void setShroud(bool check) { gShroud.isActives = check; }
	bool getShroud() { return gShroud.isActives; }

	int getMaxHp() { return maxHp; }
	int getHp() { return hp; }
	int getCoin() { return coin; }
	float getX() { return _x; }
	float getY() { return _y; }

	//skill gauge용
	float getSkillGauge() { return skillGauge; }
	bool getUpgradeReady() { return upgradeReady; }
	//inferno용
	int getGaugeTime() { return _gaugeTime; }
	bool getGauging() { return isGauging; }


	void setHp(int _hp) { hp = _hp; }
	void setCoin(int _coin) { coin = _coin; }
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }

	//skill gauge용
	void setSkillGauge(float gauge) { skillGauge = gauge; }
	void setUpgradeReady(bool ready) { upgradeReady = ready; }
	//inferno용
	void setGauging(bool temp) { isGauging = temp; }
	void setGaugeTime(int time) { _gaugeTime = time; }


	tagStat getStat() { return stat; }
	void setStat(string itemName);

	int damageCul(int damage, bool isCritical);

	//inven item
	void pushInven(tagItem item) { vInven.push_back(item); }
	vector<tagItem> getInven() { return vInven; }
	void resetInven() { vInven.clear(); }
	//inven arcana
	void pushArcanaInfo(tagWizardArcana info) { vArcanaInfo.push_back(info); }
	vector<tagWizardArcana> getAracaInfo() { return vArcanaInfo; }
	void setArcanaInfo(int index, string keyName, string info, int CoolTime);
	void clearArcanaInfo() { vArcanaInfo.clear(); }

	bool criAppear();


	void setTile(vector<int> tile) { vTile = tile; }
	vector<int> getTile() { return vTile; }

	void setWall(vector<int> wall) { vWall = wall; }
	vector<int> getWall() { return vWall; }

	void setWallTile(tagTile* _wallTile) { wallTile = _wallTile; }
	tagTile* getWallTile() { return wallTile; }

	void setTile(tagTile* _tile) { tile = _tile; }
	tagTile* _getTile() { return tile; }

	//currentStage
	void setCurrentStage(STAGEKIND _currentStage) { currentStage = _currentStage; }
	STAGEKIND getCurrentStage() { return currentStage; }
	playerData() {}
	~playerData() {}
};