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
		"���� �����", 0.2f, 0, 0, 0, 0, 0, 0,
		false, 0, false,
		
		false, false, false, false, false, false, false, 
		125,
		"�������� 20% �����Ѵ�.",
		{ 1,0 });

	//���� �� (monster.cpp)
	addItem(
		"ȭ�� ����", 0, 0, 0, 0, 0, 0, 0.3f, 
		false, 0, false, 
		
		false, false, false, false, false, false, false,
		100,
		"30% Ȯ���� ���� ���¿��.",
		{ 2,0 });

	addItem(
		"��� ����", 0, 0.2f, 0.3f, 0, 0, 0, 0, 
		false, 0, false,
		
		false, false, false, false, false, false, false,
		170,
		"ũ��Ƽ�� �������� 20% �����ϰ�, Ȯ���� 30% �����Ѵ�.",
		{ 3,0 });

	addItem(
		"��Ŷ ��Ʈ", 0, 0, 0, 0, 0.2f, 0, 0, 
		false, 0, false, 
		
		false, false, false, false, false, false, false,
		90,
		"���� ������� 20% �����Ѵ�.",
		{ 4,0 });

	//���� �Ϸ� (player.cpp)
	addItem(
		"Ȳ�� ������", 0, 0, 0, 0, 0, 0, 0, 
		true, 0, false,
		
		false, false, false, false, false, false, false,
		70,
		"���� ȹ�� �� 50% Ȯ���� �߰� ���� ȹ��", 
		{ 5,0 });

	addItem(
		"����� �尩", 0, 0, 0, 150, 0, 0, 0, 
		false, 0, false,
		
		false, false, false, false, false, false, false,
		150,
		"�ִ� ü���� 150 �����Ѵ�.",
		{ 6,0 });

	addItem(
		"������ �ܰ�", 0, 0, 0.3f, 0, 0, 0, 0, 
		false, 0, false, 
		
		false, false, false, false, false, false, false,
		190,
		"ũ��Ƽ�� Ȯ���� 30% �����Ѵ�.",
		{ 7,0 });

	addItem(
		"�������� ������", 0, 0.2f, 0, 0, 0, 0, 0, 
		false, 0, false,
		
		false, false, false, false, false, false, false,
		140,
		"ũ��Ƽ�� �������� 20% �����Ѵ�",
		{ 8,0 });

	addItem(
		"Ȳ�� ������", 0.25f, 0.25f, 0.25f, 0, 0, 0, 0, 
		false, 0, false,
		
		false, false, false, false, false, false, false,
		240,
		"������, ũ��Ƽ�� ������, ũ��Ƽ�� Ȯ���� 25% �����Ѵ�.",
		{ 0,1 });

	//���� �� (player.cpp)
	addItem(
		"�ż��� ������", 0, 0, 0, 0, 0, 0, 0,
		false, 1, false,

		false, false, false, false, false, false, false,
		999,
		"��ų�� ��Ÿ���� 1�� ���ҽ�Ų��.",
		{ 1,1 });

	//���� ��
	addItem(
		"���� �ſ�", 0, 0, 0, 0, 0, 0, 0,
		false, 0, true,

		false, false, false, false, false, false, false,
		999,
		"�������� 5�ʰ� ���� ���� �� ������ �ѹ� �����ִ� ���� �帷�� �����Ѵ�.",
		{ 2,1 });

	//curse

	//���� �� (player.cpp)
	addItem(
		"�Ǹ��� ����", 0, 0, 0.4f, 0, 0, 0, 0, 
		false, 0, false,

		false, true, false, false, false, false, false,
		0,
		"2���� ���ظ� ������, ġ��Ÿ Ȯ���� 40% �����Ѵ�.",
		{ 0,2 });
	
	//���� �Ϸ� (gameScene.cpp)
	addItem(
		"���ֹ��� �����", 0, 0.5f, 0, 0, 0, 0, 0, 
		false, 0, false,

		false, false, true, false, false, false, false,
		0,
		"���̻� ���°������� ����������, ġ��Ÿ ���ݷ��� 50% �����Ѵ�.",
		{ 1,2 });

	//���� �Ϸ� (playerData.cpp)
	addItem(
		"������ ��", 0.4f, 0, 0, 0, 0, 0, 0, 
		false, 0, false,

		false, false, false, true, false, false, false,
		0,
		"ġ��Ÿ�� �߻����� ������, ���ݷ��� 40% �����Ѵ�.",
		{ 2,2 });

	//���� �Ϸ� (playerData.cpp)
	addItem(
		"�̸��� ��", 0, 0, 0, -250, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, true, false,
		0,
		"�ִ� ü���� 250 ����������, ü���� 50% ���ϰ� �Ǹ� ���ݷ��� 50% �����Ѵ�.",
		{ 3,2 });

	//���� �� (gameScene.cpp)
	addItem(
		"�������� ���� �Ȱ�", 0.3f, 0, 0, 0, 0, 0, 0, 
		false, 0, false,

		false, false, false, false, true, false, false,
		0,
		"�� �� �ִ� �þ߰� ����������, �������� 30% �����Ѵ�.",
		{ 4,2 });

	//���� �Ϸ� (playerData.cpp)
	addItem(
		"������ �˾�", -0.2f, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, false,
		0,
		"���ݷ��� 20% ����������, �ִ� ��������� ȸ�� �Ѵ�.",
		{ 5,2 });

	//���� �Ϸ� (playerData.cpp)
	addItem(
		"�ǹ��� �ݱ�", -0.2f, 0, 0, 0, 0, 0, 0,
		false, 0, false,

		false, false, false, false, false, false, true,
		0,
		"���ݷ��� 20% ����������, ������ ���� ������ ���� ���ݷ��� ������Ų��.",
		{ 6,2 });

	//���� �Ϸ�
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
		0,
		"�ִ� ü���� 100�����ϰų� �����ϰ�, ���ݷ��� 20% �����ϰų� �����Ѵ�.",
		{ 7,2 });

	//���� �� (monster.cpp)
	addItem(
		"������ īŸ��", 0, 0, 0, -200, 0, 0, 0, 
		false, 0, false, 
		
		true, false, false, false, false, false, false,
		0,
		"�ִ� ü���� 200 ���������� ���͸� ���� �� ���� ���� ü���� ȸ���Ѵ�.",
		{ 8,2 });

	addImage();
	return S_OK;
}

void itemManager::addImage()
{
	IMAGEMANAGER->addFrameImage("itemFrame", "Images/item/ItemSprite.bmp", 360, 120, 9, 3);
}

//������ �Ŵ����� �������� �߰��ϱ� ����
void itemManager::addItem(string keyName, float damage, float criDamage, float criChance, int maxHp,
	float potionDropChance, float defenceChance, float burnChance,
	bool goldPig, int CoolTimeReduction, bool glassMirror,

	bool vampireBlood, bool doubleDamage, bool ManaRejection, 
	bool criRejection, bool reducedVisibility, bool halfFace, bool bloodGold,
	int price,string description, POINT frame)
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

		price,
		description,frame };
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

	if (Normal) ranNum = RANDOM->range(1, 11); // normal

	else ranNum = RANDOM->range(12, vItem.size() - 1); // curse

	return vItem[ranNum];
}