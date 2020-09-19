#pragma once
#include "singletonBase.h"
//#include "gameNode.h"
//=============================================================
//	## sceneManager ## (씬매니져)
//=============================================================

//싱글톤을 상속받은 아이들은 static으로 만들어 지기때문에
//일반클래스인 게임노드를 사용하려고 하면 터진다

//게임노드 클래스 전방선언
class gameNode;

class sceneManager : public singletonBase <sceneManager>
{
private:
	typedef map<string, gameNode*> mSceneList;
	typedef map<string, gameNode*>::iterator miSceneList;

private:	
	//map<string, gameNode*> _mSceneList;			//각각의 씬(화면)들을 담아둘 맵
	mSceneList _mSceneList;			//각각의 씬(화면)들을 담아둘 맵
	gameNode* _currentScene;		//현재씬

public:
	HRESULT init();
	void release();
	void update();
	void render();

	//씬추가
	gameNode* addScene(string sceneName, gameNode* scene);
	//씬변경
	HRESULT loadScene(string sceneName);

	sceneManager() : _currentScene(NULL) {}
	~sceneManager() {}
};

