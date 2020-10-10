#include "stdafx.h"
#include "mainGame.h"

//=============================================================
//	## �ʱ�ȭ ## init()
//=============================================================
HRESULT mainGame::init()
{
	gameNode::init();
	IMAGEMANAGER->addImage("cursor", "wizard/cursor.bmp", 64, 64, true, RGB(255, 0, 255));

	/*������ ���ΰ����� ������ ���鸸 ������ �Ѵ�*/
	/*�̰����� ������ ������ �߰��ϰ� ������� �����Ѵ�*/

	soundInit();

	
	/*���߰�*/
	SCENEMANAGER->addScene("����ȭ��", new startScene);
	//SCENEMANAGER->addScene("�ȼ��浹", new pixelCollisionScene);
	SCENEMANAGER->addScene("�ΰ���", new gameScene);
	SCENEMANAGER->addScene("������", new bossScene);

	mapToolScene* maptool = new mapToolScene;
	SCENEMANAGER->addScene("��������", maptool);
	maptool->init();
	maptool->addImage();
	/*�����*/

	SCENEMANAGER->loadScene("�ΰ���");
	//SCENEMANAGER->loadScene("����ȭ��");
	//SCENEMANAGER->loadScene("��������");
	//SCENEMANAGER->loadScene("������");

	ShowCursor(false);
	return S_OK;
}

//=============================================================
//	## ���� ## release()
//=============================================================
void mainGame::release()
{
	
	gameNode::release();
}

//=============================================================
//	## ������Ʈ ## update()
//=============================================================
void mainGame::update()
{
	gameNode::update();

	//���� ��Ʈ�� 
	this->volumeControl();
	//���Ŵ��� ������Ʈ
	SCENEMANAGER->update();

	//����Ŵ��� ������Ʈ (�̰� ������ ����Ŵ��� ����� �������� �ʴ´�!!!)
	SOUNDMANAGER->update();

	if(INPUT->GetKeyDown(0x31)) // 1
		SCENEMANAGER->loadScene("����ȭ��");
	if(INPUT->GetKeyDown(0x32)) // 2
		SCENEMANAGER->loadScene("��������");
	if(INPUT->GetKeyDown(0x33)) // 3
		SCENEMANAGER->loadScene("�ΰ���");
	if(INPUT->GetKeyDown(0x34)) // 4
		SCENEMANAGER->loadScene("������");
}

//=============================================================
//	## ���� ## render()
//=============================================================
void mainGame::render()
{
	//��� �� ��Ʈ�� (�̰��� ������ �׳� �α�)
	PatBlt(getMemDC(), 0, 0, WINSIZEX, WINSIZEY, BLACKNESS);
//=============================================================
	
	//���ڻ� ��� ���ֱ�
	SetBkMode(getMemDC(), TRANSPARENT);

	//���Ŵ��� ����
	SCENEMANAGER->render();
	TIME->render(getMemDC());
	//IMAGEMANAGER->findImage("cursor")->render(getMemDC(), _ptMouse.x - 32, _ptMouse.y - 32);
	IMAGEMANAGER->findImage("cursor")->alphaRender(getMemDC(), _ptMouse.x - 32, _ptMouse.y - 32, 100);

	
//=============================================================
	//������� ������ ȭ��DC�� �׸��� (�̰͵� ������ �׳� �α�)
	this->getBackBuffer()->render(getHDC());
}

void mainGame::volumeControl()
{
	if (INPUT->GetKeyDown(VK_ADD) || INPUT->GetKeyDown(VK_OEM_PLUS))
	{
		if (volume < 1.f)
		{
			volume += .05f;
			SOUNDMANAGER->setBackGroundVolume("titleBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("mapToolBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("ingameBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("bossBGM", volume);
		}
	}
	if (INPUT->GetKeyDown(VK_SUBTRACT) || INPUT->GetKeyDown(VK_OEM_MINUS))
	{
		if (volume > .0f)
		{
			volume -= .05f;
			SOUNDMANAGER->setBackGroundVolume("titleBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("mapToolBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("ingameBGM", volume);
			SOUNDMANAGER->setBackGroundVolume("bossBGM", volume);
		}
	}
}

void mainGame::soundInit()
{
	
	//BGM_Sound
	SOUNDMANAGER->addSound("titleBGM", "Sound/TitleScreen.mp3", true, true);
	SOUNDMANAGER->addSound("mapToolBGM", "Sound/MapToolBGM.mp3", true, true);
	SOUNDMANAGER->addSound("ingameBGM", "Sound/IngameBGM.mp3", true, true);
	SOUNDMANAGER->addSound("bossBGM", "Sound/BossBGM.mp3", true, true);
	
	volume = 0.5f;
	
	//SFX_Sound
	SOUNDMANAGER->addSound("playerHit", "Sound/player/hit.mp3");
	SOUNDMANAGER->addSound("playerNomalDash", "Sound/player/NomalDash.mp3");
	SOUNDMANAGER->addSound("playerFoot", "Sound/player/footstep.mp3");
	//SOUNDMANAGER->addSound("playerNomalDash", "Sound/player/.mp3");
	//SOUNDMANAGER->addSound("playerNomalDash", "Sound/player/.mp3");
	

}
