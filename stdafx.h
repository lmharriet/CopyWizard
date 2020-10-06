// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//
#pragma once

#include <SDKDDKVer.h>

//디버깅용 (주석치면 콘솔창이 사라진다)
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <Windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
// C++ 런타임 헤더 파일입니다.
#include <iostream>
using namespace std;

// 자주사용하는 STL
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <unordered_map>

//=============================================================
//	## 내가 만든 헤더파일을 이곳에 추가한다 ##
//=============================================================
#include "commonMacroFunction.h"
#include "input.h"
#include "random.h"
#include "imageManager.h"
#include "util.h"
using namespace MY_UTIL;
#include "playerData.h"
#include "sceneManager.h"
#include "soundManager.h"
#include "cameraManager.h"
#include "timeManager.h"
#include "particleManager.h"
#include "uiManager.h"
#include "effectManager.h"
#include "itemManager.h"
#include "dropManager.h"
#include "bossManager.h"
//#include "astarManager.h"

//=============================================================
//	## 싱글톤을 추가한다 ##
//=============================================================
#define INPUT input::getSingleton() //세미콜론 찍으면 작동안함
#define RANDOM random::getSingleton()
#define IMAGEMANAGER imageManager::getSingleton()
#define PLAYERDATA playerData::getSingleton()
#define SCENEMANAGER sceneManager::getSingleton()
#define SOUNDMANAGER soundManager::getSingleton()
#define CAMERAMANAGER cameraManager::getSingleton()
#define TIME timeManager::getSingleton()
#define PARTICLE particleManager::getSingleton()
#define PARTICLE0 particleManager0::getSingleton()
#define UI uiManager::getSingleton()
#define EFFECT effectManager::getSingleton()
#define ITEM itemManager::getSingleton()
#define DROP dropManager::getSingleton()
#define BOSSMANAGER bossManager::getSingleton()
//#define ASTAR astarManager::getSingleton()

//=============================================================
//	## 디파인문 ## (윈도우창 초기화)
//=============================================================
#define WINNAME (LPTSTR)(TEXT(" copy Wizard of Legend - 이혜림 채수현 허재윤 정광희 team Yellow Carrot "))
#define WINSTARTX	100
#define WINSTARTY	50
#define WINSIZEX	1280
#define WINSIZEY	720
#define WINSTYLE	WS_CAPTION | WS_SYSMENU

//=============================================================
//	## 매크로함수 ## (클래스에서 동적할당된 부분 해제)
//=============================================================
#define SAFE_DELETE(p)		{if(p) {delete (p); (p) = NULL;}}
#define SAFE_DELETE_ARRAY(p)	{if(p) {delete[] (p); (p) = NULL;}}

//=============================================================
//	## 전역변수 ##
//=============================================================
extern HINSTANCE	_hInstance;
extern HWND			_hWnd;
extern POINT		_ptMouse;
extern int			_mouseWheel;