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
	//세이브
	if (INPUT->GetKeyDown(VK_LBUTTON))
	{
		//섹션, 키, 밸류
		INIDATA->addData("플밍22기", "으뜸", "100.99");
		INIDATA->addData("플밍22기", "의겸", "10000");
		INIDATA->addData("플밍22기", "정연", "10");

		vector<string> vStr;
		vStr.push_back("50.5");
		vStr.push_back("111");
		vStr.push_back("7");

		INIDATA->addData("플레이어", "공격력", vStr[1].c_str());
		INIDATA->addData("플레이어", "방어력", vStr[2].c_str());

		//파일 이름으로 세이프
		INIDATA->saveINI("플밍플밍");
	}
	//로드
	if (INPUT->GetKeyDown(VK_RBUTTON))
	{
		int num = INIDATA->loadDataInteger("플밍플밍", "플밍22기", "정연");
		cout << num << endl;
	}

}

void iniTestScene::render()
{
}
