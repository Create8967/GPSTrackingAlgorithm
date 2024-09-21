/* B I R D F U L  G R A P H I C S
   Console Graphics by Uniprism
*/
#pragma once

#include <iostream>
#include <math.h>
#include <string>
#include <io.h>
#include <fcntl.h>
#include <Windows.h>
#include <chrono>
#include "BirdfulText.h"

using namespace std;
namespace bird {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);

	int ScrH = 32; // Screen and pixel sizes
	int ScrW = 32;
	int pw = 2;
	int ph = 2;
	float pi = 3.141592653f;

	CHAR_INFO* buf = new CHAR_INFO[ScrW * ScrH];

	void hidecursor() {
		HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO info;
		info.dwSize = 100;
		info.bVisible = TRUE;
		SetConsoleCursorInfo(consoleHandle, &info);
	}

	void PxlSize(int w, int h)
	{
		CONSOLE_FONT_INFOEX cfi;
		cfi.cbSize = sizeof(cfi);
		cfi.nFont = 0;
		cfi.dwFontSize.X = w;                   // Width of each character in the font
		cfi.dwFontSize.Y = h;                  // Height
		cfi.FontFamily = FF_DONTCARE;
		cfi.FontWeight = FW_NORMAL;
		wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
		SetCurrentConsoleFontEx(hStdOut, FALSE, &cfi);

	}

	void NoSelect()
	{
		HANDLE hInput;
		DWORD prev_mode;
		hInput = GetStdHandle(STD_INPUT_HANDLE);
		GetConsoleMode(hInput, &prev_mode);
		SetConsoleMode(hInput, prev_mode & ENABLE_EXTENDED_FLAGS);
	}
	bool IsBound(int x, int y)
	{
		bool val = false;
		if (x >= 0 && x < ScrW)
		{
			if (y >= 0 && y < ScrH)
			{
				val = true;
			}
		}
		return val;
	}
	void Draw(int x, int y, int col, int shade)
	{
		if (IsBound(x, y))
		{
			WCHAR ch = 0x2588;
			switch (shade)
			{
			case 0:
				ch = 0x2591;
				break;
			case 1:
				ch = 0x2592;
				break;
			case 2:
				ch = 0x2593;
				break;
			case 3:
				ch = 0x2588;
				break;
			}
			//Full block = 0x2588, Dark shade = 0x2593, Medium shade = 0x2592, Light shade = 0x2591
			int pos = y * ScrW + x;
			buf[pos].Char.UnicodeChar = ch;
			buf[pos].Attributes = 15 * 16 + col;
		}
	}
	void DrawC(int x, int y, int col, int colB, int shade)
	{
		if (IsBound(x, y))
		{
			short ch = 0x2591;
			switch (shade)
			{
			case 0:
				ch = 0x2593;
			case 1:
				ch = 0x2592;
			case 2:
				ch = 0x2591;
			case 3:
				ch = 0x2588;
			}
			//Full block = 0x2588, Dark shade = 0x2593, Medium shade = 0x2592, Light shade = 0x2591
			int pos = y * ScrW + x;
			buf[pos].Char.UnicodeChar = ch;
			buf[pos].Attributes = colB * 16 + col;
		}
	}
	void Line(int x1, int y1, int x2, int y2, int c)
	{
		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1; dy = y2 - y1;
		dx1 = abs(dx); dy1 = abs(dy);
		px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
		if (dy1 <= dx1)
		{
			if (dx >= 0)
			{
				x = x1; y = y1; xe = x2;
			}
			else
			{
				x = x2; y = y2; xe = x1;
			}

			Draw(x, y, c, 3);

			for (i = 0; x < xe; i++)
			{
				x = x + 1;
				if (px < 0)
					px = px + 2 * dy1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				Draw(x, y, c, 3);
			}
		}
		else
		{
			if (dy >= 0)
			{
				x = x1; y = y1; ye = y2;
			}
			else
			{
				x = x2; y = y2; ye = y1;
			}

			Draw(x, y, c, 3);

			for (i = 0; y < ye; i++)
			{
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				Draw(x, y, c, 3);
			}
		}
	}
	void Bline(int x1, int y1, int x2, int y2, int c) {
		int d = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
		if (d > 0) {
			float dx = float(x2 - x1) / d;
			float dy = float(y2 - y1) / d;

			for (int i = 0; i < d; i++) {
				Draw(x1 + (dx * i), y1 + (dy * i), c, 4);
			}
		}
	}

	void CCLine(int x1, int y1, int x2, int y2, int sizX, int sizY, int* b)
	{
		int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
		dx = x2 - x1; dy = y2 - y1;
		dx1 = abs(dx); dy1 = abs(dy);
		px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;
		if (dy1 <= dx1)
		{
			if (dx >= 0)
			{
				x = x1; y = y1; xe = x2;
			}
			else
			{
				x = x2; y = y2; xe = x1;
			}

			if ((x >= 0 && x < sizX) && (y >= 0 && y < sizY)) {
				int c = x + (y * sizX);
				if (c >= 0 && c < (sizX * sizY)) { b[c] = 1; }

			}

			for (i = 0; x < xe; i++)
			{
				x = x + 1;
				if (px < 0)
					px = px + 2 * dy1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
					px = px + 2 * (dy1 - dx1);
				}
				if ((x >= 0 && x < sizX) && (y >= 0 && y < sizY)) {
					int c = x + (y * sizX);
					if (c >= 0 && c < (sizX * sizY)) { b[c] = 1; }

				}
			}
		}
		else
		{
			if (dy >= 0)
			{
				x = x1; y = y1; ye = y2;
			}
			else
			{
				x = x2; y = y2; ye = y1;
			}

			if ((x >= 0 && x < sizX) && (y >= 0 && y < sizY)) {
				int c = x + (y * sizX);
				if (c >= 0 && c < (sizX * sizY)) { b[c] = 1; }

			}

			for (i = 0; y < ye; i++)
			{
				y = y + 1;
				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
					py = py + 2 * (dx1 - dy1);
				}
				if ((x >= 0 && x < sizX) && (y >= 0 && y < sizY)) {
					int c = x + (y * sizX);
					if (c >= 0 && c < (sizX * sizY)) { b[c] = 1; }

				}
			}
		}
	}

	void Triangle(int x1, int y1, int x2, int y2, int x3, int y3, int col)
	{
		Line(x1, y1, x2, y2, col);
		Line(x2, y2, x3, y3, col);
		Line(x3, y3, x1, y1, col);
	}
	void Cline(int x1, int y1, int x2, int y2, int sizX, int sizY, int* b) {
		int d = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
		if (d > 0) {
			float dx = float(x2 - x1) / d;
			float dy = float(y2 - y1) / d;

			for (int i = 0; i < d; i++) {
				int bx = int(x1 + (dx * i));
				int by = int(y1 + (dy * i));
				int c = bx + (by * sizX);
				if ((bx >= 0 && bx < sizX) && (by >= 0 && by < sizY)) {
					int c = bx + (by * sizX);
					if (c >= 0 && c < (sizX * sizY)) { b[c] = 1; }

				}
			}
		}
	}
	void Dline(int x1, int y1, int x2, int y2, int sizX, int sizY, int* be) {
		int d = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
		if (d > 0) {
			float dx = float(x2 - x1) / d;
			float dy = float(y2 - y1) / d;

			for (int i = 0; i < d; i++) {
				int bx = int(x1 + (dx * i));
				int by = int(y1 + (dy * i));
				int c = bx + (by * sizX);
				if ((bx >= 0 && bx < sizX) && (by >= 0 && by < sizY)) {
					be[by * 2] = min(be[by * 2], bx); // Min
					be[by * 2 + 1] = max(be[by * 2], bx); // Max

				}
			}
		}
	}
	void CTri(int x1, int y1, int x2, int y2, int x3, int y3, int sizX, int sizY, int* b) {
		Cline(x1, y1, x2, y2, sizX, sizY, b);
		Cline(x2, y2, x3, y3, sizX, sizY, b);
		Cline(x3, y3, x1, y1, sizX, sizY, b);
	}
	void DTri(int x1, int y1, int x2, int y2, int x3, int y3, int sizX, int sizY, int* b) {
		Dline(x1, y1, x2, y2, sizX, sizY, b);
		Dline(x2, y2, x3, y3, sizX, sizY, b);
		Dline(x3, y3, x1, y1, sizX, sizY, b);
	}

	void DFillTri(int x1, int y1, int x2, int y2, int x3, int y3, int col) {
		int maxY = max(max(y1, y2), y3) + 1;
		int minY = min(min(y1, y2), y3);
		int maxX = max(max(x1, x2), x3) + 1;
		int minX = min(min(x1, x2), x3);

		int* buffy = (int*)calloc(2 * (maxY - minY), sizeof(int));
		for (int i = 0; i < (maxY - minY); i++) {
			buffy[i * 2] = 100000; // Min to max
			buffy[i * 2 + 1] = -1; // Max to min
		}
		DTri(x1 - minX, y1 - minY, x2 - minX, y2 - minY, x3 - minX, y3 - minY, (maxX - minX), (maxY - minY), buffy);

		for (int ye = 0; ye < (maxY - minY); ye++) {

			for (int xval = buffy[ye * 2]; xval < buffy[ye * 2 + 1]; xval++) {
				Draw(xval + minX, ye + minY, col, 4);
			}
		}
		free(buffy);

	}
	void FillTri(int x1, int y1, int x2, int y2, int x3, int y3, int col) {
		int maxY = max(max(y1, y2), y3) + 1;
		int minY = min(min(y1, y2), y3);
		int maxX = max(max(x1, x2), x3) + 1;
		int minX = min(min(x1, x2), x3);

		int* buffy = (int*)calloc((maxX - minX + 1) * (maxY - minY + 1), sizeof(int));

		CTri(x1 - minX, y1 - minY, x2 - minX, y2 - minY, x3 - minX, y3 - minY, (maxX - minX), (maxY - minY), buffy);

		for (int ye = 0; ye < (maxY - minY); ye++) {
			bool on = false;
			bool hardr = false;
			int minn = 0;
			int maxx = 0;

			for (int xar = 0; xar < (maxX - minX); xar++) {
				minn = xar;
				if (buffy[xar + (ye * (maxX - minX))] == 1) break;
			}
			for (int xer = (maxX - minX) - 1; xer >= 0; xer--) {
				maxx = xer;
				if (buffy[xer + (ye * (maxX - minX))] == 1) break;
			}
			for (int xr = minn; xr <= maxx; xr++) {
				Draw(xr + minX, ye + minY, col, 4);

			}
		}
		free(buffy);

	}
	char GetDrawP(int x, int y)
	{
		return buf[y * ScrW + x].Char.UnicodeChar;
	}
	int GetDrawC(int x, int y)
	{
		return buf[y * ScrW + x].Attributes;
	}
	void setBuf()
	{
		COORD sumCoords = { ScrW, ScrH };
		SetConsoleScreenBufferSize(hStdOut, sumCoords);
	}
	void Fill(int x1, int y1, int x2, int y2, int col)
	{
		for (int i = y1; i < y2; i++)
		{
			for (int j = x1; j < x2; j++)
				Draw(j, i, col, 3);
		}
	}
	void FillB(int x1, int y1, int x2, int y2, int col)
	{
		for (int i = y1; i < y2; i++)
		{
			for (int j = x1; j < x2; j++)
			{
				if (i == y1 || i == y2 - 1)
					Draw(j, i, col, 3);
				Draw(j, i, col, 3);
				if (j == x1 || j == x2 - 1)
					Draw(j, i, col, 3);
			}
		}
	}
	void clear()
	{
		for (int i = 0; i < ScrW * ScrH; i++)
		{
			buf[i].Char.UnicodeChar = 0x2588;
			buf[i].Attributes = 15;
		}
	}

	void DrawCirc(int x, int y, int r, int col)
	{
		for (int i = y - r; i < y + r; i++)
		{
			for (int j = x - r; j < x + r; j++)
			{
				if (((i - y) * (i - y)) + ((j - x) * (j - x)) < r * r)
				{
					Draw(j, i, col, 3);
				}
			}
		}
	}
	void DrawCirc(int x, int y, int r, int col, bool low)
	{
		if (low) {
			for (int i = y - r + 1; i < y + r; i += 2)
			{
				for (int j = x - r + 1; j < x + r; j += 2)
				{
					if (((i - y) * (i - y)) + ((j - x) * (j - x)) < r * r)
					{
						Draw(j, i, col, 3);
					}
				}
			}
		}
		else {
			for (int i = y - r; i < y + r; i++)
			{
				for (int j = x - r; j < x + r; j++)
				{
					if (((i - y) * (i - y)) + ((j - x) * (j - x)) < r * r)
					{
						Draw(j, i, col, 3);
					}
				}
			}
		}
	}
	void DrawCircLP(int x, int y, int r, int thk, int col)
	{
		float r2 = (r - thk) * (r - thk);
		float r22 = abs(r - thk);
		// Find distance from max size square inside circle [sqrt(2r^2) - r]
		int sqOver = sqrtf(2 * r22 * r22) - r22;
		for (int i = y - r; i < y + r; i++)
		{
			for (int j = x - r; j < x + r; j++)
			{
				if (i > (y - r) + sqOver && i < (y + r) - sqOver) { // Might increase efficiency

					if (j > (x - r) + sqOver && j < (x + r) - sqOver) {
						//Draw(j, i, 6, 3);
						j = x + r - sqOver;
						//Draw(i, j, 6, 3);
					}
				}
				if (((i - y) * (i - y)) + ((j - x) * (j - x)) < r * r && ((i - y) * (i - y)) + ((j - x) * (j - x)) >= r2)
				{
					Draw(j, i, col, 3);
				}
			}
		}
	}

	void setup(int w, int h, int wid, int hig)
	{
		HWND hwnd = GetConsoleWindow();
		ScrW = wid;
		ScrH = hig;
		pw = w;
		ph = h;
		buf = new CHAR_INFO[ScrW * ScrH];
		PxlSize(w, h);
		NoSelect();
		hidecursor();
		setBuf();
		MoveWindow(hwnd, 0, 0, wid * w, hig * h, TRUE);
	}

	void update()
	{
		SMALL_RECT window = { 0,0 ,ScrW, ScrH };
		COORD BufSize = { ScrW, ScrH };
		WriteConsoleOutputW(hStdOut, buf, BufSize, { 0,0 }, &window);
	}
	bool GetKey(char Key)
	{
		return GetAsyncKeyState((unsigned short)Key) & 0x8000;
	}
	bool GetLeftMouseDown() {
		return GetAsyncKeyState(VK_LBUTTON);
	}
	bool GetRightMouseDown() {
		return GetAsyncKeyState(VK_RBUTTON);
	}
	bool GetMiddleMouseDown() {
		return GetAsyncKeyState(VK_MBUTTON);
	}
	void GetMousePos(PPOINT mouse) {
		HWND hwnd = GetConsoleWindow();

		POINT temp_mouse = *mouse;

		GetCursorPos(&temp_mouse);
		ScreenToClient(hwnd, &temp_mouse);
		temp_mouse.x /= pw;
		temp_mouse.y /= ph;
		*mouse = temp_mouse;
	}
	void drawChar(char c, int x, int y, int fcol) {
		Bchar p = BirdfulText::getChar(c);
		for (int i = 0; i < 6; i++) {
			for (int j = 0; j < 5; j++) {
				int e = i * 5 + j;

				if (p.pix[e] == true) {
					Draw(x + j, y + i, fcol, 3);
				}
				/*else {
					Draw(x + j, y + i, bcol, 3);
				}*/
			}
		}
	}

	void Bprint(const char* text, int x, int y, int fcol) {
		for (int i = 0; i < sizeof(text) + 1; i++) {
			drawChar(*(text + i), x + i * 6, y, fcol);
		}
	}
	void Bprint(int num, int x, int y, int fcol) {
		string text = std::to_string(num);
		for (int i = 0; i < sizeof(text); i++) {
			drawChar(*(text.c_str() + i), x + i * 6, y, fcol);
		}
	}
	void Bprint(float num, int x, int y, int fcol) {
		string text = std::to_string(num);
		for (int i = 0; i < sizeof(text); i++) {
			drawChar(*(text.c_str() + i), x + i * 6, y, fcol);
		}
	}
	void Bprint(double num, int x, int y, int fcol) {
		string text = std::to_string(num);
		for (int i = 0; i < sizeof(text); i++) {
			drawChar(*(text.c_str() + i), x + i * 6, y, fcol);
		}
	}
}