#pragma once
//=============================================================
//	## image ## (������ ��� ������Ʈ �ȴ�)
//=============================================================

class image
{
public:
	enum IMAGE_LOAD_KIND
	{
		LOAD_EMPTY,		//���Ʈ�� �ε�
		LOAD_FILE		//���Ϸ� �ε�
	};

	typedef struct tagImage
	{
		HDC hMemDC;			//�޸� DC
		HBITMAP hBit;		//��Ʈ��
		HBITMAP hOBit;		//�õ��Ʈ��
		float x;				//�̹��� x��ǥ
		float y;				//�̹��� y��ǥ
		int width;			//�̹��� ���α���
		int height;			//�̹��� ���α���
		int currentFrameX;	//���� ������X
		int currentFrameY;	//���� ������Y
		int maxFrameX;		//�ִ� ������X ����
		int maxFrameY;		//�ִ� ������Y ����
		int frameWidth;		//1������ ���α���
		int frameHeight;	//1������ ���α���
		BYTE loadType;		//�̹��� �ε�Ÿ��
		tagImage()
		{
			hMemDC = NULL;
			hBit = NULL;
			hOBit = NULL;
			x = 0;
			y = 0;
			width = 0;
			height = 0;
			currentFrameX = 0;
			currentFrameY = 0;
			maxFrameX = 0;
			maxFrameY = 0;
			frameWidth = 0;
			frameHeight = 0;
			loadType = LOAD_EMPTY;
		}
	}IMAGE_INFO, *LPIMAGE_INFO;

private:
	LPIMAGE_INFO	_imageInfo;		//�̹��� ����
	char*			_fileName;		//�̹��� �����̸�
	bool			_isTrans;		//���� ���ٰų�?
	COLORREF		_transColor;	//���� ���� RGB (����Ÿ = RGB(255, 0, 255))

	LPIMAGE_INFO	_blendImage;	//���ĺ��� �̹���
	BLENDFUNCTION	_blendFunc;		//���ĺ��� ���
	LPIMAGE_INFO    _stretchImage;    //��Ʈ��ġ �̹���

public:
	image();
	~image();

	//�� ��Ʈ�� �ʱ�ȭ
	HRESULT init(int width, int height);
	//�̹��� ���Ϸ� �ʱ�ȭ
	HRESULT init(const char* fileName, int width, int height, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
	HRESULT init(const char* fileName, float x, float y, int width, int height, bool isTrans = false, COLORREF transColor = RGB(0, 0, 0));
	//������ �̹��� ���Ϸ� �ʱ�ȭ
	HRESULT init(const char* fileName, int width, int height, int frameX, int frameY, bool isTrans = true, COLORREF transColor = RGB(255, 0, 255));
	HRESULT init(const char* fileName, float x, float y, int width, int height, int frameX, int frameY, bool isTrans = true, COLORREF transColor = RGB(255, 0, 255));
	
	//���ĺ��� �ʱ�ȭ
	HRESULT initForAlphaBlend();
	HRESULT initForStretchBlend();

	//����
	void release();

	//����(���� ���ϴ� ��ǥ�� ����ض�, �Ϲ������� �츮�� RECT�� LT��ǥ�� �̹����� ���δ�)
	void render(HDC hdc, int destX = 0, int destY = 0);
	void render(HDC hdc, int destX, int destY, int scale);
	//����(���ϴ� ��ǥ�� �̹����� �߶� ���̱�)
	void render(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight);
	
	void renderResize(HDC hdc, int destX, int destY, int sourWidth, int sourHeight, RECT rc, int defaultSize);

	void stretchRender(HDC hdc, int destX, int destY, float scale);

	void stretchFrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, float scale);

	//���ķ���(�̹����� ���İ���ŭ ����ȭ �����ش�)
	void alphaRender(HDC hdc, BYTE alpha);
	void alphaRender(HDC hdc, int destX, int destY, BYTE alpha);
	void alphaRender(HDC hdc, int destX, int destY, int sourX, int sourY, int sourWidth, int sourHeight, BYTE alpha);
	//�����ӷ���
	void frameRender(HDC hdc, int destX, int destY);
	void frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY);
	void frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, RECT rc, int defaultSize);
	void frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, RECT scale);
	void frameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, float scale);
	//���� ������ ����
	void alphaFrameRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, BYTE alpha);


	//��������
	void loopRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY);
	void loopAlphaRender(HDC hdc, const LPRECT drawArea, int offsetX, int offsetY, BYTE alpha);

	void frameStretchAlphaRender(HDC hdc, int destX, int destY, int currentFrameX, int currentFrameY, float scaleX, float scaleY, BYTE alpha);

	//DC ���
	inline HDC getMemDC() { return _imageInfo->hMemDC; }

	//�̹��� x, y��ǥ
	inline float getX() { return _imageInfo->x; }
	inline float getY() { return _imageInfo->y; }
	inline void setX(float x) { _imageInfo->x = x; }
	inline void setY(float y) { _imageInfo->y = y; }
	//�̹��� ������ǥ
	inline void setCenter(float x, float y)
	{
		_imageInfo->x = x + (_imageInfo->width / 2);
		_imageInfo->y = y + (_imageInfo->height / 2);
	}
	//�̹��� ����, ���α��� ��������
	inline int getWidth() { return _imageInfo->width; }
	inline int getHeight() { return _imageInfo->height; }
	//�ٿ�� �ڽ�(�浹�� ��Ʈ)
	inline RECT boundingBox()
	{
		RECT rc = { (int)_imageInfo->x, (int)_imageInfo->y,
		(int)_imageInfo->x + _imageInfo->width,
		(int)_imageInfo->y + _imageInfo->height };
		return rc;
	}
	inline RECT boudingBoxWithFrame()
	{
		RECT rc = { (int)_imageInfo->x, (int)_imageInfo->y,
		(int)_imageInfo->x + _imageInfo->frameWidth,
		(int)_imageInfo->y + _imageInfo->frameHeight };
		return rc;
	}
	//������ X, Y
	inline int getFrameX() { return _imageInfo->currentFrameX; }
	inline int getFrameY() { return _imageInfo->currentFrameY; }
	inline void setFrameX(int frameX)
	{
		_imageInfo->currentFrameX = frameX;
		if (frameX > _imageInfo->maxFrameX)
		{
			_imageInfo->currentFrameX = _imageInfo->maxFrameX;
		}
	}
	inline void setFrameY(int frameY)
	{
		_imageInfo->currentFrameY = frameY;
		if (frameY > _imageInfo->maxFrameY)
		{
			_imageInfo->currentFrameY = _imageInfo->maxFrameY;
		}
	}
	//1������ ����, ���α��� ��������
	inline int getFrameWidth() { return _imageInfo->frameWidth; }
	inline int getFrameHeight() { return _imageInfo->frameHeight; }
	//�ƽ� ������ ��������
	inline int getMaxFrameX() { return _imageInfo->maxFrameX; }
	inline int getMaxFrameY() { return _imageInfo->maxFrameY; }

};