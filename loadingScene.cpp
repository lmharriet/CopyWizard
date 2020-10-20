#include "stdafx.h"
#include "loadingScene.h"

HRESULT loadingScene::init()
{
	//로딩클래스 초기화
	_loading = new loading;
	_loading->init();

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
		SAFE_DELETE(_loading);
	}
}

void loadingScene::update()
{
	//로딩클래스 업데이트
	_loading->update();

	//로딩완료후 화면 변경
	if (_loading->loadingDone())
	{
		SCENEMANAGER->loadScene("인게임");
	}

}

void loadingScene::render()
{
	//로딩클래스 렌더
	_loading->render();

	textOut(getMemDC(), 10, 10, "로딩화면");
}

void loadingScene::loadingImage()
{
	_loading->loadFrameImage("summoner", "resource/enemy/SummonerSource.bmp", 500, 800, 5, 8);
	_loading->loadFrameImage("golem", "resource/enemy/Golem.bmp", 1080, 1050, 6, 5);
	_loading->loadFrameImage("knight", "resource/enemy/knight.bmp", 650, 1123, 6, 8);
	_loading->loadFrameImage("golemHit", "resource/enemy/golemHit.bmp", 1080, 630, 6, 3);
	_loading->loadFrameImage("ghoul", "resource/enemy/ghoul.bmp", 800, 933, 6, 7);
	_loading->loadFrameImage("ghoulLarge", "resource/enemy/ghoulKing.bmp", 1050, 1500, 7, 10);
	_loading->loadFrameImage("ghoulLargeHit", "resource/enemy/ghoulKingHit.bmp", 1050, 1500, 7, 10);
	//몬스터 이미지
	/*IMAGEMANAGER->addFrameImage("summoner", "resource/enemy/SummonerSource.bmp", 500, 800, 5, 8);
	IMAGEMANAGER->addFrameImage("golem", "resource/enemy/Golem.bmp", 1080, 1050, 6, 5);
	IMAGEMANAGER->addFrameImage("knight", "resource/enemy/knight.bmp", 650, 1123, 6, 8);
	IMAGEMANAGER->addFrameImage("golemHit", "resource/enemy/golemHit.bmp", 1080, 630, 6, 3);
	IMAGEMANAGER->addFrameImage("ghoul", "resource/enemy/ghoul.bmp", 800, 933, 6, 7);
	IMAGEMANAGER->addFrameImage("ghoulLarge", "resource/enemy/ghoulKing.bmp", 1050, 1500, 7, 10);
	IMAGEMANAGER->addFrameImage("ghoulLargeHit", "resource/enemy/ghoulKingHit.bmp", 1050, 1500, 7, 10);*/

	//스킬 이펙트 이미지
	_loading->loadFrameImage("smallSlash", "resource/enemyEffect/smallSlash.bmp", 600, 1200, 3, 6);
	_loading->loadFrameImage("knightSlashL", "resource/enemyEffect/knightSlashL.bmp", 246, 82, 3, 1);
	_loading->loadFrameImage("knightSlashR", "resource/enemyEffect/knightSlashR.bmp", 246, 82, 3, 1);
	_loading->loadFrameImage("knightSlashUp", "resource/enemyEffect/knightSlashUp.bmp", 246, 82, 3, 1);
	_loading->loadFrameImage("knightSlashDown", "resource/enemyEffect/knightSlashDown.bmp", 246, 82, 3, 1);
	_loading->loadFrameImage("stoneFly", "resource/enemyEffect/stoneFrame.bmp", 144, 72, 2, 1);

	/*IMAGEMANAGER->addFrameImage("smallSlash", "resource/enemyEffect/smallSlash.bmp", 600, 1200, 3, 6);
	IMAGEMANAGER->addFrameImage("knightSlashL", "resource/enemyEffect/knightSlashL.bmp", 246, 82, 3, 1);
	IMAGEMANAGER->addFrameImage("knightSlashR", "resource/enemyEffect/knightSlashR.bmp", 246, 82, 3, 1);
	IMAGEMANAGER->addFrameImage("knightSlashUp", "resource/enemyEffect/knightSlashUp.bmp", 246, 82, 3, 1);
	IMAGEMANAGER->addFrameImage("knightSlashDown", "resource/enemyEffect/knightSlashDown.bmp", 246, 82, 3, 1);
	IMAGEMANAGER->addFrameImage("stoneFly", "resource/enemyEffect/stoneFrame.bmp", 144, 72, 2, 1);*/



}

void loadingScene::loadingSound()
{

	_loading->loadSound("playerHit", "Sound/player/hit.mp3");
	_loading->loadSound("playerNomalDash", "Sound/player/nomalDash.mp3");
	_loading->loadSound("playerfireDash", "Sound/player/fireDash.mp3");
	_loading->loadSound("playerFoot", "Sound/player/footstep.mp3");
	_loading->loadSound("blazeFire", "Sound/player/blaze_fire.mp3");
	_loading->loadSound("blazeExp", "Sound/player/blaze_exp.mp3");
	_loading->loadSound("RagingInfernoExp", "Sound/player/RagingInferno_exp.mp3");
	_loading->loadSound("RagingInfernoFire", "Sound/player/RagingInferno_fire.mp3");
	_loading->loadSound("meteoFire0", "Sound/player/meteo_fire0.mp3");
	_loading->loadSound("meteoFire1", "Sound/player/meteo_fire1.mp3");
	_loading->loadSound("meteoFire2", "Sound/player/meteo_fire2.mp3");
	_loading->loadSound("meteoFire3", "Sound/player/meteo_fire3.mp3");
	_loading->loadSound("meteoExp0", "Sound/player/meteo_full0.mp3");
	_loading->loadSound("meteoExp1", "Sound/player/meteo_full1.mp3");
	_loading->loadSound("meteoExp2", "Sound/player/meteo_full2.mp3");
	_loading->loadSound("meteoExp3", "Sound/player/meteo_full3.mp3");

	_loading->loadSound("healBall", "Sound/healBall.mp3");
	_loading->loadSound("portalWarp", "Sound/portalWarp.mp3");
	_loading->loadSound("coinGet", "Sound/coinGet.mp3");
	_loading->loadSound("invenOpen", "Sound/UI/inventoryON.mp3");
	_loading->loadSound("invenClose", "Sound/UI/inventoryOFF.mp3");
	_loading->loadSound("npcBuyFirst", "Sound/UI/npcBuy.mp3");
	_loading->loadSound("npcBuy", "Sound/UI/npcBuy02.mp3");
	_loading->loadSound("invenClose", "Sound/UI/inventoryOFF.mp3");
	_loading->loadSound("shieldON", "Sound/UI/shieldON.mp3");
	_loading->loadSound("shieldOFF", "Sound/UI/shieldOFF.mp3");
	_loading->loadSound("npcNox", "Sound/UI/npc_nox.mp3");

	_loading->loadSound("golemAtk", "Sound/golem_atk.mp3");
	_loading->loadSound("golemWalkL", "Sound/golem_walkL.mp3");
	_loading->loadSound("golemWalkR", "Sound/golem_walkR.mp3");
	_loading->loadSound("golemDie", "Sound/golem_die.mp3");
	_loading->loadSound("golemHit", "Sound/golem_hit.mp3");

	_loading->loadSound("knightAtk0", "Sound/knight_atk0.mp3");
	_loading->loadSound("knightAtk1", "Sound/knight_atk1.mp3");
	_loading->loadSound("knightAtk2", "Sound/knight_atk2.mp3");
	_loading->loadSound("knightAtk3", "Sound/knight_atk3.mp3");
	_loading->loadSound("knightWalkL", "Sound/knight_walkL.mp3");
	_loading->loadSound("knightWalkR", "Sound/knignt_walkR.mp3");
	_loading->loadSound("knightDie", "Sound/knight_die.mp3");
	_loading->loadSound("knightHit0", "Sound/knight_hit0.mp3");
	_loading->loadSound("knightHit1", "Sound/knight_hit1.mp3");

	_loading->loadSound("summonerCasting", "Sound/summoner_casting.mp3");
	_loading->loadSound("summonerFire", "Sound/summoner_fire.mp3");
	_loading->loadSound("summonerAtk", "Sound/summoner_atk.mp3");

	_loading->loadSound("ghoulHit0", "Sound/ghoulHit0.mp3");
	_loading->loadSound("ghoulHit1", "Sound/ghoulHit1.mp3");
	_loading->loadSound("ghoulDie", "Sound/ghoulDie.mp3");
	_loading->loadSound("ghoulATK", "Sound/ghoulATK.mp3");
	_loading->loadSound("ghoulWalk", "Sound/ghoulWalk.mp3");

	_loading->loadSound("bossDrillSFX", "Sound/bossDrillSFX.mp3");
	_loading->loadSound("bossDrillVO", "Sound/bossDrillVO.mp3");
	_loading->loadSound("bossJumpSFX", "Sound/bossJumpSFX.mp3");
	_loading->loadSound("bossJumpVO", "Sound/bossJumpVO.mp3");
	_loading->loadSound("bossNiddleSFX", "Sound/bossNiddleSFX.mp3");
	_loading->loadSound("bossPunchSFX", "Sound/bossPunchSFX.mp3");
	_loading->loadSound("bossPunchVO", "Sound/bossPunchVO.mp3");
	_loading->loadSound("bossWallSFX", "Sound/bossWallSFX.mp3");
	_loading->loadSound("bossWallVO", "Sound/bossWallVO.mp3");
	_loading->loadSound("CutSceneSFX", "Sound/CutSceneSFX.mp3");
	_loading->loadSound("CutSceneVO", "Sound/CutSceneVO.mp3");
	






	////player
	//SOUNDMANAGER->addSound("playerHit", "Sound/player/hit.mp3");
	//SOUNDMANAGER->addSound("playerNomalDash", "Sound/player/nomalDash.mp3");
	//SOUNDMANAGER->addSound("playerfireDash", "Sound/player/fireDash.mp3");
	//SOUNDMANAGER->addSound("playerFoot", "Sound/player/footstep.mp3");
	//SOUNDMANAGER->addSound("blazeFire", "Sound/player/blaze_fire.mp3");
	//SOUNDMANAGER->addSound("blazeExp", "Sound/player/blaze_exp.mp3");
	//SOUNDMANAGER->addSound("RagingInfernoExp", "Sound/player/RagingInferno_exp.mp3");
	//SOUNDMANAGER->addSound("RagingInfernoFire", "Sound/player/RagingInferno_fire.mp3");
	//SOUNDMANAGER->addSound("meteoFire0", "Sound/player/meteo_fire0.mp3");
	//SOUNDMANAGER->addSound("meteoFire1", "Sound/player/meteo_fire1.mp3");
	//SOUNDMANAGER->addSound("meteoFire2", "Sound/player/meteo_fire2.mp3");
	//SOUNDMANAGER->addSound("meteoFire3", "Sound/player/meteo_fire3.mp3");
	//SOUNDMANAGER->addSound("meteoExp0", "Sound/player/meteo_full0.mp3");
	//SOUNDMANAGER->addSound("meteoExp1", "Sound/player/meteo_full1.mp3");
	//SOUNDMANAGER->addSound("meteoExp2", "Sound/player/meteo_full2.mp3");
	//SOUNDMANAGER->addSound("meteoExp3", "Sound/player/meteo_full3.mp3");

	////UI
	//SOUNDMANAGER->addSound("healBall", "Sound/healBall.mp3");
	//SOUNDMANAGER->addSound("portalWarp", "Sound/portalWarp.mp3");
	//SOUNDMANAGER->addSound("coinGet", "Sound/coinGet.mp3");
	//SOUNDMANAGER->addSound("invenOpen", "Sound/UI/inventoryON.mp3");
	//SOUNDMANAGER->addSound("invenClose", "Sound/UI/inventoryOFF.mp3");
	//SOUNDMANAGER->addSound("npcBuyFirst", "Sound/UI/npcBuy.mp3");
	//SOUNDMANAGER->addSound("npcBuy", "Sound/UI/npcBuy02.mp3");
	//SOUNDMANAGER->addSound("invenClose", "Sound/UI/inventoryOFF.mp3");
	//SOUNDMANAGER->addSound("shieldON", "Sound/UI/shieldON.mp3");
	//SOUNDMANAGER->addSound("shieldOFF", "Sound/UI/shieldOFF.mp3");
	//SOUNDMANAGER->addSound("npcNox", "Sound/UI/npc_nox.mp3");

	////몬스터
	//SOUNDMANAGER->addSound("golemAtk", "Sound/golem_atk.mp3");
	//SOUNDMANAGER->addSound("golemWalkL", "Sound/golem_walkL.mp3");
	//SOUNDMANAGER->addSound("golemWalkR", "Sound/golem_walkR.mp3");
	//SOUNDMANAGER->addSound("golemDie", "Sound/golem_die.mp3");
	//SOUNDMANAGER->addSound("golemHit", "Sound/golem_hit.mp3");
	//
	//SOUNDMANAGER->addSound("knightAtk0", "Sound/knight_atk0.mp3");
	//SOUNDMANAGER->addSound("knightAtk1", "Sound/knight_atk1.mp3");
	//SOUNDMANAGER->addSound("knightAtk2", "Sound/knight_atk2.mp3");
	//SOUNDMANAGER->addSound("knightAtk3", "Sound/knight_atk3.mp3");
	//SOUNDMANAGER->addSound("knightWalkL", "Sound/knight_walkL.mp3");
	//SOUNDMANAGER->addSound("knightWalkR", "Sound/knignt_walkR.mp3");
	//SOUNDMANAGER->addSound("knightDie", "Sound/knight_die.mp3");
	//SOUNDMANAGER->addSound("knightHit0", "Sound/knight_hit0.mp3");
	//SOUNDMANAGER->addSound("knightHit1", "Sound/knight_hit1.mp3");

	//SOUNDMANAGER->addSound("summonerCasting", "Sound/summoner_casting.mp3");
	//SOUNDMANAGER->addSound("summonerFire", "Sound/summoner_fire.mp3");
	//SOUNDMANAGER->addSound("summonerAtk", "Sound/summoner_atk.mp3");
	//
	//SOUNDMANAGER->addSound("ghoulHit0", "Sound/ghoulHit0.mp3");
	//SOUNDMANAGER->addSound("ghoulHit1", "Sound/ghoulHit1.mp3");
	//SOUNDMANAGER->addSound("ghoulDie", "Sound/ghoulDie.mp3");
	//SOUNDMANAGER->addSound("ghoulATK", "Sound/ghoulATK.mp3");
	//SOUNDMANAGER->addSound("ghoulWalk", "Sound/ghoulWalk.mp3");


	////boss
	//SOUNDMANAGER->addSound("bossDrillSFX", "Sound/bossDrillSFX.mp3");
	//SOUNDMANAGER->addSound("bossDrillVO", "Sound/bossDrillVO.mp3");
	//SOUNDMANAGER->addSound("bossJumpSFX", "Sound/bossJumpSFX.mp3");
	//SOUNDMANAGER->addSound("bossJumpVO", "Sound/bossJumpVO.mp3");
	//SOUNDMANAGER->addSound("bossNiddleSFX", "Sound/bossNiddleSFX.mp3");
	//SOUNDMANAGER->addSound("bossPunchSFX", "Sound/bossPunchSFX.mp3");
	//SOUNDMANAGER->addSound("bossPunchVO", "Sound/bossPunchVO.mp3");
	//SOUNDMANAGER->addSound("bossWallSFX", "Sound/bossWallSFX.mp3");
	//SOUNDMANAGER->addSound("bossWallVO", "Sound/bossWallVO.mp3");
	//SOUNDMANAGER->addSound("CutSceneSFX", "Sound/CutSceneSFX.mp3");
	//SOUNDMANAGER->addSound("CutSceneVO", "Sound/CutSceneVO.mp3");
}
