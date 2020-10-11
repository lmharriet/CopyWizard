#pragma once
#include "singletonBase.h"
#include "tileNode.h"
struct tagStat
{
	float damage;			//데미지
	float criDamage;		//크리티컬 데미지
	float criChance;		//크리티컬 확률

	float potionDropChance;	//포션 드랍율
	float defenceChance;	//공격 방어율
	float burnChance;		//불태울 확률

	bool vampireBlood;		//흡혈기능
};
class playerData : public singletonBase <playerData>
{
private:
	//stat//
	int hp;
	int maxHp;	
	
	tagStat stat;
	////////
	int coin;
	float _x, _y;

	int _gaugeTime;

	
	tagTile* tile;
	vector<int> vTile;

	tagTile* wallTile;
	vector<int> vWall;
public:
	HRESULT init();
	void release();
	void update();

	int getMaxHp() { return maxHp; }
	int getHp() { return hp; }
	int getCoin() { return coin; }
	float getX() { return _x; }
	float getY() { return _y; }
	int getGaugeTime() { return _gaugeTime; }

	void setHp(int _hp) { hp = _hp; }
	void setCoin(int _coin) { coin = _coin; }
	void setX(float x) { _x = x; }
	void setY(float y) { _y = y; }
	void setGaugeTime(int time) { _gaugeTime = time; }


	tagStat getStat() { return stat; }
	void setStat(string statName, float value);

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