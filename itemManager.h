#pragma once
#include "singletonBase.h"

struct tagItem
{
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