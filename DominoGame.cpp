#include<Windows.h>
#include<thread>
#include"GameController.hpp"




void CALLBACK EventProc(HWINEVENTHOOK hook, DWORD event, HWND wnd, LONG object, LONG child,
	DWORD thread, DWORD time) {
	DWORD fullScreenCheck = CONSOLE_FULLSCREEN;
	GetConsoleDisplayMode(&fullScreenCheck);
	if (fullScreenCheck != CONSOLE_FULLSCREEN)
	{
		COORD endCor = GameController::GetConsoleSize(), cursorStart = { 0,0 };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorStart);

		char* clean = new char[(endCor.X + 1) * endCor.Y + 1]{ '\0' };
		memset(clean, ' ', (endCor.X + 1) * endCor.Y * sizeof(char));
		printf("%s", clean);
		delete[] clean;

		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorStart);

		CONSOLE_FONT_INFOEX font = GameController::GetStartConsoleFont();
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE),1,&font);
		exit(0);
	}
}

void FULLSCREEN(void)
{
	
	HWND windowConsole = GetConsoleWindow();

	HWINEVENTHOOK eventHook = SetWinEventHook(EVENT_CONSOLE_LAYOUT, EVENT_CONSOLE_LAYOUT,
		NULL, EventProc, 0, 0, WINEVENT_OUTOFCONTEXT);
	if (eventHook == 0)
	{
		exit(0);
	}
	MSG msg;
	while (GetMessage(&msg, windowConsole, EVENT_CONSOLE_LAYOUT, EVENT_CONSOLE_LAYOUT));
}



int main()
{
	GameController controller;
	std::thread thr(FULLSCREEN);
	short v = SCENE_LOGO;
	while (v!=SCENE_GAME_EXIT)
	{
		v = controller.Scene(v);
	}
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_WINDOWED_MODE, 0);
	thr.join();
	return 0;
}
