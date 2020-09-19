#pragma once
#include "singletonBase.h"
//=============================================================
//	## input ## �Է¸Ŵ��� (Ű����, ���콺��ư)
//=============================================================

#define KEYMAX 256

class input : public singletonBase <input>
{
private:
	bool _keyUp[KEYMAX];
	bool _keyDown[KEYMAX];

public:
	//�Է¸Ŵ��� �ʱ�ȭ
	HRESULT init();
	//�Է¸Ŵ��� ����
	void release();

	//Ű�� ��� ���� �ֳ�?
	bool GetKey(int key);
	//Ű�� �ѹ��� ���ȳ�?
	bool GetKeyDown(int key);
	//Ű�� �ѹ� ���ȴ� �����?
	bool GetKeyUp(int key);
	//���Ű��? (����� ����Ƽ���� �̱���� ����)
	bool GetToggleKey(int key);

	input() {}
	~input() {}
};