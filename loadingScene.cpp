#include "stdafx.h"
#include "loadingScene.h"

HRESULT loadingScene::init()
{

	//로딩클래스 초기화
	currentLoading = PLAYERDATA->getCurrentStage();
	if (_loading[STARTSTAGE] == nullptr)
		currentLoading = STARTSTAGE;

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
		_loading[currentLoading]->loadFrameImage("PlayerAttackCircle", "resource/player/PlayerAttackCircle1.bmp", 3600, 100, 36, 1);
		_loading[currentLoading]->loadFrameImage("meteor", "resource/player/meteor.bmp", 1200, 250, 6, 1);
		_loading[currentLoading]->loadFrameImage("flame", "resource/player/flame1.bmp", 4096, 128, 32, 1);
		_loading[currentLoading]->loadFrameImage("flameStrike", "resource/player/flameStrike1.bmp", 1707, 171, 10, 1);
		_loading[currentLoading]->loadFrameImage("playerFrame", "resource/player/playerSpearFrame.bmp", 400, 400, 4, 4);
		_loading[currentLoading]->loadFrameImage("playerSpearFrame", "resource/player/playerSpearFrame.bmp", 400, 400, 4, 4);


		//몬스터 이미지
		_loading[currentLoading]->loadFrameImage("summoner", "resource/enemy/SummonerSource.bmp", 500, 800, 5, 8);
		_loading[currentLoading]->loadFrameImage("golem", "resource/enemy/Golem.bmp", 1080, 1050, 6, 5);
		_loading[currentLoading]->loadFrameImage("knight", "resource/enemy/knight.bmp", 650, 1123, 6, 8);
		_loading[currentLoading]->loadFrameImage("golemHit", "resource/enemy/golemHit.bmp", 1080, 630, 6, 3);
		_loading[currentLoading]->loadFrameImage("ghoul", "resource/enemy/ghoul.bmp", 800, 933, 6, 7);
		_loading[currentLoading]->loadFrameImage("ghoulLarge", "resource/enemy/ghoulKing.bmp", 1050, 1500, 7, 10);
		_loading[currentLoading]->loadFrameImage("ghoulLargeHit", "resource/enemy/ghoulKingHit.bmp", 1050, 1500, 7, 10);


		//스킬 이펙트 이미지
		_loading[currentLoading]->loadFrameImage("smallSlash", "resource/enemyEffect/smallSlash.bmp", 600, 1200, 3, 6);
		_loading[currentLoading]->loadFrameImage("knightSlashL", "resource/enemyEffect/knightSlashL.bmp", 246, 82, 3, 1);
		_loading[currentLoading]->loadFrameImage("knightSlashR", "resource/enemyEffect/knightSlashR.bmp", 246, 82, 3, 1);
		_loading[currentLoading]->loadFrameImage("knightSlashUp", "resource/enemyEffect/knightSlashUp.bmp", 246, 82, 3, 1);
		_loading[currentLoading]->loadFrameImage("knightSlashDown", "resource/enemyEffect/knightSlashDown.bmp", 246, 82, 3, 1);
		_loading[currentLoading]->loadFrameImage("stoneFly", "resource/enemyEffect/stoneFrame.bmp", 144, 72, 2, 1);
		_loading[currentLoading]->loadImage("IceState", "resource/player/isIce.bmp", 122, 177, true, RGB(255, 0, 255));
		break;
	case MIDDLESTAGE:
		for (int i = 0; i < 300; i++)
		{
			char str[20];
			sprintf(str, "bossImg%d", i);
			_loading[MIDDLESTAGE]->loadImage(str, 100, 100);
		}
		break;
	case FINALSTAGE:
		for (int i = 0; i < 300; i++)
		{
			char str[20];
			sprintf(str, "bossImg%d", i);
			_loading[FINALSTAGE]->loadImage(str, 100, 100);
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
		_loading[STARTSTAGE]->loadSound("playerHit", "Sound/player/hit.mp3");
		_loading[STARTSTAGE]->loadSound("playerNomalDash", "Sound/player/nomalDash.mp3");
		_loading[STARTSTAGE]->loadSound("playerfireDash", "Sound/player/fireDash.mp3");
		_loading[STARTSTAGE]->loadSound("playerFoot", "Sound/player/footstep.mp3");
		_loading[STARTSTAGE]->loadSound("blazeFire", "Sound/player/blaze_fire.mp3");
		_loading[STARTSTAGE]->loadSound("blazeExp", "Sound/player/blaze_exp.mp3");
		_loading[STARTSTAGE]->loadSound("RagingInfernoExp", "Sound/player/RagingInferno_exp.mp3");
		_loading[STARTSTAGE]->loadSound("RagingInfernoFire", "Sound/player/RagingInferno_fire.mp3");
		_loading[STARTSTAGE]->loadSound("meteoFire0", "Sound/player/meteo_fire0.mp3");
		_loading[STARTSTAGE]->loadSound("meteoFire1", "Sound/player/meteo_fire1.mp3");
		_loading[STARTSTAGE]->loadSound("meteoFire2", "Sound/player/meteo_fire2.mp3");
		_loading[STARTSTAGE]->loadSound("meteoFire3", "Sound/player/meteo_fire3.mp3");
		_loading[STARTSTAGE]->loadSound("meteoExp0", "Sound/player/meteo_full0.mp3");
		_loading[STARTSTAGE]->loadSound("meteoExp1", "Sound/player/meteo_full1.mp3");
		_loading[STARTSTAGE]->loadSound("meteoExp2", "Sound/player/meteo_full2.mp3");
		_loading[STARTSTAGE]->loadSound("meteoExp3", "Sound/player/meteo_full3.mp3");

		//UI 사운드
		_loading[STARTSTAGE]->loadSound("healBall", "Sound/healBall.mp3");
		_loading[STARTSTAGE]->loadSound("portalWarp", "Sound/portalWarp.mp3");
		_loading[STARTSTAGE]->loadSound("coinGet", "Sound/coinGet.mp3");
		_loading[STARTSTAGE]->loadSound("invenOpen", "Sound/UI/inventoryON.mp3");
		_loading[STARTSTAGE]->loadSound("invenClose", "Sound/UI/inventoryOFF.mp3");
		_loading[STARTSTAGE]->loadSound("npcBuyFirst", "Sound/UI/npcBuy.mp3");
		_loading[STARTSTAGE]->loadSound("npcBuy", "Sound/UI/npcBuy02.mp3");
		_loading[STARTSTAGE]->loadSound("invenClose", "Sound/UI/inventoryOFF.mp3");
		_loading[STARTSTAGE]->loadSound("shieldON", "Sound/UI/shieldON.mp3");
		_loading[STARTSTAGE]->loadSound("shieldOFF", "Sound/UI/shieldOFF.mp3");
		_loading[STARTSTAGE]->loadSound("npcNox", "Sound/UI/npc_nox.mp3");
		_loading[STARTSTAGE]->loadSound("BOX_appear", "Sound/UI/BOX_appear.mp3");
		_loading[STARTSTAGE]->loadSound("portalIN", "Sound/UI/portalIN.mp3");
		_loading[STARTSTAGE]->loadSound("portalOUT", "Sound/UI/portalOUT.mp3");

		//몬스터 사운드
		_loading[STARTSTAGE]->loadSound("golemAtk", "Sound/golem_atk.mp3");
		_loading[STARTSTAGE]->loadSound("golemWalkL", "Sound/golem_walkL.mp3");
		_loading[STARTSTAGE]->loadSound("golemWalkR", "Sound/golem_walkR.mp3");
		_loading[STARTSTAGE]->loadSound("golemDie", "Sound/golem_die.mp3");
		_loading[STARTSTAGE]->loadSound("golemHit", "Sound/golem_hit.mp3");

		_loading[STARTSTAGE]->loadSound("knightAtk0", "Sound/knight_atk0.mp3");
		_loading[STARTSTAGE]->loadSound("knightAtk1", "Sound/knight_atk1.mp3");
		_loading[STARTSTAGE]->loadSound("knightAtk2", "Sound/knight_atk2.mp3");
		_loading[STARTSTAGE]->loadSound("knightAtk3", "Sound/knight_atk3.mp3");
		_loading[STARTSTAGE]->loadSound("knightWalkL", "Sound/knight_walkL.mp3");
		_loading[STARTSTAGE]->loadSound("knightWalkR", "Sound/knignt_walkR.mp3");
		_loading[STARTSTAGE]->loadSound("knightDie", "Sound/knight_die.mp3");
		_loading[STARTSTAGE]->loadSound("knightHit0", "Sound/knight_hit0.mp3");
		_loading[STARTSTAGE]->loadSound("knightHit1", "Sound/knight_hit1.mp3");

		_loading[STARTSTAGE]->loadSound("summonerCasting", "Sound/summoner_casting.mp3");
		_loading[STARTSTAGE]->loadSound("summonerFire", "Sound/summoner_fire.mp3");
		_loading[STARTSTAGE]->loadSound("summonerAtk", "Sound/summoner_atk.mp3");

		_loading[STARTSTAGE]->loadSound("ghoulHit0", "Sound/ghoulHit0.mp3");
		_loading[STARTSTAGE]->loadSound("ghoulHit1", "Sound/ghoulHit1.mp3");
		_loading[STARTSTAGE]->loadSound("ghoulDie", "Sound/ghoulDie.mp3");
		_loading[STARTSTAGE]->loadSound("ghoulATK", "Sound/ghoulATK.mp3");
		_loading[STARTSTAGE]->loadSound("ghoulWalk", "Sound/ghoulWalk.mp3");


		break;
	case MIDDLESTAGE:
		//보스  
		_loading[MIDDLESTAGE]->loadSound("bossDrillSFX", "Sound/bossDrillSFX.mp3");
		_loading[MIDDLESTAGE]->loadSound("bossDrillVO", "Sound/bossDrillVO.mp3");
		_loading[MIDDLESTAGE]->loadSound("bossJumpSFX", "Sound/bossJumpSFX.mp3");
		_loading[MIDDLESTAGE]->loadSound("bossJumpVO", "Sound/bossJumpVO.mp3");
		_loading[MIDDLESTAGE]->loadSound("bossNiddleSFX", "Sound/bossNiddleSFX.mp3");
		_loading[MIDDLESTAGE]->loadSound("bossNiddleSFX_END", "Sound/bossNiddleSFX_END.mp3");
		_loading[MIDDLESTAGE]->loadSound("bossPunchSFX", "Sound/bossPunchSFX.mp3");
		_loading[MIDDLESTAGE]->loadSound("bossPunchSFX02", "Sound/bossPunchSFX02.mp3");
		_loading[MIDDLESTAGE]->loadSound("bossPunchVO", "Sound/bossPunchVO.mp3");
		_loading[MIDDLESTAGE]->loadSound("bossPunchEXP", "Sound/bossPunchEXP.mp3");
		_loading[MIDDLESTAGE]->loadSound("bossWallSFX", "Sound/bossWallSFX.mp3");
		_loading[MIDDLESTAGE]->loadSound("bossWallVO", "Sound/bossWallVO.mp3");
		_loading[MIDDLESTAGE]->loadSound("bossFinalATK_start", "Sound/bossFinalATK_start.mp3");
		_loading[MIDDLESTAGE]->loadSound("bossFinalATK_end", "Sound/bossFinalATK_end.mp3");
		_loading[MIDDLESTAGE]->loadSound("bossFinalATK_up", "Sound/bossFinalATK_up.mp3");
		_loading[MIDDLESTAGE]->loadSound("bossDie", "Sound/bossDie.mp3");
		_loading[MIDDLESTAGE]->loadSound("CutSceneSFX", "Sound/CutSceneSFX.mp3");
		_loading[MIDDLESTAGE]->loadSound("CutSceneVO", "Sound/CutSceneVO.mp3");
		break;
	case FINALSTAGE:


		break;
	}

}
