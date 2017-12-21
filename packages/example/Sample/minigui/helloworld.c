/*
 ** $Id: helloworld.c 793 2010-07-28 03:36:29Z dongjunjie $
 **
 ** Listing 1.1
 **
 ** helloworld.c: Sample program for MiniGUI Programming Guide
 **      The first MiniGUI application.
 **
 ** Copyright (C) 2004 ~ 2009 Feynman Software.
 **
 ** License: GPL
 */

#include <stdio.h>
#include <string.h>

#include <common.h>
#include <minigui.h>
#include <gdi.h>
#include <window.h>

#define _LANG_ZHCN
#ifdef _LANG_ZHCN
#include "helloworld_res_cn.h"
#elif defined _LANG_ZHTW
#include "helloworld_res_tw.h"
#else
#include "helloworld_res_en.h"
#endif

static char welcome_text [512];
static char msg_text [256];
static RECT welcome_rc = {10, 100, 600, 400};
static RECT msg_rc = {10, 100, 600, 400};

static const char* syskey = "";

static int last_key = -1;
static int last_key_count = 0;

static void make_welcome_text (void)
{
	const char* sys_charset = GetSysCharset (TRUE);
	const char* format;

	if (sys_charset == NULL)
		sys_charset = GetSysCharset (FALSE);

	SetRect (&welcome_rc,  5, 5, g_rcScr.right - 5, g_rcScr.bottom / 2 - 10);
	SetRect (&msg_rc, 5, welcome_rc.bottom + 10, g_rcScr.right - 10, g_rcScr.bottom - 30);

	if (strcmp (sys_charset, FONT_CHARSET_GB2312_0) == 0
			|| strcmp (sys_charset, FONT_CHARSET_GBK) == 0
			|| strcmp (sys_charset, FONT_CHARSET_GB18030_0) == 0) {
		format = "欢迎来到 MiniGUI 的世界! 如果您能看到该文本, 则说明 MiniGUI Version %d.%d.%d 可在该硬件上运行!";
	}
	else if (strcmp (sys_charset, FONT_CHARSET_BIG5) == 0) {
		format = "欢迎来到 MiniGUI 的世界! 如果您能看到该文本, 则说明 MiniGUI Version %d.%d.%d 可在该硬件上运行!";
	}
	else {
		format = "Welcome to the world of ingenic MiniGUI. MiniGUI Version %d.%d.%d\n";
	}

	sprintf (welcome_text, format, MINIGUI_MAJOR_VERSION, MINIGUI_MINOR_VERSION, MINIGUI_MICRO_VERSION);
	strcat(welcome_text,format);
	strcpy (msg_text, HL_ST_NOMES);
}

PLOGFONT logfontsong20;
BITMAP bmp;
unsigned int counter = 100;
int tmp = 0;
static int HelloWinProc(HWND hWnd, int message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;

	syskey = "";

	switch (message) {
		case MSG_CREATE:
			make_welcome_text ();
			SetTimer (hWnd, 100, 100);
			LoadBitmap(HDC_SCREEN, &bmp, "./penguin.jpg");
			logfontsong20 = CreateLogFont (NULL, "song", "GB2312",
					FONT_WEIGHT_REGULAR, FONT_SLANT_ROMAN,
					FONT_FLIP_NIL, FONT_OTHER_NIL,
					FONT_UNDERLINE_NONE, FONT_STRUCKOUT_NONE,
					16, 0);
			SetWindowFont(hWnd,logfontsong20);

			break;

		case MSG_TIMER:
			sprintf (msg_text, HL_ST_TIMER,counter);
			counter--;
			InvalidateRect (hWnd, &msg_rc, TRUE);
			if(counter == 0) {
				PostMessage (hWnd, MSG_CLOSE, 0, 0);
			}
			break;

		case MSG_LBUTTONDOWN:
			strcpy (msg_text, HL_ST_LBD);
			InvalidateRect (hWnd, &msg_rc, TRUE);
			break;

		case MSG_LBUTTONUP:
			strcpy (msg_text, HL_ST_LBU);
			InvalidateRect (hWnd, &msg_rc, TRUE);
			break;

		case MSG_RBUTTONDOWN:
			strcpy (msg_text, HL_ST_RBD);
			InvalidateRect (hWnd, &msg_rc, TRUE);
			break;

		case MSG_RBUTTONUP:
			strcpy (msg_text, HL_ST_RBU);
			InvalidateRect (hWnd, &msg_rc, TRUE);
			break;

		case MSG_PAINT:
			hdc = BeginPaint (hWnd);
			FillBoxWithBitmap(hdc, 0, 0, 230, 230, &bmp);
			DrawText (hdc, msg_text, -1, &msg_rc, DT_LEFT | DT_WORDBREAK);
			EndPaint (hWnd, hdc);
			return 0;

		case MSG_SYSKEYDOWN:
			syskey = HL_ST_SYS;
		case MSG_KEYDOWN:
			if(last_key == wParam)
				last_key_count++;
			else
			{
				last_key = wParam;
				last_key_count = 1;
			}
			sprintf (msg_text, HL_ST_KEYD,
					wParam, syskey, last_key_count);
			InvalidateRect (hWnd, &msg_rc, TRUE);
			return 0;

		case MSG_KEYLONGPRESS:
			sprintf (msg_text, HL_ST_KEYLONG, wParam);
			InvalidateRect (hWnd, &msg_rc, TRUE);
			break;

		case MSG_KEYALWAYSPRESS:
			sprintf (msg_text, HL_ST_KEYALWAY, wParam);
			InvalidateRect (hWnd, &msg_rc, TRUE);
			break;

		case MSG_KEYUP:
			sprintf (msg_text, HL_ST_KEYU, wParam);
			InvalidateRect (hWnd, &msg_rc, TRUE);
			return 0;

		case MSG_CLOSE:
			UnloadBitmap (&bmp);
			DestroyLogFont(logfontsong20);
			KillTimer (hWnd, 100);
			DestroyMainWindow (hWnd);
			PostQuitMessage (hWnd);
			return 0;
	}

	return DefaultMainWinProc(hWnd, message, wParam, lParam);
}

int MiniGUIMain (int argc, const char* argv[])
{
	MSG Msg;
	HWND hMainWnd;
	MAINWINCREATE CreateInfo;
	HDC hdc;
	char buf=0;
#ifdef _MGRM_PROCESSES
	JoinLayer(NAME_DEF_LAYER , "helloworld" , 0 , 0);
#endif

	CreateInfo.dwStyle =
		WS_VISIBLE | WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;
	CreateInfo.dwExStyle = WS_EX_NONE;
	CreateInfo.spCaption = "HelloWorld";
	CreateInfo.hMenu = 0;
	CreateInfo.hCursor = GetSystemCursor(0);
	CreateInfo.hIcon = 0;
	CreateInfo.MainWindowProc = HelloWinProc;
	CreateInfo.lx = 0;
	CreateInfo.ty = 0;
	CreateInfo.rx = g_rcScr.right;
	CreateInfo.by = g_rcScr.bottom;
	CreateInfo.iBkColor = COLOR_lightwhite;
	CreateInfo.dwAddData = 0;
	CreateInfo.hHosting = HWND_DESKTOP;

	hMainWnd = CreateMainWindow (&CreateInfo);
	if (hMainWnd == HWND_INVALID)
		return -1;

	ShowWindow(hMainWnd, SW_SHOWNORMAL);
	while (GetMessage(&Msg, hMainWnd)) {
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	MainWindowThreadCleanup (hMainWnd);

	exit(0);
}

#ifdef _MGRM_THREADS
//#include <dti.c>
#endif

