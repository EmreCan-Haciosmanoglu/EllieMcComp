#include <iostream>
#include <fstream>
#include <windows.h>
#include <chrono>
#include <atlimage.h> 
#include <Gdiplusimaging.h> 
#include <thread>

#include <time.h>
#include "NeuralNetwork.h"

#if 1
/* Globals */
int ScreenX = 0;
int ScreenY = 0;
BYTE* ScreenData = 0;

void ScreenCap()
{
	HDC hScreen = GetDC(NULL);
	ScreenX = GetDeviceCaps(hScreen, HORZRES);
	ScreenY = GetDeviceCaps(hScreen, VERTRES);

	HDC hdcMem = CreateCompatibleDC(hScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hScreen, ScreenX, ScreenY);
	HGDIOBJ hOld = SelectObject(hdcMem, hBitmap);
	BitBlt(hdcMem, 0, 0, ScreenX, ScreenY, hScreen, 0, 0, SRCCOPY);
	SelectObject(hdcMem, hOld);

	BITMAPINFOHEADER bmi = { 0 };
	bmi.biSize = sizeof(BITMAPINFOHEADER);
	bmi.biPlanes = 1;
	bmi.biBitCount = 32;
	bmi.biWidth = ScreenX;
	bmi.biHeight = -ScreenY;
	bmi.biCompression = BI_RGB;
	bmi.biSizeImage = 0;// 3 * ScreenX * ScreenY;

	if (ScreenData)
		free(ScreenData);
	ScreenData = (BYTE*)malloc(4 * ScreenX * ScreenY);

	GetDIBits(hdcMem, hBitmap, 0, ScreenY, ScreenData, (BITMAPINFO*)& bmi, DIB_RGB_COLORS);

	ReleaseDC(GetDesktopWindow(), hScreen);
	DeleteDC(hdcMem);
	DeleteObject(hBitmap);
}

inline int PosB(int x, int y)
{
	return ScreenData[4 * ((y * ScreenX) + x)];
}

inline int PosG(int x, int y)
{
	return ScreenData[4 * ((y * ScreenX) + x) + 1];
}

inline int PosR(int x, int y)
{
	return ScreenData[4 * ((y * ScreenX) + x) + 2];
}

bool ButtonPress(int Key)
{
	bool button_pressed = false;

	while (GetAsyncKeyState(Key))
		button_pressed = true;

	return button_pressed;
}

int main()
{
	while (true)
	{
		if (ButtonPress(VK_SPACE))
		{

			// Print out current cursor position
			POINT p;
			GetCursorPos(&p);
			printf("X:%d Y:%d \n", p.x, p.y);
			// Print out RGB value at that position
			std::cout << "Bitmap: r: " << PosR(p.x, p.y) << " g: " << PosG(p.x, p.y) << " b: " << PosB(p.x, p.y) << "\n";

		}
		else if (ButtonPress(VK_ESCAPE))
		{
			printf("Quit\n");
			break;
		}
		else if (ButtonPress(VK_SHIFT))
		{
			ScreenCap();
			printf("Captured\n");
		}
	}

	system("PAUSE");
	return 0;
}
/*int main() {
	srand(time(NULL));
	int layers[3] = { 2, 5 ,2 };
	NeuralNetwork* nn = new NeuralNetwork(layers, 3, 0.01f);
	int times = 2000000;
	for (int i = 0; i < times; i++)
	{
		if (0 == i % (times / 100))
		{
			int perc = (i / (times / 100));
			//system("CLS");
			for (int i = 0; i < perc; i++)
			{
				std::cout << "|";
			}
			for (int i = 0; i < 100 - perc; i++)
			{
				std::cout << " ";
			}
			std::cout << "| " << perc << "%" << std::endl;
		}
		int r = rand() % 4;
		if (r == 0)
		{
			float in[2] = { 1,0 };
			float ta[2] = { 1,0 };
			Matrix* input = new Matrix(2, 1, in);
			Matrix* target = new Matrix(2, 1, ta);
			nn->Train(input, target);
		}
		if (r == 1)
		{
			float in[2] = { 0, 1 };
			float ta[2] = { 1 ,0 };
			Matrix* input = new Matrix(2, 1, in);
			Matrix* target = new Matrix(2, 1, ta);
			nn->Train(input, target);
		}
		if (r == 2)
		{
			float in[2] = { 0,0 };
			float ta[2] = { 0,1 };
			Matrix* input = new Matrix(2, 1, in);
			Matrix* target = new Matrix(2, 1, ta);
			nn->Train(input, target);
		}
		if (r == 3)
		{
			float in[2] = { 1,1 };
			float ta[2] = { 0,1 };
			Matrix* input = new Matrix(2, 1, in);
			Matrix* target = new Matrix(2, 1, ta);
			nn->Train(input, target);
		}
	}

	float inputs[4][2] = { {1,0},{0,1},{1,1},{0,0} };
	Matrix* input0 = new Matrix(2, 1, inputs[0]);
	Matrix* result0 = nn->FeedForward(input0);
	result0->Print();
	delete result0;

	Matrix* input1 = new Matrix(2, 1, inputs[1]);
	Matrix* result1 = nn->FeedForward(input1);
	result1->Print();
	delete result1;

	Matrix* input2 = new Matrix(2, 1, inputs[2]);
	Matrix* result2 = nn->FeedForward(input2);
	result2->Print();
	delete result2;

	Matrix* input3 = new Matrix(2, 1, inputs[3]);
	Matrix* result3 = nn->FeedForward(input3);
	result3->Print();
	delete result3;

	delete nn;
	int wait = 0;
	std::cin >> wait;
	return 0;
}
*/
#endif
