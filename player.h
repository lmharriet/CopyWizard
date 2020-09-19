#pragma once
#include "gameNode.h"
#include "bullet.h"
#include "progressBar.h"
//���� Ŭ������ ���� ����� �ɾ��ָ� ������
//���� Ŭ���� ���漱���� ����� �Ѵ�
//#include "enemyManager.h" 

//��ȣ������ Ŭ���� ���漱��
class enemyManager;

class player : public gameNode
{

private:
	enemyManager* _em;			//����� �����Ҵ� ���� ���ƶ�!!

private:
	image* _rocket;					//����(�÷��̾�) �̹���
	missile* _missile;				//�̻��� Ŭ����
	bomb* _bomb;					//��ź Ŭ����

	progressBar* _hpBar;			//ü�¹�
	float _currentHp, _maxHp;		//�÷��̾� ����ü��, �ִ�ü��
	float _x, _y;					//�÷��̾� ��ġ��ǥ

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//��ź Ŭ���� ��������
	bomb* getBomb() { return _bomb; }

	//��ź����
	void removeBomb(int index);

	//�浹ó�� (�÷��̾� �Ѿ�, �̴Ͼ� �浹)
	void collision();

	//������
	void hitDamage(float damage);

	//��ȣ������ ���Ŵ��� �����͸� �Ѱܹ��� �����Լ�
	void setEMLink(enemyManager* em) { _em = em; }

	//�÷��̾� �̹��� ��������
	image* getPlayerImg() { return _rocket; }

	//������ ���̺�
	void save();
	//������ �ε�
	void load();

	player() {}
	~player() {}
};

