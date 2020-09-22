#pragma once
#include "gameNode.h"
#include "tileNode.h"
#include "colorNode.h"

class mapToolScene : public gameNode
{
private:

	tagRC maptool;
	/// USER ///
	tagUser user;
	TOOL tool;
	//DRAG//
	tagDrag drag;
	tagRC dragButton;

	/// TILE ///
	tagTile tile[MAXTILE];

	/// OPTION ///
	RECT mapOption[3];
	OPTION option;

	// PAGEBUTTON //
	int pageNum;
	RECT pageButton[2];
	string page[3] = {"objectMenu","objectMenu2" ,"objectMenu3"};

	/// BUTTON ///
	RECT SAVE, LOAD, BACK;
	///  ICON  ///
	RECT icon[4];

	tagTile bigTile[6];
	//tagTile miniTile[4];
	
	tagTile wall[6];

	tagTile object[5];

	/// CAMERA ///
	tagCam cam;

	//저장&불러오기 창
	RECT fileWin[3];
	bool isSave;
	bool isLoad;

	//calculator
	vector<int> cul;

	//vk_down
	bool isLeftDown, isLeft, isLeftUp;
	

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void buttonCheck();
	void iconCheck();

	//reset
	void resetUserData();

	//init
	void initTile();
	void initMaptool();
	void initButton();
	void initDrag();
	void initUser();
	void initSaveAndLoad();
	void initSelectTerrain();
	void initCam();

	//camera
	void moveRect();

	//image
	void addImage();

	//save&load
	void mapSave(int index);
	void mapLoad(int index);
	void saveCheck();
	void loadCheck();

	//render
	void tileRender();
	void UIRender();
	void textRender();
	void buttonRender();
	void rcRender();
	void checkRender();

	//0922
	void objectImgRender();

	void imageRender(string keyName, POINT pt);
	void imageAlphaRender(string keyName, POINT pt, int alpha = 150);
	void imageFrameRender(string keyName, POINT pt, int frameX, int frameY);

	//don't touch
	void controller();
	void vkCheck();
};