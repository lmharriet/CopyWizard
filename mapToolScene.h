#pragma once
#include "gameNode.h"
#include "tileNode.h"
#include "colorNode.h"

class mapToolScene : public gameNode
{
private:
	/// USER ///
	tagUser user;

	/// TILE ///
	tagTile tile[MAXTILE];

	/// OPTION ///
	RECT mapOption[3];
	OPTION option;
	
	/// BUTTON ///
	RECT SAVE, LOAD, BACK;
	///  ICON  ///
	tagIcon icon[4];
	tagIcon drag;

	tagTile bigTile[4];
	tagTile miniTile[4];

	/// CAMERA ///
	tagCam cam;

	//vk_down
	bool isLeftDown, isLeft, isLeftUp;
public:
	HRESULT init();
	void release();
	void update();
	void render();

	void buttonCheck();
	void iconCheck();

	//init
	void initTile();
	void initSelecTile();
	void initCam();
	//camera
	void moveRect();

	//image
	void addImage();

	//render
	void tileRender();
	void UIRender();
	void textRender();
	void buttonRender();
	void rcRender();
	void checkRender();

	void imageRender(string keyName, POINT pt);
	void imageAlphaRender(string keyName, POINT pt, int alpha = 150);

	//don't touch
	void controller();
	void vkCheck();
};