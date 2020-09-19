//=============================================================
//	## 헤더파일 써준다 ##
//=============================================================
#include "stdafx.h"
#include "mainGame.h"

//=============================================================
//	## 전역변수 써준다 ##
//=============================================================
HWND		_hWnd;
HINSTANCE	_hInstance;
POINT		_ptMouse = { 0, 0 };
//메인게임 클래스 선언
mainGame* _mg;


//=============================================================
//	## 함수 전방선언 ##
//=============================================================
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void setWindowSize(int x, int y, int width, int height);

//=============================================================
//	## 윈도우 메인함수 ##
//=============================================================
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	//메인게임 클래스 동적할당
	_mg = new mainGame;

	//인스턴스를 전역변수에 담아둔다
	_hInstance = hInstance;

	//WNDCLASS : 윈도우의 정보를 저장하기 위한 구조체

	//윈도우 구조체 선언 및 초기화
	WNDCLASS wndClass;
	wndClass.cbClsExtra = 0;										//클래스 여분 메모리
	wndClass.cbWndExtra = 0;										//윈도우 여분 메모리
	wndClass.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);	//백그라운드
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);					//마우스 커서
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);				//아이콘
	wndClass.hInstance = hInstance;									//인스턴스
	wndClass.lpfnWndProc = (WNDPROC)WndProc;						//윈도우 프로시져
	wndClass.lpszClassName = WINNAME;								//클래스 이름
	wndClass.lpszMenuName = NULL;									//메뉴이름
	wndClass.style = CS_HREDRAW | CS_VREDRAW;						//윈도우 스타일

	//윈도우 클래스 등록																//윈도우 클래스 등록
	RegisterClass(&wndClass);

	//윈도우창 생성
	_hWnd = CreateWindow(
		WINNAME,				//윈도우 클래스의 이름
		WINNAME,				//윈도우 타이틀바 이름
		WINSTYLE,				//윈도우 스타일
		WINSTARTX,				//윈도우 화면좌표 X
		WINSTARTY,				//윈도우 화면좌표 Y
		WINSIZEX,				//윈도우 화면 가로크기
		WINSIZEY,				//윈도우 화면 세로크기
		NULL,					//부모 윈도우
		(HMENU)NULL,			//메뉴핸들
		hInstance,				//인스턴스 지정
		NULL					//윈도우 및 자식 윈도우를 생성하면 지정해주고 그렇지 않으면 NULL
	);

	//클라이언트 영역의 사이즈를 정확히 잡아준다
	setWindowSize(WINSTARTX, WINSTARTY, WINSIZEX, WINSIZEY);

	//화면에 윈도우 보여준다
	ShowWindow(_hWnd, nCmdShow);

	//메인게임 클래스 초기화를 실패하면 바로 종료시키기
	if (FAILED(_mg->init()))
	{
		return 0;
	}

	//MSG : 운영체제에서 발행하는 메세지 정보를 저장하기 위한 구조체
	MSG message;
	//메세지 루프
	//GetMessage : 메세지를 꺼내올 수 있을때까지 멈춰있는 함수
	//PeekMessage : 메세지가 없더라도 리턴되는 함수

	/*
	//게임용 나중에 사용한다
	while (true)
	{
		if (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT) break;
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}
	*/

	//일반 프로그래밍용
	while (GetMessage(&message, 0, 0, 0))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	//메인게임 클래스 해제
	_mg->release();

	return message.wParam;
}



//윈도우 프로시져 : 메세지를 운영체제에 전달하고 코딩하는 구간
//hWnd : 어느 윈도우창에서 발생한 메세지인지 구분
//message : 메세지 구분 번호
//wParam : unsigned int 마우스 버튼의 상태, 키보드 조합키의 상태를 전달
//lParam : unsigned long 마우스 클릭 좌표를 전달(x, y)
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//메인게임 클래스야 니가 메인프로시져 실행시켜
	return _mg->MainProc(hWnd, message, wParam, lParam);
}

//=============================================================
//	## 윈도우 크기 조정 ## (클라이언트 영역의 사이즈를 정확히 잡아준다)
//=============================================================
void setWindowSize(int x, int y, int width, int height)
{
	RECT rc = { 0, 0, width, height };

	//실제 윈도우 크기 조정
	AdjustWindowRect(&rc, WINSTYLE, false);

	//위 렉트 정보로 윈도우 클라이언트 사이즈 세팅
	SetWindowPos(_hWnd, NULL, x, y, (rc.right - rc.left), (rc.bottom - rc.top),
		SWP_NOZORDER | SWP_NOMOVE);
}