// Lab8.cpp: Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "Lab8.h"
#include "windows.h"
#include <cstdio>
#include <algorithm>
#include <string>
#include <limits>
#define ID_GAMESTEP 1002
#define ID_GETRESULT 1003
#define ID_EXIT 1004
#define ID_NEWGAME 1005
#define MAX_LOADSTRING 100

using namespace std;
POINT coords;
enum states { state_1, state_2 };

enum signals {
	unknownSig, gameStep, getResult, btnYes, exitGame
};
enum actions {
	unknown, gameStepAct, getResultAct, newGame, exitGameAct
};

enum states current_state = state_1;
enum signals current_signal = unknownSig;
enum actions current_action;
enum players { player1 = 0, computer };

wstring winner = L"";

void getSignal(int lowordId, int hiwordId) {
	switch (lowordId) {
	case ID_GAMESTEP:
		current_signal = gameStep;
		break;

	case ID_GETRESULT:
		current_signal = getResult;
		break;

	case ID_NEWGAME:
		current_signal = btnYes;
		break;

	case ID_EXIT:
		current_signal = exitGame;
		break;

	default:
		current_signal = unknownSig;
		break;
	}
}
actions getAction(signals sig) {
	switch (sig)
	{
	case gameStep:
	{
		if (current_state == state_1)
			return gameStepAct;
		else
			return unknown;
	}
	case getResult:
	{
		if (current_state == state_1)
			return getResultAct;
		else
			return unknown;
	}
	case btnYes:
	{
		if (current_state == state_2)
			return newGame;
		else
			return unknown;
	}
	case exitGame:
		return exitGameAct;
	default:
		return unknown;
	}
}

BOOL RegClass(WNDPROC, LPCTSTR, UINT);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE hInstance;

void DrawField(HDC);
void DrawCross(HDC, int, int);

void start(HDC, players);
void stepPlayer(HDC);
void stepComputer(HDC);

BOOL isSquareFree(int, int);
BOOL isTie();
BOOL isWinner(char);

POINT Minimax();
int MaxSearch();
int MinSearch();

HWND hwnd;

char field[3][3] = { ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' };
char shape[2]{ 'X', 'O' };

int xMouse, yMouse;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	hInstance = hInst;

	if (!RegClass(WndProc, L"MainClass", COLOR_GRADIENTACTIVECAPTION)) { return FALSE; }

	hwnd = CreateWindowW(L"MainClass", L"Крестики-нолики",
		WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX,
		(GetSystemMetrics(SM_CXSCREEN) - 500) / 2,
		(GetSystemMetrics(SM_CXSCREEN) - 500) / 2,
		500, 500,
		0, 0, hInstance, NULL);

	if (!hwnd) { return FALSE; }

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

BOOL RegClass(WNDPROC Proc, LPCTSTR szName, UINT brBackground)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpfnWndProc = Proc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(brBackground + 1);
	wc.lpszMenuName = (LPCTSTR)NULL;
	wc.lpszClassName = szName;
	return (RegisterClass(&wc) != 0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (msg)
	{
	case WM_COMMAND:
	{
		int lowordId = LOWORD(wParam);
		int hiwordId = HIWORD(wParam);
		
		getSignal(lowordId, hiwordId);
		current_action = getAction(current_signal);

		switch (current_action) {
		case gameStepAct:
		{
			hdc = GetDC(hWnd);
			start(hdc, player1);

			if (isWinner(shape[player1]))
			{
				winner += L"Поздравляем, Вы победили!";	
				PostMessage(hWnd, WM_COMMAND, ID_GETRESULT, 0);
			}

			if (isWinner(shape[computer]))
			{
				winner += L"Компьютер победил!";
				PostMessage(hWnd, WM_COMMAND, ID_GETRESULT, 0);
			}

			if (isTie())
			{
				winner += L"Ничья!";				
				PostMessage(hWnd, WM_COMMAND, ID_GETRESULT, 0);
			}
		}
		break;
		case getResultAct:
		{
			current_state = state_2;
			winner += L"\n\n Начать новую игру?";
			if (MessageBox(hwnd, winner.c_str(), L"Результат", MB_YESNO) == IDYES)
				PostMessage(hWnd, WM_COMMAND, ID_NEWGAME, 0);
			else
				PostMessage(hWnd, WM_COMMAND, ID_EXIT, 0);
		}
		break;
		case newGame:
		{
			current_state = state_1;
			winner = L"";
			for (int i = 0; i < 3; i++)
				for (int j = 0; j < 3; j++)
					field[i][j] = ' ';

			hdc = GetDC(hWnd);
			DrawField(hdc);			
		}
		break;
		case exitGameAct:
		{
			PostQuitMessage(0);
		}
		break;
		default:
			break;
		}
	}
	break;
	case WM_LBUTTONDOWN:
	{
		xMouse = LOWORD(lParam);
		yMouse = HIWORD(lParam);
		SendMessage(hWnd, WM_COMMAND, ID_GAMESTEP, 0);
		break;
	}
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		DrawField(hdc);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_CLOSE:
		SendMessage(hWnd, WM_COMMAND, ID_EXIT, 0);
		break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void start(HDC hdc, players curPlayer)
{
	switch (curPlayer)
	{
	case player1:
	{
		stepPlayer(hdc);
		break;
	}
	case computer:
	{
		stepComputer(hdc);
		break;
	}
	}
}

void stepPlayer(HDC hdc)
{
	if (isSquareFree(xMouse, yMouse))
	{
		DrawCross(hdc, (xMouse - 100) / 100, (yMouse - 100) / 100);
		field[(xMouse - 100) / 100][(yMouse - 100) / 100] = shape[player1];
	}
	else
	{
		char str[64];

		MessageBox(nullptr, L"Клетка занята!", L"Ошибка", MB_OK);
		return;
	}

	start(hdc, computer);
}

void stepComputer(HDC hdc)
{
	if (isTie()) return;

	POINT computerMove = Minimax();

	Ellipse(hdc, computerMove.x * 100 + 110, computerMove.y * 100 + 110, (computerMove.x + 1) * 100 + 90, (computerMove.y + 1) * 100 + 90);
	field[computerMove.x][computerMove.y] = shape[computer];

}

void DrawField(HDC hdc)
{
	HPEN hPen = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 1, RGB(179, 179, 204)));
	HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
	SelectObject(hdc, brush);
	Rectangle(hdc, 100, 100, 400, 400);
	DeleteObject(brush);
	MoveToEx(hdc, 200, 100, NULL);
	LineTo(hdc, 200, 400);

	MoveToEx(hdc, 300, 100, NULL);
	LineTo(hdc, 300, 400);

	MoveToEx(hdc, 100, 200, NULL);
	LineTo(hdc, 400, 200);

	MoveToEx(hdc, 100, 300, NULL);
	LineTo(hdc, 400, 300);
}

void DrawCross(HDC hdc, int left, int top)
{
	HPEN hPen = (HPEN)SelectObject(hdc, CreatePen(PS_SOLID, 2, RGB(0, 0, 255)));

	MoveToEx(hdc, left * 100 + 110, top * 100 + 110, NULL);
	LineTo(hdc, (left + 1) * 100 + 90, (top + 1) * 100 + 90);

	MoveToEx(hdc, (left + 1) * 100 + 90, top * 100 + 110, NULL);
	LineTo(hdc, left * 100 + 110, (top + 1) * 100 + 90);

	DeleteObject(hPen);

}

BOOL isSquareFree(int _xMouse, int _yMouse)
{
	return (field[(_xMouse - 100) / 100][(_yMouse - 100) / 100] == ' ') ? true : false;
}

BOOL isTie()
{
	for (int i = 0; i < 3; i++)
	{
		if (field[i][0] == ' ' || field[i][1] == ' ' || field[i][2] == ' ')
		{
			return false;
		}
	}
	return true;
}

BOOL isWinner(char player)
{
	for (int i = 0; i < 3; i++)
	{
		for (int i = 0; i < 3; i++)
		{
			if (field[i][0] == player && field[i][1] == player && field[i][2] == player)
			{
				return true;
			}

			if (field[0][i] == player && field[1][i] == player && field[2][i] == player)
			{
				return true;
			}
		}

		if (field[0][0] == player && field[1][1] == player && field[2][2] == player)
		{
			return true;
		}

		if (field[0][2] == player && field[1][1] == player && field[2][0] == player)
		{
			return true;
		}

		return false;
	}
}

POINT Minimax()
{
	int score = (std::numeric_limits<int>::max)();
	POINT move;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (field[i][j] == ' ')
			{
				field[i][j] = shape[computer];

				int temp = MaxSearch();

				if (temp < score)
				{
					score = temp;
					move.x = i;
					move.y = j;
				}
				field[i][j] = ' ';
			}
		}
	}

	return move;
}

int MaxSearch()
{
	if (isWinner(shape[player1])) { return 10; }
	else if (isWinner(shape[computer])) { return -10; }
	else if (isTie()) { return 0; }

	int score = (std::numeric_limits<int>::min)();

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (field[i][j] == ' ')
			{
				field[i][j] = shape[player1];
				score = (std::max)(score, MinSearch());
				field[i][j] = ' ';
			}
		}
	}

	return score;
}

int MinSearch()
{
	if (isWinner(shape[player1])) { return 10; }
	else if (isWinner(shape[computer])) { return -10; }
	else if (isTie()) { return 0; }

	int score = (std::numeric_limits<int>::max)();

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (field[i][j] == ' ')
			{
				field[i][j] = shape[computer];
				score = (std::min)(score, MaxSearch());
				field[i][j] = ' ';
			}
		}
	}

	return score;
}

