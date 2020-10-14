#pragma once
#include "gameNode.h"
#include "progressBar.h"

//=============================================================
//	## loadItem ## (로드아이템 - 이미지정보, 사운드정보)
//=============================================================

//로드아이템 종류
enum LOAD_KIND
{
	LOAD_KIND_IMAGE_0,			//빈비트맵 이미지
	LOAD_KIND_IMAGE_1,			//일반 이미지
	LOAD_KIND_IMAGE_2,			//일반 이미지(좌표)
	LOAD_KIND_FRAMEIMAGE_0,		//프레임 이미지
	LOAD_KIND_FRAMEIMAGE_1,		//프레임 이미지(좌표)
	LOAD_KIND_SOUND,			//사운드
	LOAD_KIND_END
};

//이미지 리소스 구조체
struct tagImageResource
{
	string keyName;				//이미지 키값
	const char* fileName;		//이미지 파일이름
	int x, y;					//위치좌표
	int width, height;			//가로, 세로길이
	int frameX, frameY;			//프레임 x, y
	bool isTrans;				//배경지울거임?
	COLORREF transColor;		//어떤색으로
};

//사운드 리소스 구조체
struct tagSoundResource
{
	//(키값, 파일이름, BGM ? , 루프시킬거냐)
};

class loadItem
{
private:
	LOAD_KIND _kind;					//로딩종류
	tagImageResource _imageResource;	//이미지 정보

public:
	HRESULT init(string strKey, int width, int height);
	HRESULT init(string strKey, const char* fileName, int width, int height, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
	HRESULT init(string strKey, const char* fileName, float x, float y, int width, int height, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
	HRESULT init(string strKey, const char* fileName, int width, int height, int frameX, int frameY, bool isTrans = true, COLORREF transColor = RGB(255, 0, 255));
	HRESULT init(string strKey, const char* fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans = true, COLORREF transColor = RGB(255, 0, 255));

	//로딩아이템 종류 가져오기
	LOAD_KIND getLoadKind() { return _kind; }
	//이미지 리소스 가져오기
	tagImageResource getImageResource() { return _imageResource; }
};

//=============================================================
//	## loading ## (로딩클래스 - 로딩화면 구현하기, 로딩바, 백그라운드)
//=============================================================
class loading : public gameNode
{
private:
	//로드아이템 클래스를 담을 벡터 및 반복자
	typedef vector<loadItem*> vLoadItem;
	typedef vector<loadItem*>::iterator viLoadItem;

private:
	vLoadItem _vLoadItem;

	//이부분은 로딩화면에서 사용할 이미지와 로딩바
	image* _background;
	progressBar* _loadingBar;
	int _currentGauge;

public:
	HRESULT init();
	void release();
	void update();
	void render();

	void loadImage(string strKey, int width, int height);
	void loadImage(string strKey, const char* fileName, int width, int height, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
	void loadImage(string strKey, const char* fileName, float x, float y, int width, int height, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
	void loadFrameImage(string strKey, const char* fileName, int width, int height, int frameX, int frameY, bool isTrans = true, COLORREF transColor = RGB(255, 0, 255));
	void loadFrameImage(string strKey, const char* fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans = true, COLORREF transColor = RGB(255, 0, 255));

	/*중요함*/
	//로딩완료 됐냐? (로딩완료후 화면전환)
	bool loadingDone();

	//로드아이템 클래스 담겨있는 벡터 가져오기
	vLoadItem getLoadItem() { return _vLoadItem; }
	//현재게이지 가져오기
	int getCurrentGauge() { return _currentGauge; }

};

