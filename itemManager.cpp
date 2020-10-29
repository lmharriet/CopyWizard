#include "stdafx.h"
#include "itemManager.h"

HRESULT itemManager::init()
{
	addItem("ERROR", 0, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false, 
		999,
		"ERROR",
		{ 0,0 });

	//normal

	addItem(
		"힘의 목걸이", 0.2f, 0, 0, 0, 0, 0, 0,
		false, 0, false,
		
		false, false, false, false, false, false, false, 
		125,
		"데미지가 20% 증가한다.",
		{ 1,0 });

	////구현 취소 (monster.cpp)
	//addItem(
	//	"화염 구슬", 0, 0, 0, 0, 0, 0, 0.3f, 
	//	false, 0, false, 
	//	
	//	false, false, false, false, false, false, false,
	//	100,
	//	"30% 확률로 적을 불태운다.",
	//	{ 2,0 });

	addItem(
		"당근 케익", 0, 0.2f, 0.3f, 0, 0, 0, 0, 
		false, 0, false,
		
		false, false, false, false, false, false, false,
		170,
		"크리티컬 데미지가 20% 증가하고, 확률은 30% 증가한다.",
		{ 3,0 });

	addItem(
		"비스킷 세트", 0, 0, 0, 0, 0.2f, 0, 0, 
		false, 0, false, 
		
		false, false, false, false, false, false, false,
		90,
		"포션 드랍율이 20% 증가한다.",
		{ 4,0 });

	addItem(
		"황금 저금통", 0, 0, 0, 0, 0, 0, 0, 
		true, 0, false,
		
		false, false, false, false, false, false, false,
		70,
		"코인 획득 시 50% 확률로 추가 코인을 획득한다.", 
		{ 5,0 });

	addItem(
		"기사의 장갑", 0, 0, 0, 150, 0, 0, 0, 
		false, 0, false,
		
		false, false, false, false, false, false, false,
		150,
		"최대 체력이 150 증가한다.",
		{ 6,0 });

	addItem(
		"뾰족한 단검", 0, 0, 0.3f, 0, 0, 0, 0, 
		false, 0, false, 
		
		false, false, false, false, false, false, false,
		190,
		"크리티컬 확률이 30% 증가한다.",
		{ 7,0 });

	addItem(
		"고위관의 마법서", 0, 0.2f, 0, 0, 0, 0, 0, 
		false, 0, false,
		
		false, false, false, false, false, false, false,
		140,
		"크리티컬 데미지가 20% 증가한다",
		{ 8,0 });

	addItem(
		"황금 레시피", 0.25f, 0.25f, 0.25f, 0, 0, 0, 0, 
		false, 0, false,
		
		false, false, false, false, false, false, false,
		240,
		"데미지, 크리티컬 데미지, 크리티컬 확률이 25% 증가한다.",
		{ 0,1 });

	addItem(
		"신속의 마법서", 0, 0, 0, 0, 0, 0, 0,
		false, 1, false,

		false, false, false, false, false, false, false,
		125,
		"스킬 쿨타임을 1초 감소시킨다.",
		{ 1,1 });

	addItem(
		"유리 거울", 0, 0, 0, 0, 0, 0, 0,
		false, 0, true,

		false, false, false, false, false, false, false,
		150,
		"데미지를 5초간 입지 않을 때 공격을 한번 막아주는 유리 장막을 생성한다.",
		{ 2,1 });

	//curse

	addItem(
		"악마의 심장", 0, 0, 0.4f, 0, 0, 0, 0, 
		false, 0, false,

		false, true, false, false, false, false, false,
		0,
		"2배의 피해를 입지만, 치명타 확률이 40% 증가 한다.",
		{ 0,2 });
	
	addItem(
		"저주받은 목걸이", 0, 0.5f, 0, 0, 0, 0, 0, 
		false, 0, false,

		false, false, true, false, false, false, false,
		0,
		"더이상 마력게이지가 차지않지만, 치명타 공격력이 50% 증가 한다.",
		{ 1,2 });

	addItem(
		"거인의 덫", 0.4f, 0, 0, 0, 0, 0, 0, 
		false, 0, false,

		false, false, false, true, false, false, false,
		0,
		"치명타가 발생하지 않지만, 공격력이 40% 증가 한다.",
		{ 2,2 });

	addItem(
		"이면의 얼굴", 0, 0, 0, -250, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, true, false,
		0,
		"최대 체력이 250 감소하지만, 체력이 50% 이하가 되면 공격력이 50% 증가 한다.",
		{ 3,2 });

	addItem(
		"고학자의 깨진 안경", 0.3f, 0, 0, 0, 0, 0, 0, 
		false, 0, false,

		false, false, false, false, true, false, false,
		0,
		"볼 수 있는 시야가 감소하지만, 데미지가 30% 증가 한다.",
		{ 4,2 });

	addItem(
		"수상한 알약", -0.2f, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0,
		"공격력이 20% 감소하지만, 최대 생명력으로 회복 한다.",
		{ 5,2 });

	addItem(
		"피묻은 금괴", -0.2f, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, true,
		0,
		"공격력이 20% 감소하지만, 보유한 코인 개수에 따라 공격력이 증가 한다.",
		{ 6,2 });

	int ran = RANDOM->range(0, 1);
	int plusHp = -100;
	float plusAttack = -0.2f;
	if (ran == 1) plusHp = 100;
	ran = RANDOM->range(0, 1);
	if (ran == 1) plusAttack = 0.2f;

	addItem(
		"도박사의 주사위",
		plusAttack, 0, 0, plusHp, 0, 0, 0, 
		false, 0, false,
		
		false, false, false, false, false, false, false,
		0,
		"최대 체력이 100 감소하거나 증가하고, 공격력이 20% 감소하거나 증가 한다.",
		{ 7,2 });

	addItem(
		"흡혈의 카타나", 0, 0, 0, -200, 0, 0, 0, 
		false, 0, false, 
		
		true, false, false, false, false, false, false,
		0,
		"최대 체력이 200 감소하지만 몬스터를 잡을 때 마다 일정 체력을 회복 한다.",
		{ 8,2 });

	//start scene item
	addStartItem();

	addImage();
	return S_OK;
}

void itemManager::addStartItem()
{
	addItem("힘의 조각", 0.05f, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		2,
		"데미지가 5% 증가한다.",
		{ 0,3 });

	addItem("철갑옷", 0, 0, 0, 25, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		1,
		"최대 체력이 25 증가한다.",
		{ 3,3 });

	addItem("낡은 단검", 0, 0.05f, 0.25f, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		5,
		"크리티컬 확률이 25%, 크리티컬 데미지가 5% 증가한다.",
		{ 6,3 });

	addItem("힘법사의 도끼", 0.1f, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		5,
		"데미지가 10% 증가한다.",
		{ 1,3 });

	addItem("뼈갑옷", 0, 0, 0, 50, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		4,
		"최대 체력이 50 증가한다.",
		{ 4,3 });

	addItem("마법사의 단검", 0, 0.1f, 0.05f, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		6,
		"크리티컬 확률이 15%, 크리티컬 데미지가 10% 증가한다.",
		{ 7,3 });

	addItem("오거파워건틀릿", 0.15f, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		8,
		"데미지가 15% 증가한다.",
		{ 2,3 });

	addItem("황금갑옷", 0, 0, 0, 100, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		7,
		"최대 체력이 100 증가한다.",
		{ 5,3 });

	addItem("황금 단검", 0, 0.15f, 0.05f, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		4,
		"크리티컬 확률이 5%, 크리티컬 데미지가 15% 증가한다.",
		{ 8,3 });

	addItem("저격수의 고글", 0, 0, 0.25f, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		4,
		"크리티컬 확률이 25% 증가한다.",
		{ 0,4 });

	addItem("톱니바퀴", 0, 0.15f, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		5,
		"크리티컬 데미지가 15% 증가한다.",
		{ 1,4 });

	addItem("집중의 로브", 0, 0, 0, 0, 0, 0, 0,
		false, 1, false,

		false, false, false, false, false, false, false,
		7,
		"스킬 쿨타임을 1초 감소시킨다.",
		{ 2,4 });

	addItem("활력의 심장", 0, 0, 0, 0, 0.1f, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		4,
		"포인 드랍률이 10% 증가한다.",
		{ 3,4 });
	//
	addItem("미다스의 손", 0, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		6,
		"코인 획득 시 50 % 확률로 추가 코인을 획득한다.",
		{ 4,4 });

	addItem("에메랄드", 0, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		8,
		"??? 드랍율이 20% 증가한다.",
		{ 5,4 });

	addItem("보험증서", 0, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		8,
		"1회에 한해 부활을 한다.",
		{ 6,4 });

	addItem("절대자의 로브", 0, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		10,
		"5초마다 1초간 데미지를 입지 않는다.",
		{ 7,4 });

	addItem("마력의 근원", 0, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		3,
		"마력게이지의 충전량이 증가한다.",
		{ 8,4 });

	addItem("흡혈귀의 송곳니", 0, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0,
		"치명타 확률이 50% 감소하지만 치명타 발동 시 일정 체력을 회복한다.",
		{ 0,5 });

	addItem("암살자의 복장", 0, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0,
		"최대 체력이 250 감소하지만 무조건 치명타 공격이 발동된다.",
		{ 1,5 });

	addItem("회귀의 반지", 0, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0,
		"더 이상 마력 게이지가 차지 않지만, 일정확률로 같은 스킬을 반복 사용한다.",
		{ 2,5 });

	addItem("시간의 모래시계", 0, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0,
		"입는 데미지가 2배가 되지만 5초 동안 데미지를 입지 않으면 데미지가 2배가 된다.",
		{ 3,5 });
}

void itemManager::addImage()
{
	IMAGEMANAGER->addFrameImage("itemFrame", "Images/item/ItemSprite.bmp", 360, 240, 9, 6);
	IMAGEMANAGER->addImage("glassEffect", "Images/item/brokenGlass.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
}

//아이템 매니져에 아이템을 추가하기 위함
void itemManager::addItem(string keyName, float damage, float criDamage, float criChance, int maxHp,
	float potionDropChance, float defenceChance, float burnChance,
	bool goldPig, int CoolTimeReduction, bool glassMirror,

	bool vampireBlood, bool doubleDamage, bool ManaRejection, 
	bool criRejection, bool reducedVisibility, bool halfFace, bool bloodGold,
	int price,string Explanation, POINT frame)
{
	/*
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
	bool ManaRejection;		//몬스터를 공격했을 때 마나 획득 유무
	bool criRejection;		//치명타 발생 유무
	bool reducedVisibility;	//시야 감소
	bool halfFace;			//체력이 50% 이하일 때 공격력 증가 유무
	bool bloodGold;			//보유 코인에 따라 공격력 증가 유무

	int price;				//가격
	string description;		//아이템 설명
	POINT frame;			//프레임 번호
	*/
	tagItem item = { 
		keyName,
		damage,
		criDamage,
		criChance,
		maxHp,
		potionDropChance,
		defenceChance,
		burnChance,

		goldPig,
		CoolTimeReduction,
		glassMirror,

		vampireBlood,

		doubleDamage,
		ManaRejection,
		criRejection,
		reducedVisibility,
		halfFace,
		bloodGold,

		price,
		Explanation,frame };
	vItem.push_back(item);
}

//아이템을 가져오기 위함
tagItem itemManager::getItem(string keyName)
{
	vector<tagItem>::iterator iter = vItem.begin();

	for (iter; iter != vItem.end(); ++iter)
	{
		if (keyName == iter->keyName)
		{
			//cout << "정상적으로 불러옴" << '\n';
			return *iter;
		}
	}

	cout << "쓰래기값을 불러옴" << '\n';
	tagItem item = vItem[0];
	return item;
}

tagItem itemManager::getRandomItem(bool Normal)
{
    int ranNum = 0;

    if (Normal)
    {
        ranNum = RANDOM->range(1, 10); // normal
    }

    else ranNum = RANDOM->range(11, 19); // curse

    return vItem[ranNum];
}