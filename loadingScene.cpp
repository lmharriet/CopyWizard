#include "stdafx.h"
#include "loadingScene.h"

HRESULT loadingScene::init()
{

	//로딩클래스 초기화
	currentLoading = PLAYERDATA->getCurrentStage();
	if (_loading[STARTSTAGE] == nullptr)
	{
		PLAYERDATA->init();
		currentLoading = STARTSTAGE;
	}
	switch (currentLoading)
	{
	case STARTSTAGE:
		_loading[STARTSTAGE] = make_unique<loading>();
		_loading[STARTSTAGE]->init("LoadingBackground", "resource/UI/loadingBackground.bmp", WINSIZEX, WINSIZEY);
		break;
	case MIDDLESTAGE:
		_loading[MIDDLESTAGE] = make_unique<loading>();
		_loading[MIDDLESTAGE]->init("LoadingBackground1", "resource/UI/loadingBG1.bmp", WINSIZEX, WINSIZEY);
		break;
	case FINALSTAGE:
		_loading[FINALSTAGE] = make_unique<loading>();
		_loading[FINALSTAGE]->init("LoadingBackground2", "resource/UI/loadingBG2.bmp", WINSIZEX, WINSIZEY);
		break;
	}


	if (currentLoading == STAGEMAX)
		currentLoading = FINALSTAGE;

	//로딩 이미지 및 사운드 초기화
	this->loadingImage();
	this->loadingSound();
	
	return S_OK;
}

void loadingScene::release()
{
	//로딩클래스 해제
	if (_loading != nullptr)
	{
		_loading->release();
	}
}

void loadingScene::update()
{
	//로딩클래스 업데이트
	_loading[currentLoading]->update();

	//로딩완료후 화면 변경

	switch (currentLoading)
	{
	case STARTSTAGE:
		if (_loading[currentLoading]->loadingDone())
		{
			currentLoading++;
			SCENEMANAGER->loadScene("인게임");
		}
		break;
	case MIDDLESTAGE:
		if (_loading[currentLoading]->loadingDone())
		{
			currentLoading++;
			SCENEMANAGER->loadScene("보스방");
		}
		break;
	case FINALSTAGE:
		if (_loading[currentLoading]->loadingDone())
		{
			currentLoading = 0;
			SCENEMANAGER->loadScene("최종보스방");
		}
		break;
	}


}

void loadingScene::render()
{
	//로딩클래스 렌더
	_loading[currentLoading]->render();

}

void loadingScene::loadingImage()
{
	switch (currentLoading)
	{
	case STARTSTAGE:
		//플레이어 이미지
	//_loading->loadFrameImage("playerFrame", "resource/player/playerFrame_small1.bmp", 1000, 2500, 10, 25);  loading.cpp -> init에 있습니다. 모르겠으면 재윤한테 이야기하기하세요 
		IMAGEMANAGER->addFrameImage("PlayerAttackCircle", "resource/player/PlayerAttackCircle1.bmp", 3600, 100, 36, 1);
		IMAGEMANAGER->addFrameImage("meteor", "resource/player/meteor.bmp", 1200, 250, 6, 1);
		IMAGEMANAGER->addFrameImage("flame", "resource/player/flame1.bmp", 4096, 128, 32, 1);
		IMAGEMANAGER->addFrameImage("flameStrike", "resource/player/flameStrike1.bmp", 1707, 171, 10, 1);
		IMAGEMANAGER->addFrameImage("playerFrame", "resource/player/playerSpearFrame.bmp", 400, 400, 4, 4);
		IMAGEMANAGER->addFrameImage("playerSpearFrame", "resource/player/playerSpearFrame.bmp", 400, 400, 4, 4);
		
		IMAGEMANAGER->addFrameImage("summoner", "resource/enemy/SummonerSource.bmp", 500, 800, 5, 8);
		IMAGEMANAGER->addFrameImage("golem", "resource/enemy/Golem.bmp", 1080, 1050, 6, 5);
		IMAGEMANAGER->addFrameImage("knight", "resource/enemy/knight.bmp", 650, 1123, 6, 8);
		IMAGEMANAGER->addFrameImage("golemHit", "resource/enemy/golemHit.bmp", 1080, 630, 6, 3);
		IMAGEMANAGER->addFrameImage("ghoul", "resource/enemy/ghoul.bmp", 800, 933, 6, 7);
		IMAGEMANAGER->addFrameImage("ghoulLarge", "resource/enemy/ghoulKing.bmp", 1050, 1500, 7, 10);
		IMAGEMANAGER->addFrameImage("ghoulLargeHit", "resource/enemy/ghoulKingHit.bmp", 1050, 1500, 7, 10);
		
		IMAGEMANAGER->addFrameImage("smallSlash", "resource/enemyEffect/smallSlash.bmp", 600, 1200, 3, 6);
		IMAGEMANAGER->addFrameImage("knightSlashL", "resource/enemyEffect/knightSlashL.bmp", 246, 82, 3, 1);
		IMAGEMANAGER->addFrameImage("knightSlashR", "resource/enemyEffect/knightSlashR.bmp", 246, 82, 3, 1);
		IMAGEMANAGER->addFrameImage("knightSlashUp", "resource/enemyEffect/knightSlashUp.bmp", 246, 82, 3, 1);
		IMAGEMANAGER->addFrameImage("knightSlashDown", "resource/enemyEffect/knightSlashDown.bmp", 246, 82, 3, 1);
		IMAGEMANAGER->addFrameImage("stoneFly", "resource/enemyEffect/stoneFrame.bmp", 144, 72, 2, 1);
		IMAGEMANAGER->addImage("IceState", "resource/player/isIce.bmp", 122, 177, true, RGB(255, 0, 255));

		for (int i = 0; i < 200; i++)
		{
			
			if (i < 40)
			{
				char str[50] = "맵 만드는 중 ...";
				_loading[currentLoading]->loadImage(str, 100, 100);
			}
			else if (i < 80)
			{
				char str[50] = "몬스터 생성 중 ...";
				_loading[currentLoading]->loadImage(str, 100, 100);

			}
			else if (i < 120)
			{
				char str[50] =  "소리를 불러오는 중 ...";
				_loading[currentLoading]->loadImage(str, 100, 100);
			}
			else if (i < 160)
			{
				char str[50] =  "어둠을 걷어내는 중 ...";
				_loading[currentLoading]->loadImage(str, 100, 100);
			}
			else 
			{
				char str[50] = "손님맞이 방정리 중 ...";
				_loading[currentLoading]->loadImage(str, 100, 100);
			}

		}
		break;
	case MIDDLESTAGE:
		for (int i = 0; i < 200; i++)
		{
			if (i < 40)
			{
				char str[50] = "아틀라스 생성하는중 ...";
				_loading[currentLoading]->loadImage(str, 100, 100);
			}
			else if (i < 80)
			{
				char str[50] = "맵만드는중 ...";
				_loading[currentLoading]->loadImage(str, 100, 100);

			}
			else if (i < 120)
			{
				char str[50] = "소리를 불러오는 중 ...";
				_loading[currentLoading]->loadImage(str, 100, 100);
			}
			else if (i < 160)
			{
				char str[50] = "어둠을 걷어내는 중 ...";
				_loading[currentLoading]->loadImage(str, 100, 100);
			}
			else
			{
				char str[50] = "아틀라스의 방정리 중 ...";
				_loading[currentLoading]->loadImage(str, 100, 100);
			}
			
		}
		break;
	case FINALSTAGE:
		for (int i = 0; i < 200; i++)
		{
			if (i < 40)
			{
				char str[50] = "맵만드는중 ...";
				_loading[currentLoading]->loadImage(str, 100, 100);
			}
			else if (i < 80)
			{
				char str[50] = "마스터 수라 생성중 ..";
				_loading[currentLoading]->loadImage(str, 100, 100);

			}
			else if (i < 120)
			{
				char str[50] = "소리를 불러오는 중 ...";
				_loading[currentLoading]->loadImage(str, 100, 100);
			}
			else if (i < 160)
			{
				char str[50] = "마스터 수라의 성 정리 중 ...";
				_loading[currentLoading]->loadImage(str, 100, 100);
			}
			else
			{
				char str[50] = "당근밭주인 초코우유 사오는 중 ... ";
				_loading[currentLoading]->loadImage(str, 100, 100);
			}

		}
		
		break;

	}



}

void loadingScene::loadingSound()
{
	switch (currentLoading)
	{
	case STARTSTAGE:
		//플레이어 사운드
		SOUNDMANAGER->addSound("playerHit", "Sound/player/hit.mp3");
		SOUNDMANAGER->addSound("playerNomalDash", "Sound/player/nomalDash.mp3");
		SOUNDMANAGER->addSound("playerfireDash", "Sound/player/fireDash.mp3");
		SOUNDMANAGER->addSound("playerFoot", "Sound/player/footstep.mp3");
		SOUNDMANAGER->addSound("blazeFire", "Sound/player/blaze_fire.mp3");
		SOUNDMANAGER->addSound("blazeExp", "Sound/player/blaze_exp.mp3");
		SOUNDMANAGER->addSound("RagingInfernoExp", "Sound/player/RagingInferno_exp.mp3");
		SOUNDMANAGER->addSound("RagingInfernoFire", "Sound/player/RagingInferno_fire.mp3");
		SOUNDMANAGER->addSound("meteoFire0", "Sound/player/meteo_fire0.mp3");
		SOUNDMANAGER->addSound("meteoFire1", "Sound/player/meteo_fire1.mp3");
		SOUNDMANAGER->addSound("meteoFire2", "Sound/player/meteo_fire2.mp3");
		SOUNDMANAGER->addSound("meteoFire3", "Sound/player/meteo_fire3.mp3");
		SOUNDMANAGER->addSound("meteoExp0", "Sound/player/meteo_full0.mp3");
		SOUNDMANAGER->addSound("meteoExp1", "Sound/player/meteo_full1.mp3");
		SOUNDMANAGER->addSound("meteoExp2", "Sound/player/meteo_full2.mp3");
		SOUNDMANAGER->addSound("meteoExp3", "Sound/player/meteo_full3.mp3");
		
		
		SOUNDMANAGER->addSound("healBall", "Sound/healBall.mp3");
		SOUNDMANAGER->addSound("portalWarp", "Sound/portalWarp.mp3");
		SOUNDMANAGER->addSound("coinGet", "Sound/coinGet.mp3");
		SOUNDMANAGER->addSound("invenOpen", "Sound/UI/inventoryON.mp3");
		SOUNDMANAGER->addSound("invenClose", "Sound/UI/inventoryOFF.mp3");
		SOUNDMANAGER->addSound("npcBuyFirst", "Sound/UI/npcBuy.mp3");
		SOUNDMANAGER->addSound("npcBuy", "Sound/UI/npcBuy02.mp3");
		SOUNDMANAGER->addSound("invenClose", "Sound/UI/inventoryOFF.mp3");
		SOUNDMANAGER->addSound("shieldON", "Sound/UI/shieldON.mp3");
		SOUNDMANAGER->addSound("shieldOFF", "Sound/UI/shieldOFF.mp3");
		SOUNDMANAGER->addSound("npcNox", "Sound/UI/npc_nox.mp3");
		SOUNDMANAGER->addSound("BOX_appear", "Sound/UI/BOX_appear.mp3");
		SOUNDMANAGER->addSound("portalIN", "Sound/UI/portalIN.mp3");
		SOUNDMANAGER->addSound("portalOUT", "Sound/UI/portalOUT.mp3");
		
		
		SOUNDMANAGER->addSound("golemAtk", "Sound/golem_atk.mp3");
		SOUNDMANAGER->addSound("golemWalkL", "Sound/golem_walkL.mp3");
		SOUNDMANAGER->addSound("golemWalkR", "Sound/golem_walkR.mp3");
		SOUNDMANAGER->addSound("golemDie", "Sound/golem_die.mp3");
		SOUNDMANAGER->addSound("golemHit", "Sound/golem_hit.mp3");
		
		SOUNDMANAGER->addSound("knightAtk0", "Sound/knight_atk0.mp3");
		SOUNDMANAGER->addSound("knightAtk1", "Sound/knight_atk1.mp3");
		SOUNDMANAGER->addSound("knightAtk2", "Sound/knight_atk2.mp3");
		SOUNDMANAGER->addSound("knightAtk3", "Sound/knight_atk3.mp3");
		SOUNDMANAGER->addSound("knightWalkL", "Sound/knight_walkL.mp3");
		SOUNDMANAGER->addSound("knightWalkR", "Sound/knignt_walkR.mp3");
		SOUNDMANAGER->addSound("knightDie", "Sound/knight_die.mp3");
		SOUNDMANAGER->addSound("knightHit0", "Sound/knight_hit0.mp3");
		SOUNDMANAGER->addSound("knightHit1", "Sound/knight_hit1.mp3");
		
		SOUNDMANAGER->addSound("summonerCasting", "Sound/summoner_casting.mp3");
		SOUNDMANAGER->addSound("summonerFire", "Sound/summoner_fire.mp3");
		SOUNDMANAGER->addSound("summonerAtk", "Sound/summoner_atk.mp3");
		
		SOUNDMANAGER->addSound("ghoulHit0", "Sound/ghoulHit0.mp3");
		SOUNDMANAGER->addSound("ghoulHit1", "Sound/ghoulHit1.mp3");
		SOUNDMANAGER->addSound("ghoulDie", "Sound/ghoulDie.mp3");
		SOUNDMANAGER->addSound("ghoulATK", "Sound/ghoulATK.mp3");
		SOUNDMANAGER->addSound("ghoulWalk", "Sound/ghoulWalk.mp3");

		break;
	case MIDDLESTAGE:
		//보스  
		SOUNDMANAGER->addSound("bossDrillSFX", "Sound/bossDrillSFX.mp3");
		SOUNDMANAGER->addSound("bossDrillVO", "Sound/bossDrillVO.mp3");
		SOUNDMANAGER->addSound("bossJumpSFX", "Sound/bossJumpSFX.mp3");
		SOUNDMANAGER->addSound("bossJumpVO", "Sound/bossJumpVO.mp3");
		SOUNDMANAGER->addSound("bossNiddleSFX", "Sound/bossNiddleSFX.mp3");
		SOUNDMANAGER->addSound("bossNiddleSFX_END", "Sound/bossNiddleSFX_END.mp3");
		SOUNDMANAGER->addSound("bossPunchSFX", "Sound/bossPunchSFX.mp3");
		SOUNDMANAGER->addSound("bossPunchSFX02", "Sound/bossPunchSFX02.mp3");
		SOUNDMANAGER->addSound("bossPunchVO", "Sound/bossPunchVO.mp3");
		SOUNDMANAGER->addSound("bossPunchEXP", "Sound/bossPunchEXP.mp3");
		SOUNDMANAGER->addSound("bossWallSFX", "Sound/bossWallSFX.mp3");
		SOUNDMANAGER->addSound("bossWallVO", "Sound/bossWallVO.mp3");
		SOUNDMANAGER->addSound("bossFinalATK_start", "Sound/bossFinalATK_start.mp3");
		SOUNDMANAGER->addSound("bossFinalATK_end", "Sound/bossFinalATK_end.mp3");
		SOUNDMANAGER->addSound("bossFinalATK_up", "Sound/bossFinalATK_up.mp3");
		SOUNDMANAGER->addSound("bossDie", "Sound/bossDie.mp3");
		SOUNDMANAGER->addSound("CutSceneSFX", "Sound/CutSceneSFX.mp3");
		SOUNDMANAGER->addSound("CutSceneVO", "Sound/CutSceneVO.mp3");
		break;
	case FINALSTAGE:
		SOUNDMANAGER->addSound("finalBossSpawn", "Sound/finalBoss/finalBoss_spawn.mp3");
		SOUNDMANAGER->addSound("castleAmb", "Sound/finalBoss/castleAmb.mp3");
		SOUNDMANAGER->addSound("fnialBoss_fireBall", "Sound/finalBoss/finalBoss_fireBall.mp3");
		SOUNDMANAGER->addSound("fnialBoss_iceSpear", "Sound/finalBoss/finalBoss_iceSpearAppear.mp3");
		SOUNDMANAGER->addSound("fnialBoss_wind", "Sound/finalBoss/finalBoss_wind.mp3");

		break;
	}

}
