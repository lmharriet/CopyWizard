#include "stdafx.h"
#include "progressBar.h"

HRESULT progressBar::init(const char * frontImg, const char * backImg)
{
	//체력바 이미지 초기화
	_progressBarFront = IMAGEMANAGER->findImage(frontImg);
	_progressBarBack = IMAGEMANAGER->findImage(backImg);

	//체력바 위치 초기화
	_x = _progressBarBack->getX();
	_y = _progressBarBack->getY();

	//체력바 가로, 세로길이 초기화
	_width = _progressBarBack->getWidth();
	_height = _progressBarBack->getHeight();
	
	//체력바 렉트 초기화
	_rcProgress = RectMake(_x, _y, _width, _height);


	return S_OK;
}

void progressBar::release()
{
}

void progressBar::update()
{
	//체력바 렉트 업데이트
	_rcProgress = RectMake(_x, _y, _width, _height);
}

void progressBar::render()
{
	//렌더링 되는 순서에 의해서 렌더가 되니까 피통부터 렌더 시킨다
	_progressBarBack->render(getMemDC(), _x, _y);
	//앞에 보여지는 체력바 이미지
	_progressBarFront->render(getMemDC(), _x, _y,
		0, 0, _width, _height);
}

//체력바 게이지 세팅하기
void progressBar::setGauge(float maxHp, float currentHp)
{
	_width = (currentHp / maxHp) * _progressBarBack->getWidth();
}
