#pragma once
#include "singletonBase.h"

struct tagItem
{
	string keyName;			//아이템 이름
	float damage;			//데미지
	float criDamage;		//크리티컬 데미지
	float criChance;		//크리티컬 확률
	int maxHp;				//최대 체력

	float potionDropChance;	//포션 드랍율
	float defenceChance;	//공격 방어율
	float burnChance;		//불태울 확률
	
	//normal
	bool goldPig;			//추가 코인 획득
	int CoolTimeReduction;	//스킬 쿨타임 감소
	bool glassMirror;		//유리 장막 유무

	//curse
	bool vampireBlood;		//흡혈기능

	bool doubleDamage;		//2배의 피해
	bool ManaRejection;		//몬스터를 공격했을 때 마나 거부
	bool criRejection;		//치명타 발생 거부
	bool reducedVisibility;	//시야 감소
	bool halfFace;			//체력이 50% 이하일 때 공격력 증가 유무
	bool bloodGold;			//보유 코인에 따라 공격력 증가 유무

	int price;				//가격
	string description;		//아이템 설명
	POINT frame;			//프레임 번호
};

class itemManager : public singletonBase <itemManager>
{
private:
	vector<tagItem> vItem;

public:
	HRESULT init();
	void addImage();
	void addItem(string keyName, float damage, float criDamage, float criChance, int maxHp,
		float potionDropChance, float defenceChance, float burnChance, bool goldPig, int CoolTimeReduction,
		bool glassMirror, bool vampireBlood, bool doubleDamage, bool ManaRejection, bool criRejection,
		bool reducedVisibility, bool halfFace, bool bloodGold, int price, string description, POINT frame);


	tagItem getItem(string keyName);
	tagItem getRandomItem(bool Normal = true);
};