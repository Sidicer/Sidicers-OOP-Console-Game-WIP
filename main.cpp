#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
using namespace std;

#include <stdio.h>
#include <Windows.h>
#include <conio.h>

enum COLOUR
{
	FG_BLACK = 0x0000,
	FG_DARK_BLUE = 0x0001,
	FG_DARK_GREEN = 0x0002,
	FG_DARK_CYAN = 0x0003,
	FG_DARK_RED = 0x0004,
	FG_DARK_MAGENTA = 0x0005,
	FG_DARK_YELLOW = 0x0006,
	FG_GREY = 0x0007, // Thanks MS :-/
	FG_DARK_GREY = 0x0008,
	FG_BLUE = 0x0009,
	FG_GREEN = 0x000A,
	FG_CYAN = 0x000B,
	FG_RED = 0x000C,
	FG_MAGENTA = 0x000D,
	FG_YELLOW = 0x000E,
	FG_WHITE = 0x000F,
	BG_BLACK = 0x0000,
	BG_DARK_BLUE = 0x0010,
	BG_DARK_GREEN = 0x0020,
	BG_DARK_CYAN = 0x0030,
	BG_DARK_RED = 0x0040,
	BG_DARK_MAGENTA = 0x0050,
	BG_DARK_YELLOW = 0x0060,
	BG_GREY = 0x0070,
	BG_DARK_GREY = 0x0080,
	BG_BLUE = 0x0090,
	BG_GREEN = 0x00A0,
	BG_CYAN = 0x00B0,
	BG_RED = 0x00C0,
	BG_MAGENTA = 0x00D0,
	BG_YELLOW = 0x00E0,
	BG_WHITE = 0x00F0,
};

enum PIXEL_TYPE
{
	PIXEL_SOLID = 0x2588,
	PIXEL_THREEQUARTERS = 0x2593,
	PIXEL_HALF = 0x2592,
	PIXEL_QUARTER = 0x2591,
};

class console {
private:
	unsigned short width,
		height;

public:
	unsigned short getWidth() { return this->width; }
	unsigned short getHeight() { return this->height; }

	void set(unsigned short width, unsigned short height) { this->width = width; this->height = height; }
	void setWidth(unsigned short width) { this->width = width; }
	void setHeight(unsigned short height) { this->height = height; }

	std::wstring consoleName = L"Sidicers Console Game";
};

class player {
private:
	int positionX,
		positionY;
public:
	int getPositionX() { return this->positionX; }
	int getPositionY() { return this->positionY; }

	void set(int positionX, int positionY) { this->positionX = positionX; this->positionY = positionY; }
	void setPositionX(int positionX) { this->positionX = positionX; }
	void setPositionY(int positionY) { this->positionY = positionY; }
};

console MainConsole;
player MainPlayer;

int main()
{
	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	// Get Console Info (Width, Height)
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	MainConsole.set(csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1);

	// Initiate the Console buffer
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT consoleInfo = { 0, 0, (short)MainConsole.getWidth() - 1, (short)MainConsole.getHeight() - 1 };;
	CHAR_INFO *screenBuffer = new CHAR_INFO[MainConsole.getWidth()*MainConsole.getHeight()];;
	memset(screenBuffer, 0, sizeof(CHAR_INFO) * MainConsole.getWidth() * MainConsole.getHeight());
	SetConsoleWindowInfo(hConsole, TRUE, &consoleInfo);

	// Set player position in the middle
	MainPlayer.set(MainConsole.getWidth() / 2, MainConsole.getHeight() / 2);

	while (true)
	{
		// Tracking time passed
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float timePassed = elapsedTime.count();

		for (int i = 0; i < MainConsole.getWidth()*MainConsole.getHeight(); i++) {
			screenBuffer[i].Char.UnicodeChar = L' ';														// Fill empty spaces with ' '
			screenBuffer[i].Attributes = BG_DARK_RED | FG_WHITE;											// Set the background color to DARK RED

			if (i <= MainConsole.getWidth() - 1)															// Top Border
				screenBuffer[i].Char.UnicodeChar = L'═';

			if (i >= (40 * MainConsole.getWidth() - MainConsole.getWidth()))								// Bottom Border
				screenBuffer[i].Char.UnicodeChar = L'═';

			for (int j = 1; j < MainConsole.getHeight() - 1; j++)											// Left Border
				if (i == j * MainConsole.getWidth())
					screenBuffer[i].Char.UnicodeChar = L'║';

			for (int j = 1; j < MainConsole.getHeight() - 1; j++)											// Right Border
				if (i == j * MainConsole.getWidth() + MainConsole.getWidth() - 1)
					screenBuffer[i].Char.UnicodeChar = L'║';

			if (i == 0)																						// Top Left Corner
				screenBuffer[i].Char.UnicodeChar = L'╔';
			if (i == MainConsole.getWidth() - 1)															// Top Right Corner
				screenBuffer[i].Char.UnicodeChar = L'╗';
			if (i == (MainConsole.getHeight() - 1) * MainConsole.getWidth())								// Bottom Left Corner
				screenBuffer[i].Char.UnicodeChar = L'╚';
			if (i == (MainConsole.getHeight() - 1) * MainConsole.getWidth() + MainConsole.getWidth() - 1)	// Bottom Right Corner
				screenBuffer[i].Char.UnicodeChar = L'╝';
		}

		if (_kbhit() && _getch()) {
			switch (_getch()) {
			case 72: // code for arrow up
				if (MainPlayer.getPositionY() != 1) MainPlayer.setPositionY(MainPlayer.getPositionY() - 1);
				break;

			case 80: // code for arrow down
				if (MainPlayer.getPositionY() != MainConsole.getHeight() - 2) MainPlayer.setPositionY(MainPlayer.getPositionY() + 1);
				break;

			case 75: // code for arrow left
				if (MainPlayer.getPositionX() != 1) MainPlayer.setPositionX(MainPlayer.getPositionX() - 1);
				break;

			case 77: // code for arrow right
				if (MainPlayer.getPositionX() != MainConsole.getWidth() - 2) MainPlayer.setPositionX(MainPlayer.getPositionX() + 1);
				break;
			}
		}

		screenBuffer[MainPlayer.getPositionY() * MainConsole.getWidth() + MainPlayer.getPositionX()].Char.UnicodeChar = PIXEL_SOLID;
		screenBuffer[MainPlayer.getPositionY() * MainConsole.getWidth() + MainPlayer.getPositionX()].Attributes = FG_BLACK;

		// Update screen and console info
		wchar_t s[256];
		swprintf_s(s, 256, L"%s [FPS: %3.2f]", MainConsole.consoleName.c_str(), 1.0f / timePassed);
		SetConsoleTitle(s);
		WriteConsoleOutput(hConsole, screenBuffer, { (short)MainConsole.getWidth(), (short)MainConsole.getHeight() }, { 0,0 }, &consoleInfo);
	}
	return 0;
}
