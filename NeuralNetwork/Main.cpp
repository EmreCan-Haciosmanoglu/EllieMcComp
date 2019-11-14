#include <iostream>
#include <fstream>
#include <windows.h>
#include <atlimage.h> 
#include <Gdiplusimaging.h> 
#include <thread>

#include <time.h>
#include "NeuralNetwork.h"
#include <chrono>
#include <vector>

#if 1
struct ProfileResult
{
	const char* Name;
	float Time;
};

/* Globals */
int ScreenX = 0;
int ScreenY = 0;
BYTE* ScreenData = 0;
float* RGBData = 0;
std::vector<ProfileResult> m_ProfileResults;


class Timer
{
public:
	Timer(const char* name)
		: m_Name(name), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_Stopped)
			Stop();
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) * 0.001f;
		m_ProfileResults.push_back({ m_Name, duration });
	}
private:
	const char* m_Name;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
};


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
inline void Prepare()
{
	int size = 3 * ScreenX * ScreenY;
	if (RGBData)
		free(RGBData);
	RGBData = (float*)malloc(size * sizeof(float));
	BYTE* ptrOld = ScreenData;
	float* ptrNew = RGBData;
	for (int y = 0; y < ScreenY; y++)
	{
		for (int x = 0; x < ScreenX; x++)
		{
			ptrNew[0] = ptrOld[0] / 255.0f;
			ptrNew[1] = ptrOld[1] / 255.0f;
			ptrNew[2] = ptrOld[2] / 255.0f;

			ptrOld += 4;
			ptrNew += 3;
		}
	}
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
	srand((unsigned int)time(NULL));
	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);
	int layers[3] = { x * y * 3, 5 , 50 };
	NeuralNetwork* nn = new NeuralNetwork(layers, 3, 0.01f);
	for (int k = 0; k < 5; k++)
	{
		{
			Timer time("Total Process");
			{
				Timer time("ScreenShot");
				ScreenCap();
			}
			{
				Timer time("Prepare the data");
				Prepare();
			}
			{
				Timer time("NeuralNetwork Calculations");

				int size = 3 * ScreenX * ScreenY;
				Matrix* input = new Matrix(size, 1, RGBData);
				Matrix* result = nn->FeedForward(input);
				delete result;
			}

			for (auto& result : m_ProfileResults)
			{
				char label[50];
				strcpy_s(label, "%.3fms ");
				strcat_s(label, result.Name);
				strcat_s(label, "\n");
				printf_s(label, result.Time);
			}
		}
		printf("\n");
		m_ProfileResults.clear();
	}

	system("PAUSE");
	return 0;
}
/*int main() {
	int times = 2000000;
	for (int i = 0; i < times; i++)
	{

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
