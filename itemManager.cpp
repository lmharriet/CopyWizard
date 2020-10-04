#include "stdafx.h"
#include "itemManager.h"

HRESULT itemManager::init()
{
	addItem("ERROR", 0, 0, 0, 0, 0, 0, 0, 0, false, 
		"ERROR", 
		{ 0,0 });

	addItem(
		"���� �����", 0.2f,0,0,0,0,0,0,0,false,
		"���ݷ��� 20% �����Ѵ�.",
		{ 1,0 });

	addItem(
		"������ īŸ��",0,0,0,-200,0,0,0,0,true,
		"�ִ� ü���� 200 ���������� ���͸� ���� �� ���� ���� ü���� ȸ���Ѵ�.", 
		{ 2,0 });

	addItem(
		"�ż��� ��ȭ",0,0, 0, 0,2,0,0, 0, false,
		"�̵��ӵ��� 2��ŭ �����Ѵ�.",
		{ 3,0 });

	addItem(
		"ȭ�� ����",0,0,0, 0, 0, 0,0, 0.3f,false,
		"30% Ȯ���� ���� ���¿��.",
		{ 4,0 });

	addItem(
		"��� ����",0, 0.2f,0.3f,0, 0,0,0,0, false, 
		"ũ��Ƽ�� �������� 20% �����ϰ�, Ȯ���� 30% �����Ѵ�.",
		{ 5,0 });

	addItem(
		"��Ŷ ��Ʈ",0,0, 0, 0,0,0.5f,0,0, false,
		"���� ������� 50% �����Ѵ�.",
		{ 6,0 });

	addItem(
		"�ڹ��� ����",0, 0, 0,0, 0,0,0, 0, false, 
		"���⿡ ����",
		{ 7,0 });//����

	addItem(
		"Ȳ�� ������",0,0,0,0,0,0,0,0,false,
		"���⿡ ����", 
		{ 8,0 });//����
	
	addItem(
		"���� �ſ�",0,0,0,0,0,0,0,0,false,
		"���⿡ ����", 
		{ 0,1 });//����

	addItem(
		"������ �ܰ�", 0,0,0,0.5f,0,0,0,0,false,
		"ġ��Ÿ Ȯ���� 50% �����Ѵ�.",
		{ 1,1 });

	addItem(
		"�ݱ�� ������",1.0f,0,0,-300,0,0,0,0,false,
		"�ִ� ü���� 300 ���������� �������� 2�� �ȴ�.", 
		{ 2,1 });

	addItem(
		"�ż��� ������",0,0,0,0,0,0,0,0,false,
		"���⿡ ����",
		{ 3,1 });//����

	addItem(
		"Ȳ�� ������",0.3f,0.3f,0.3f,0,0,0,0,0,false,
		"���ݷ�, ũ��Ƽ�� ������, ũ��Ƽ�� Ȯ���� 30% �����Ѵ�.",
		{ 4,1 });

	addItem(
		"����� �尩",0,0,0,0,0,0,150,0,false,
		"�ִ� ü���� 150 �����Ѵ�.", 
		{ 5,1 });

	return S_OK;
}

void itemManager::addImage()
{
	IMAGEMANAGER->addFrameImage("itemFrame", "Images/item/ItemSprite.bmp", 360, 80, 9, 2);
}

//������ �Ŵ����� �������� �߰��ϱ� ����
void itemManager::addItem(string keyName, float damage, float criDamage, float criChance, int maxHp, int speed,
	float potionDropChance, float defenceChance, float burnChance, bool vampireBlood, string description, POINT frame)
{
	tagItem item = { keyName,damage,criDamage,criChance,maxHp,speed,potionDropChance,defenceChance,burnChance,vampireBlood,description,frame };
	vItem.push_back(item);
}

//�������� �������� ����
tagItem itemManager::getItem(string keyName)
{
	vector<tagItem>::iterator iter = vItem.begin();

	for (iter; iter != vItem.begin(); ++iter)
	{
		if (keyName == iter->keyName)
		{
			cout << "���������� �ҷ���" << '\n';
			return *iter;
		}
	}

	cout << "�����Ⱚ�� �ҷ���" << '\n';
	tagItem item = vItem[0];
	return item;
}