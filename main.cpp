#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
using namespace std;

#include <stdio.h>
#include <Windows.h>
#include <conio.h>

class console {
private:
	int width,
		height;
public:
	int getWidth() { return this->width; }
	int getHeight() { return this->height; }

	void set(int width, int height) { this->width = width; this->height = height; }
	void setWidth(int width) { this->width = width; }
	void setHeight(int height) { this->height = height; }
};

console MainConsole;

void draw_outer_borders() {

}

int main()
{
	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	// Get Console Info (Width, Height, etc..)
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	MainConsole.set(csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1);

	// Create Screen Buffer
	wchar_t *screen = new wchar_t[MainConsole.getWidth()*MainConsole.getHeight()];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	// Center the player
	int nPlayerX = MainConsole.getWidth() / 2;
	int nPlayerY = MainConsole.getHeight() / 2;

	while (true)
	{
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		// Update Console Title
		wchar_t s[128];
		swprintf_s(s, 128, L"Sidicers Console Game [FPS: %f]", 1 / fElapsedTime);
		SetConsoleTitle(s);

		// Drawing Borders
		for (int i = 0; i < MainConsole.getWidth()*MainConsole.getHeight(); i++)
		{
			screen[i] = ' ';																				// Fill empty spaces with ' '

			if (i <= MainConsole.getWidth() - 1)															// Top Border
				screen[i] = L'═';

			if (i >= (40 * MainConsole.getWidth() - MainConsole.getWidth()))								// Bottom Border
				screen[i] = L'═';

			for (int j = 1; j < MainConsole.getHeight() - 1; j++)											// Left Border
				if (i == j * MainConsole.getWidth())
					screen[i] = L'║';

			for (int j = 1; j < MainConsole.getHeight() - 1; j++)											// Right Border
				if (i == j * MainConsole.getWidth() + MainConsole.getWidth() - 1)
					screen[i] = L'║';

			if (i == 0)																						// Top Left Corner
				screen[i] = L'╔';
			if (i == MainConsole.getWidth() - 1)															// Top Right Corner
				screen[i] = L'╗';
			if (i == (MainConsole.getHeight() - 1) * MainConsole.getWidth())								// Bottom Left Corner
				screen[i] = L'╚';
			if (i == (MainConsole.getHeight() - 1) * MainConsole.getWidth() + MainConsole.getWidth() - 1)	// Bottom Right Corner
				screen[i] = L'╝';
		}

		if (_kbhit() && _getch()) {
			switch (_getch()) {
			case 72: // code for arrow up
				if (nPlayerY != 1) nPlayerY--;
				break;

			case 80: // code for arrow down
				if (nPlayerY != MainConsole.getHeight() - 2) nPlayerY++;
				break;

			case 75: // code for arrow left
				if (nPlayerX != 1) nPlayerX--;
				break;

			case 77: // code for arrow right
				if (nPlayerX != MainConsole.getWidth() - 2)nPlayerX++;
				break;
			}
		}

		screen[nPlayerY * MainConsole.getWidth() + nPlayerX] = L'■';

		// Draw Everything to Console (always at the end of the loop)
		WriteConsoleOutputCharacter(hConsole, screen, MainConsole.getWidth() * MainConsole.getHeight(), { 0,0 }, &dwBytesWritten);
	}
	return 0;
}
