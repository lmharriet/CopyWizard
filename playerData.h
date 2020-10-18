#pragma once
#include "singletonBase.h"
#include "tileNode.h"

struct tagStat
{
	float damage;			//������
	float criDamage;		//ũ��Ƽ�� ������
	float criChance;		//ũ��Ƽ�� Ȯ��

	//normal item
	float potionDropChance;	//���� �����
	float defenceChance;	//���� �����
	float burnChance;		//���¿� Ȯ��

	bool goldPig;			//�߰� ���� ȹ��
	int CoolTimeReduction;	//��ų ��Ÿ�� ����
	bool glassMirror;		//���� �帷 ����

	//curse item
	bool vampireBlood;		//�������

	bool doubleDamage;		//2���� ����
	bool ManaRejection;		//���͸� �������� �� ���� ȹ�� ����
	bool criRejection;		//ġ��Ÿ �߻� ����
	bool reducedVisibility;	//�þ� ����
	bool halfFace;			//ü���� 50% ������ �� ���ݷ� ���� ����
	bool bloodGold;			//���� ���ο� ���� ���ݷ� ���� ����
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

	/// //////////
	tagTile* tile;
	vector<int> vTile;

	tagTile* wallTile;
	vector<int> vWall;
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

	//skill gauge��
	float getSkillGauge() { return skillGauge; }
	bool getUpgradeReady() { return upgradeReady; }
	//inferno��
	int getGaugeTime() { return _gaugeTime; }
	bool getGauging() { return isGauging; }
	

	void setHp(int _hp) { hp = _hp; }
	void setCoin(int _coin) { coin = _coin; }
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }

	//skill gauge��
	void setSkillGauge(float gauge) { skillGauge = gauge; }
	void setUpgradeReady(bool ready) { upgradeReady = ready; }
	//inferno��
	void setGauging(bool temp) { isGauging = temp; }
	void setGaugeTime(int time) { _gaugeTime = time; }

	
	tagStat getStat() { return stat; }
	void setStat(string itemName);

	int damageCul(int damage, bool isCritical);

	//inven
	void pushInven(tagItem item) { vInven.push_back(item); }
	vector<tagItem> getInven() { return vInven; }
	//

	bool criAppear();


	void setTile(vector<int> tile) { vTile = tile; }
	vector<int> getTile() { return vTile; }

	void setWall(vector<int> wall) { vWall = wall; }
	vector<int> getWall() { return vWall; }

	void setWallTile(tagTile* _wallTile) { wallTile = _wallTile; }
	tagTile* getWallTile() { return wallTile; }

	void setTile(tagTile* _tile) { tile = _tile; }
	tagTile* _getTile() { return tile; }


	playerData() {}
	~playerData() {}
};