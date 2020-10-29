#include "stdafx.h"
#include "itemManager.h"

HRESULT itemManager::init()
{
	addItem("ERROR", 0, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false, 
		0, 0, 0, 0, 0,
		999,
		"ERROR",
		{ 0,0 });

	//normal

	addItem(
		"���� �����", 0.2f, 0, 0, 0, 0, 0, 0,
		false, 0, false,
		
		false, false, false, false, false, false, false, 
		0, 0, 0, 0, 0,
		125,
		"�������� 20% �����Ѵ�.",
		{ 1,0 });

	////���� ��� (monster.cpp)
	//addItem(
	//	"ȭ�� ����", 0, 0, 0, 0, 0, 0, 0.3f, 
	//	false, 0, false, 
	//	
	//	false, false, false, false, false, false, false,
	//	100,
	//	"30% Ȯ���� ���� ���¿��.",
	//	{ 2,0 });

	addItem(
		"��� ����", 0, 0.2f, 0.3f, 0, 0, 0, 0, 
		false, 0, false,
		
		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		170,
		"ũ��Ƽ�� �������� 20% �����ϰ�, Ȯ���� 30% �����Ѵ�.",
		{ 3,0 });

	addItem(
		"��Ŷ ��Ʈ", 0, 0, 0, 0, 0.2f, 0, 0, 
		false, 0, false, 
		
		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		90,
		"���� ������� 20% �����Ѵ�.",
		{ 4,0 });

	addItem(
		"Ȳ�� ������", 0, 0, 0, 0, 0, 0, 0, 
		true, 0, false,
		
		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		70,
		"���� ȹ�� �� 50% Ȯ���� �߰� ������ ȹ���Ѵ�.", 
		{ 5,0 });

	addItem(
		"����� �尩", 0, 0, 0, 150, 0, 0, 0, 
		false, 0, false,
		
		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		150,
		"�ִ� ü���� 150 �����Ѵ�.",
		{ 6,0 });

	addItem(
		"������ �ܰ�", 0, 0, 0.3f, 0, 0, 0, 0, 
		false, 0, false, 
		
		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		190,
		"ũ��Ƽ�� Ȯ���� 30% �����Ѵ�.",
		{ 7,0 });

	addItem(
		"�������� ������", 0, 0.2f, 0, 0, 0, 0, 0, 
		false, 0, false,
		
		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		140,
		"ũ��Ƽ�� �������� 20% �����Ѵ�",
		{ 8,0 });

	addItem(
		"Ȳ�� ������", 0.25f, 0.25f, 0.25f, 0, 0, 0, 0, 
		false, 0, false,
		
		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		240,
		"������, ũ��Ƽ�� ������, ũ��Ƽ�� Ȯ���� 25% �����Ѵ�.",
		{ 0,1 });

	addItem(
		"�ż��� ������", 0, 0, 0, 0, 0, 0, 0,
		false, 1, false,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		125,
		"��ų ��Ÿ���� 1�� ���ҽ�Ų��.",
		{ 1,1 });

	addItem(
		"���� �ſ�", 0, 0, 0, 0, 0, 0, 0,
		false, 0, true,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		150,
		"�������� 5�ʰ� ���� ���� �� ������ �ѹ� �����ִ� ���� �帷�� �����Ѵ�.",
		{ 2,1 });

	//curse

	addItem(
		"�Ǹ��� ����", 0, 0, 0.4f, 0, 0, 0, 0, 
		false, 0, false,

		false, true, false, false, false, false, false,
		0, 0, 0, 0, 0,
		0,
		"2���� ���ظ� ������, ġ��Ÿ Ȯ���� 40% ���� �Ѵ�.",
		{ 0,2 });
	
	addItem(
		"���ֹ��� �����", 0, 0.5f, 0, 0, 0, 0, 0, 
		false, 0, false,

		false, false, true, false, false, false, false,
		0, 0, 0, 0, 0,
		0,
		"���̻� ���°������� ����������, ġ��Ÿ ���ݷ��� 50% ���� �Ѵ�.",
		{ 1,2 });

	addItem(
		"������ ��", 0.4f, 0, 0, 0, 0, 0, 0, 
		false, 0, false,

		false, false, false, true, false, false, false,
		0, 0, 0, 0, 0,
		0,
		"ġ��Ÿ�� �߻����� ������, ���ݷ��� 40% ���� �Ѵ�.",
		{ 2,2 });

	addItem(
		"�̸��� ��", 0, 0, 0, -250, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, true, false,
		0, 0, 0, 0, 0,
		0,
		"�ִ� ü���� 250 ����������, ü���� 50% ���ϰ� �Ǹ� ���ݷ��� 50% ���� �Ѵ�.",
		{ 3,2 });

	addItem(
		"�������� ���� �Ȱ�", 0.3f, 0, 0, 0, 0, 0, 0, 
		false, 0, false,

		false, false, false, false, true, false, false,
		0, 0, 0, 0, 0,
		0,
		"�� �� �ִ� �þ߰� ����������, �������� 30% ���� �Ѵ�.",
		{ 4,2 });

	addItem(
		"������ �˾�", -0.2f, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		0,
		"���ݷ��� 20% ����������, �ִ� ��������� ȸ�� �Ѵ�.",
		{ 5,2 });

	addItem(
		"�ǹ��� �ݱ�", -0.2f, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, true,
		0, 0, 0, 0, 0,
		0,
		"���ݷ��� 20% ����������, ������ ���� ������ ���� ���ݷ��� ���� �Ѵ�.",
		{ 6,2 });

	int ran = RANDOM->range(0, 1);
	int plusHp = -100;
	float plusAttack = -0.2f;
	if (ran == 1) plusHp = 100;
	ran = RANDOM->range(0, 1);
	if (ran == 1) plusAttack = 0.2f;

	addItem(
		"���ڻ��� �ֻ���",
		plusAttack, 0, 0, plusHp, 0, 0, 0, 
		false, 0, false,
		
		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		0,
		"�ִ� ü���� 100 �����ϰų� �����ϰ�, ���ݷ��� 20% �����ϰų� ���� �Ѵ�.",
		{ 7,2 });

	addItem(
		"������ īŸ��", 0, 0, 0, -200, 0, 0, 0, 
		false, 0, false, 
		
		true, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		0,
		"�ִ� ü���� 200 ���������� ���͸� ���� �� ���� ���� ü���� ȸ�� �Ѵ�.",
		{ 8,2 });

	//start scene item
	addStartItem();

	addImage();
	return S_OK;
}

void itemManager::addStartItem()
{
	addItem("���� ����", 0.05f, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		2,
		"�������� 5% �����Ѵ�.",
		{ 0,3 });

	addItem("ö����", 0, 0, 0, 25, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		1,
		"�ִ� ü���� 25 �����Ѵ�.",
		{ 3,3 });

	addItem("���� �ܰ�", 0, 0.05f, 0.25f, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		5,
		"ũ��Ƽ�� Ȯ���� 25%, ũ��Ƽ�� �������� 5% �����Ѵ�.",
		{ 6,3 });

	addItem("�������� ����", 0.1f, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		5,
		"�������� 10% �����Ѵ�.",
		{ 1,3 });

	addItem("������", 0, 0, 0, 50, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		4,
		"�ִ� ü���� 50 �����Ѵ�.",
		{ 4,3 });

	addItem("�������� �ܰ�", 0, 0.1f, 0.05f, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		6,
		"ũ��Ƽ�� Ȯ���� 15%, ũ��Ƽ�� �������� 10% �����Ѵ�.",
		{ 7,3 });

	addItem("�����Ŀ���Ʋ��", 0.15f, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		8,
		"�������� 15% �����Ѵ�.",
		{ 2,3 });

	addItem("Ȳ�ݰ���", 0, 0, 0, 100, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		7,
		"�ִ� ü���� 100 �����Ѵ�.",
		{ 5,3 });

	addItem("Ȳ�� �ܰ�", 0, 0.15f, 0.05f, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		4,
		"ũ��Ƽ�� Ȯ���� 5%, ũ��Ƽ�� �������� 15% �����Ѵ�.",
		{ 8,3 });

	addItem("���ݼ��� ���", 0, 0, 0.25f, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		4,
		"ũ��Ƽ�� Ȯ���� 25% �����Ѵ�.",
		{ 0,4 });

	addItem("��Ϲ���", 0, 0.15f, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		5,
		"ũ��Ƽ�� �������� 15% �����Ѵ�.",
		{ 1,4 });

	addItem("������ �κ�", 0, 0, 0, 0, 0, 0, 0,
		false, 1, false,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		7,
		"��ų ��Ÿ���� 1�� ���ҽ�Ų��.",
		{ 2,4 });

	addItem("Ȱ���� ����", 0, 0, 0, 0, 0.1f, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		4,
		"���� ������� 10% �����Ѵ�.",
		{ 3,4 });
	//
	addItem("�̴ٽ��� ��", 0, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		true, 0, 0, 0, 0,
		6,
		"���� ȹ�� �� 50 % Ȯ���� �߰� ������ ȹ���Ѵ�.",
		{ 4,4 });

	addItem("���޶���", 0, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, .2f, 0, 0, 0,
		8,
		"���� ������� 20% �����Ѵ�.",
		{ 5,4 });

	addItem("��������", 0, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, 0, true, 0, 0,
		8,
		"1ȸ�� ���� ��Ȱ�� �Ѵ�.",
		{ 6,4 });

	addItem("�������� �κ�", 0, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, 0, 0, true, 0,
		10,
		"5�ʸ��� 1�ʰ� �������� ���� �ʴ´�.",
		{ 7,4 });

	addItem("������ �ٿ�", 0, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, true,
		3,
		"���°������� �������� �����Ѵ�.",
		{ 8,4 });

	//====================================================================//

	addItem("�������� �۰���", 0, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		0,
		"ġ��Ÿ Ȯ���� 50% ���������� ġ��Ÿ �ߵ� �� ���� ü���� ȸ���Ѵ�.",
		{ 0,5 });

	addItem("�ϻ����� ����", 0, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		0,
		"�ִ� ü���� 250 ���������� ������ ġ��Ÿ ������ �ߵ��ȴ�.",
		{ 1,5 });

	addItem("ȸ���� ����", 0, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		0,
		"�� �̻� ���� �������� ���� ������, ����Ȯ���� ���� ��ų�� �ݺ� ����Ѵ�.",
		{ 2,5 });

	addItem("�ð��� �𷡽ð�", 0, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0, 0, 0, 0, 0,
		0,
		"�Դ� �������� 2�谡 ������ 5�� ���� �������� ���� ������ �������� 2�谡 �ȴ�.",
		{ 3,5 });
}

void itemManager::addImage()
{
	IMAGEMANAGER->addFrameImage("itemFrame", "Images/item/ItemSprite.bmp", 360, 240, 9, 6);
	IMAGEMANAGER->addImage("glassEffect", "Images/item/brokenGlass.bmp", WINSIZEX, WINSIZEY, true, RGB(255, 0, 255));
}

//������ �Ŵ����� �������� �߰��ϱ� ����
void itemManager::addItem(string keyName, float damage, float criDamage, float criChance,
	int maxHp, float potionDropChance, float defenceChance, float burnChance,
	bool goldPig, int CoolTimeReduction, bool glassMirror, bool vampireBlood,
	bool doubleDamage, bool ManaRejection, bool criRejection, bool reducedVisibility,
	bool halfFace, bool bloodGold, bool midasHand, float gemDropPersent, bool resurrection,
	bool invincibility, bool magicStone, int price, string Explanation, POINT frame)
{
	/*
	string keyName;			//������ �̸�
	float damage;			//������
	float criDamage;		//ũ��Ƽ�� ������
	float criChance;		//ũ��Ƽ�� Ȯ��
	int maxHp;				//�ִ� ü��

	float potionDropChance;	//���� �����
	float defenceChance;	//���� �����
	float burnChance;		//���¿� Ȯ��
	
	//normal
	bool goldPig;			//�߰� ���� ȹ��
	int CoolTimeReduction;	//��ų ��Ÿ�� ����
	bool glassMirror;		//���� �帷 ����

	//curse
	bool vampireBlood;		//�������

	bool doubleDamage;		//2���� ����
	bool ManaRejection;		//���͸� �������� �� ���� ȹ�� ����
	bool criRejection;		//ġ��Ÿ �߻� ����
	bool reducedVisibility;	//�þ� ����
	bool halfFace;			//ü���� 50% ������ �� ���ݷ� ���� ����
	bool bloodGold;			//���� ���ο� ���� ���ݷ� ���� ����

	float gemDropPersent;
	bool resurrection;
	bool invincibility;
	bool magicStone;

	int price;				//����
	string description;		//������ ����
	POINT frame;			//������ ��ȣ
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

		midasHand,
		gemDropPersent,
		resurrection,
		invincibility,
		magicStone,

		price,
		Explanation,frame };
	vItem.push_back(item);
}

//�������� �������� ����
tagItem itemManager::getItem(string keyName)
{
	vector<tagItem>::iterator iter = vItem.begin();

	for (iter; iter != vItem.end(); ++iter)
	{
		if (keyName == iter->keyName)
		{
			//cout << "���������� �ҷ���" << '\n';
			return *iter;
		}
	}

	cout << "�����Ⱚ�� �ҷ���" << '\n';
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