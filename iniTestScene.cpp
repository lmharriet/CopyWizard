#include "stdafx.h"
#include "iniTestScene.h"


HRESULT iniTestScene::init()
{
	return S_OK;
}

void iniTestScene::release()
{
}

void iniTestScene::update()
{
	//���̺�
	if (INPUT->GetKeyDown(VK_LBUTTON))
	{
		//����, Ű, ���
		INIDATA->addData("�ù�22��", "����", "100.99");
		INIDATA->addData("�ù�22��", "�ǰ�", "10000");
		INIDATA->addData("�ù�22��", "����", "10");

		vector<string> vStr;
		vStr.push_back("50.5");
		vStr.push_back("111");
		vStr.push_back("7");

		INIDATA->addData("�÷��̾�", "���ݷ�", vStr[1].c_str());
		INIDATA->addData("�÷��̾�", "����", vStr[2].c_str());

		//���� �̸����� ������
		INIDATA->saveINI("�ù��ù�");
	}
	//�ε�
	if (INPUT->GetKeyDown(VK_RBUTTON))
	{
		int num = INIDATA->loadDataInteger("�ù��ù�", "�ù�22��", "����");
		cout << num << endl;
	}

}

void iniTestScene::render()
{
}
