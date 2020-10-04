#include "stdafx.h"
#include "itemManager.h"

HRESULT itemManager::init()
{
	addItem("ERROR", 0, 0, 0, 0, 0, 0, 0, 0, false, 
		"ERROR", 
		{ 0,0 });

	addItem(
		"힘의 목걸이", 0.2f,0,0,0,0,0,0,0,false,
		"공격력이 20% 증가한다.",
		{ 1,0 });

	addItem(
		"흡혈의 카타나",0,0,0,-200,0,0,0,0,true,
		"최대 체력이 200 감소하지만 몬스터를 잡을 때 마다 일정 체력을 회복한다.", 
		{ 2,0 });

	addItem(
		"신속의 장화",0,0, 0, 0,2,0,0, 0, false,
		"이동속도가 2만큼 증가한다.",
		{ 3,0 });

	addItem(
		"화염 구슬",0,0,0, 0, 0, 0,0, 0.3f,false,
		"30% 확률로 적을 불태운다.",
		{ 4,0 });

	addItem(
		"당근 케익",0, 0.2f,0.3f,0, 0,0,0,0, false, 
		"크리티컬 데미지가 20% 증가하고, 확률은 30% 증가한다.",
		{ 5,0 });

	addItem(
		"비스킷 세트",0,0, 0, 0,0,0.5f,0,0, false,
		"포션 드랍율이 50% 증가한다.",
		{ 6,0 });

	addItem(
		"자물쇠 반지",0, 0, 0,0, 0,0,0, 0, false, 
		"여기에 설명",
		{ 7,0 });//미정

	addItem(
		"황금 저금통",0,0,0,0,0,0,0,0,false,
		"여기에 설명", 
		{ 8,0 });//미정
	
	addItem(
		"유리 거울",0,0,0,0,0,0,0,0,false,
		"여기에 설명", 
		{ 0,1 });//미정

	addItem(
		"뾰족한 단검", 0,0,0,0.5f,0,0,0,0,false,
		"치명타 확률이 50% 증가한다.",
		{ 1,1 });

	addItem(
		"금기된 마법서",1.0f,0,0,-300,0,0,0,0,false,
		"최대 체력이 300 감소하지만 데미지가 2배 된다.", 
		{ 2,1 });

	addItem(
		"신속의 마법서",0,0,0,0,0,0,0,0,false,
		"여기에 설명",
		{ 3,1 });//미정

	addItem(
		"황금 레시피",0.3f,0.3f,0.3f,0,0,0,0,0,false,
		"공격력, 크리티컬 데미지, 크리티컬 확률이 30% 증가한다.",
		{ 4,1 });

	addItem(
		"기사의 장갑",0,0,0,0,0,0,150,0,false,
		"최대 체력이 150 증가한다.", 
		{ 5,1 });

	return S_OK;
}

void itemManager::addImage()
{
	IMAGEMANAGER->addFrameImage("itemFrame", "Images/item/ItemSprite.bmp", 360, 80, 9, 2);
}

//아이템 매니져에 아이템을 추가하기 위함
void itemManager::addItem(string keyName, float damage, float criDamage, float criChance, int maxHp, int speed,
	float potionDropChance, float defenceChance, float burnChance, bool vampireBlood, string description, POINT frame)
{
	tagItem item = { keyName,damage,criDamage,criChance,maxHp,speed,potionDropChance,defenceChance,burnChance,vampireBlood,description,frame };
	vItem.push_back(item);
}

//아이템을 가져오기 위함
tagItem itemManager::getItem(string keyName)
{
	vector<tagItem>::iterator iter = vItem.begin();

	for (iter; iter != vItem.begin(); ++iter)
	{
		if (keyName == iter->keyName)
		{
			cout << "정상적으로 불러옴" << '\n';
			return *iter;
		}
	}

	cout << "쓰래기값을 불러옴" << '\n';
	tagItem item = vItem[0];
	return item;
}