#pragma once
//=============================================================
//	## commonMacroFunction ##
//=============================================================

//POINT
inline POINT PointMake(int x, int y)
{
	POINT pt = { x, y };
	return pt;
}

//선 그리기
inline void LineMake(HDC hdc, int startX, int startY, int endX, int endY)
{
	MoveToEx(hdc, startX, startY, NULL);
	LineTo(hdc, endX, endY);
}

//RECT 만들기
//RECT rc = { 128, 59, 228, 159 };
//RECT rc = RectMake(128, 59, 100, 100)
inline RECT RectMake(int x, int y, int width, int height)
{
	RECT rc = { x, y, x + width, y + height };
	return rc;
}
//RECT 중심좌표로 만들기
inline RECT RectMakeCenter(int x, int y, int width, int height)
{
	RECT rc = { x - width / 2, y - height / 2, x + width / 2, y + height / 2 };
	return rc;
}

//Rectangle 함수 이용해서 사각형 그리기
inline void RectangleMake(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x, y, x + width, y + height);
}

//Ellipse 함수 이용해서 타원 그리기
inline void EllipseMake(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x, y, x + width, y + height);
}
//Ellipse 함수 이용해서 타원 중심좌표에 그리기
inline void EllipseMakeCenter(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x - width / 2, y - height / 2, x + width / 2, y + height / 2);
}

//편하쥬?
inline void Rectangle(HDC hdc, RECT rc)
{
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
}

//프레임렉트 함수 오버로딩(비어있는 렉트 만들기)
inline void FrameRect(HDC hdc, RECT rc, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	FrameRect(hdc, &rc, brush);
	DeleteObject(brush);
}

//텍스트 출력 편하게 사용하자
inline void textOut(HDC hdc, int x, int y, const char* text, COLORREF color = RGB(255, 255, 0))
{
	SetTextColor(hdc, color);
	TextOut(hdc, x, y, text, strlen(text));
}

inline void ptOut(HDC hdc, POINT pos, POINT pt, COLORREF color = RGB(0, 0, 0))
{
	char str[126];
	SetTextColor(hdc, color);
	wsprintf(str, "%d %d", pt.x, pt.y);
	TextOut(hdc, pos.x, pos.y, str, strlen(str));
}

//편하게 충돌비교하기
inline bool colCheck(RECT rc, RECT _rc)
{
	RECT temp;
	if (IntersectRect(&temp, &rc, &_rc)) return true;
	return false;
}

//렉트 가로(중앙값) 쉽게 가져오기
inline int getRcCenterX(RECT rc)
{
	return rc.left + (rc.right - rc.left) / 2;
}
//렉트 세로(중앙값) 쉽게 가져오기
inline int getRcCenterY(RECT rc)
{
	return rc.top + (rc.bottom - rc.top) / 2;
}