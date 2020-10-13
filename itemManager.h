#pragma once
#include "singletonBase.h"

struct tagItem
{
	string keyName;			//������ �̸�
	float damage;			//������
	float criDamage;		//ũ��Ƽ�� ������
	float criChance;		//ũ��Ƽ�� Ȯ��
	int maxHp;				//�ִ� ü��
	int speed;				//�ӵ�

	float potionDropChance;	//���� �����
	float defenceChance;	//���� �����
	float burnChance;		//���¿� Ȯ��

	bool vampireBlood;		//�������

	int price;				//����
	string description;		//������ ����
	POINT frame;			//������ ��ȣ
};

class itemManager : public singletonBase <itemManager>
{
private:
	vector<tagItem> vItem;

public:
	HRESULT init();
	void addImage();
	void addItem(string keyName, float damage, float criDamage, float criChance, int maxHp, int speed, 
		float potionDropChance, float defenceChance, float burnChance, bool vampireBlood, int price, string description, POINT frame);

	tagItem getItem(string keyName);
	tagItem getRandomItem();
};