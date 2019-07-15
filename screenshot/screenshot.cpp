﻿// ScreenCapture.cpp : Defines the entry point for the application.
//
#pragma once

#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <d3d9.h>
#include <d3dx9.h>


#include "ScreenCapture.h"
#include <stdio.h>
#include "avifile.h"
#include <shellapi.h>
#include <commdlg.h>

#define MAX_LOADSTRING 100

#define	WM_NOTIFYICON_MESSAGE	WM_USER+1000

#define	ErrorMessage(x)		MessageBox(NULL,x,"Error",MB_OK|MB_ICONERROR)

#define	WINDOW_MODE			true				//Not in fullscreen mode
#define BITSPERPIXEL		32

HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HWND	ghWnd;
HDC		hBackDC = NULL;
LPVOID	pBits = NULL;
HBITMAP	hBackBitmap = NULL;
HBITMAP	hOldBitmap = NULL;

int	nDisplayWidth;
int	nDisplayHeight;

IDirect3D9* g_pD3D = NULL;
IDirect3DDevice9* g_pd3dDevice = NULL;
IDirect3DSurface9* g_pSurface = NULL;

HRESULT Reset();
void	Render();
void	Cleanup();
HRESULT	InitD3D(HWND hWnd);
void	SaveBitmap(char* szFilename, HBITMAP hBitmap);

UINT_PTR	nTimerId = 0;
CAviFile* pAviFile = NULL;
HMENU		ghMenu = NULL;
RECT		gScreenRect = { 0,0,0,0 };
bool		gbCapturing = false;

int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SCREENCAPTURE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_SCREENCAPTURE);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_SCREENCAPTURE);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = NULL;//(HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = (LPCTSTR)IDC_SCREENCAPTURE;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{

	hInst = hInstance; // Store instance handle in our global variable

	ghWnd = CreateWindow(szWindowClass, szTitle, WS_POPUPWINDOW | WS_CAPTION | WS_VISIBLE,
		480, 400, 320, 240, NULL, NULL, hInstance, NULL);

	if (!ghWnd)
	{
		return FALSE;
	}

	ShowWindow(ghWnd, nCmdShow);
	UpdateWindow(ghWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	static NOTIFYICONDATA	nid;
	static RECT	clientRect = { 0,0,0,0 };
	static bool	bMinimized = false;

	switch (message)
	{
	case WM_SYSCOMMAND:
	{
		if (wParam == SC_CLOSE)
		{
			SendMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
			return 0;
		}
		if (wParam == SC_MINIMIZE)
		{
			bMinimized = true;
			LONG_PTR ret = DefWindowProc(hWnd, message, wParam, lParam);
			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(ID_FILE_HIDEWINDOW, 0), 0);
			return ret;
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	case WM_COMMAND: {
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_STARTCAPTURINGTOAVI:
		case ID_CAPTURE_START:
		{
			if (pAviFile == NULL)
			{
				OPENFILENAME	ofn;
				char	szFileName[512];
				strcpy(szFileName, "Output.avi");
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
				ofn.lpstrFilter = "Avi Files (*.avi)\0*.avi\0";
				ofn.lpstrDefExt = "avi";
				ofn.lpstrFile = szFileName;
				ofn.nMaxFile = 512;
				ofn.hwndOwner = hWnd;
				if (!GetSaveFileName(&ofn))	break;

				pAviFile = new CAviFile(szFileName);
			}

			nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_PAUSE));
			strcpy(nid.szTip, "Capturing the Screen - Double Click to Pause");

			if (!Shell_NotifyIcon(NIM_MODIFY, &nid))				//Modify the Icon State
				MessageBox(NULL, "Unable to Set Notification Icon", "Error", MB_ICONINFORMATION | MB_OK);

			SendMessage(hWnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);

			gbCapturing = true;

			break;
		}
		case ID_FILE_PAUSECAPTURINGTOAVI:
		case ID_CAPTURE_STOP:
		{
			nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_START));
			strcpy(nid.szTip, "Screen Capture Paused - Double Click to Resume");

			if (!Shell_NotifyIcon(NIM_MODIFY, &nid))				//Modify the Icon State
				MessageBox(NULL, "Unable to Set Notification Icon", "Error", MB_ICONINFORMATION | MB_OK);

			gbCapturing = false;

			break;
		}
		case ID_FILE_SHOWWINDOW:
		{
			if (bMinimized)	ShowWindow(hWnd, SW_RESTORE);
			else			ShowWindow(hWnd, SW_SHOW);
			SetForegroundWindow(hWnd);
			bMinimized = false;
			break;
		}
		case ID_FILE_HIDEWINDOW:
		{
			ShowWindow(hWnd, SW_HIDE);
			break;
		}
		case ID_FILE_CAPTURESCREENSHOT:
		case ID_EDIT_SAVEASBMP:
		{
			OPENFILENAME	ofn;
			char	szFileName[512];

			strcpy(szFileName, "ScreenShot.bmp");

			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.Flags = OFN_HIDEREADONLY | OFN_PATHMUSTEXIST;
			ofn.lpstrFilter = "Bitmap Files (*.bmp)\0*.bmp\0";
			ofn.lpstrDefExt = "bmp";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = 512;
			ofn.hwndOwner = hWnd;
			if (!GetSaveFileName(&ofn))	break;

			SetCursor(LoadCursor(NULL, IDC_WAIT));

			g_pd3dDevice->GetFrontBufferData(0, g_pSurface);
			D3DXSaveSurfaceToFile(szFileName, D3DXIFF_BMP, g_pSurface, NULL, NULL);		//Save to File

			D3DLOCKED_RECT	lockedRect;
			if (FAILED(g_pSurface->LockRect(&lockedRect, NULL, D3DLOCK_NO_DIRTY_UPDATE | D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY)))
			{
				ErrorMessage("Unable to Lock Front Buffer Surface");	break;
			}
			for (int i = 0; i < gScreenRect.bottom; i++)
			{
				memcpy((BYTE*)pBits + (gScreenRect.bottom - i - 1) * gScreenRect.right * BITSPERPIXEL / 8, (BYTE*)lockedRect.pBits + i * lockedRect.Pitch, gScreenRect.right * BITSPERPIXEL / 8);//g_d3dpp.BackBufferHeight*g_d3dpp.BackBufferWidth*4);				
			}
			g_pSurface->UnlockRect();
			InvalidateRect(hWnd, NULL, false);

			SetCursor(LoadCursor(NULL, IDC_ARROW));
			break;
		}
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	}
	case WM_INITMENU:
	{
		HMENU	hMenu = (HMENU)wParam;
		EnableMenuItem(hMenu, ID_FILE_SHOWWINDOW, IsWindowVisible(hWnd) ? MF_GRAYED : MF_ENABLED | MF_BYCOMMAND);
		EnableMenuItem(hMenu, ID_FILE_HIDEWINDOW, IsWindowVisible(hWnd) ? MF_ENABLED : MF_GRAYED | MF_BYCOMMAND);
		EnableMenuItem(hMenu, ID_FILE_CAPTURESCREENSHOT, gbCapturing ? MF_GRAYED | MF_BYCOMMAND : MF_ENABLED);
		EnableMenuItem(hMenu, ID_FILE_STARTCAPTURINGTOAVI, gbCapturing ? MF_GRAYED | MF_BYCOMMAND : MF_ENABLED);
		EnableMenuItem(hMenu, ID_FILE_PAUSECAPTURINGTOAVI, gbCapturing ? MF_ENABLED : MF_GRAYED | MF_BYCOMMAND);
		EnableMenuItem(hMenu, ID_CAPTURE_START, gbCapturing ? MF_GRAYED | MF_BYCOMMAND : MF_ENABLED);
		EnableMenuItem(hMenu, ID_CAPTURE_STOP, gbCapturing ? MF_ENABLED : MF_GRAYED | MF_BYCOMMAND);
		break;
	}
	case WM_INITMENUPOPUP:
	{
		EnableMenuItem(ghMenu, ID_FILE_SHOWWINDOW, IsWindowVisible(hWnd) ? MF_GRAYED : MF_ENABLED | MF_BYCOMMAND);
		EnableMenuItem(ghMenu, ID_FILE_HIDEWINDOW, IsWindowVisible(hWnd) ? MF_ENABLED : MF_GRAYED | MF_BYCOMMAND);
		EnableMenuItem(ghMenu, ID_FILE_CAPTURESCREENSHOT, gbCapturing ? MF_GRAYED | MF_BYCOMMAND : MF_ENABLED);
		EnableMenuItem(ghMenu, ID_FILE_STARTCAPTURINGTOAVI, gbCapturing ? MF_GRAYED | MF_BYCOMMAND : MF_ENABLED);
		EnableMenuItem(ghMenu, ID_FILE_PAUSECAPTURINGTOAVI, gbCapturing ? MF_ENABLED : MF_GRAYED | MF_BYCOMMAND);
		EnableMenuItem(ghMenu, ID_CAPTURE_START, gbCapturing ? MF_GRAYED | MF_BYCOMMAND : MF_ENABLED);
		EnableMenuItem(ghMenu, ID_CAPTURE_STOP, gbCapturing ? MF_ENABLED : MF_GRAYED | MF_BYCOMMAND);
		break;
	}
	case WM_CREATE:
	{
		LRESULT	ret = DefWindowProc(hWnd, message, wParam, lParam);
		if (ret < 0)	return ret;

		BITMAPINFO	bmpInfo;
		ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
		bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpInfo.bmiHeader.biBitCount = BITSPERPIXEL;
		bmpInfo.bmiHeader.biCompression = BI_RGB;
		bmpInfo.bmiHeader.biWidth = GetSystemMetrics(SM_CXSCREEN);
		bmpInfo.bmiHeader.biHeight = GetSystemMetrics(SM_CYSCREEN);
		bmpInfo.bmiHeader.biPlanes = 1;
		bmpInfo.bmiHeader.biSizeImage = abs(bmpInfo.bmiHeader.biHeight) * bmpInfo.bmiHeader.biWidth * bmpInfo.bmiHeader.biBitCount / 8;

		HDC	hdc = GetDC(GetDesktopWindow());
		hBackDC = CreateCompatibleDC(hdc);
		hBackBitmap = CreateDIBSection(hdc, &bmpInfo, DIB_RGB_COLORS, &pBits, NULL, 0);
		if (hBackBitmap == NULL)
		{
			ErrorMessage("Unable to Create BackBuffer Bitamp");		return -1;
		}
		ReleaseDC(GetDesktopWindow(), hdc);

		ZeroMemory(&nid, sizeof(nid));
		nid.cbSize = sizeof(nid);
		nid.uID = 1000;
		nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SCREENCAPTURE));
		nid.hWnd = hWnd;
		strcpy(nid.szTip, "Screen Capture Application - Double Click to Start Capturing");
		nid.uCallbackMessage = WM_NOTIFYICON_MESSAGE;
		if (!Shell_NotifyIcon(NIM_ADD, &nid))	MessageBox(NULL, "Unable to Set Notification Icon", "Error", MB_ICONINFORMATION | MB_OK);
		ghMenu = LoadMenu(hInst, MAKEINTRESOURCE(IDC_SCREENCAPTURE));

		if (FAILED(InitD3D(hWnd)))	return -1;

		nTimerId = SetTimer(hWnd, 12345, 500, NULL);	//Timer set to 500 ms.

		return ret;
	}
	case WM_CHAR:
	{
		if (wParam == VK_ESCAPE)
			DestroyWindow(hWnd);
		break;
	}
	case WM_TIMER:
	{
		if (gbCapturing == false)	break;

		g_pd3dDevice->GetFrontBufferData(0, g_pSurface);
		D3DLOCKED_RECT	lockedRect;
		if (FAILED(g_pSurface->LockRect(&lockedRect, &gScreenRect, D3DLOCK_NO_DIRTY_UPDATE | D3DLOCK_NOSYSLOCK | D3DLOCK_READONLY)))
		{
			ErrorMessage("Unable to Lock Front Buffer Surface");	break;
		}
		for (int i = 0; i < gScreenRect.bottom; i++)
		{
			memcpy((BYTE*)pBits + (gScreenRect.bottom - i - 1) * gScreenRect.right * BITSPERPIXEL / 8, (BYTE*)lockedRect.pBits + i * lockedRect.Pitch, gScreenRect.right * BITSPERPIXEL / 8);
		}
		g_pSurface->UnlockRect();

		InvalidateRect(hWnd, NULL, false);

		pAviFile->AppendNewFrame(gScreenRect.right, gScreenRect.bottom, pBits);			//Append this Captured ScreenShot to the Movie

		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT	ps;
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBackDC, hBackBitmap);
		HDC hdc = BeginPaint(hWnd, &ps);
		StretchBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, hBackDC, 0, 0, gScreenRect.right, gScreenRect.bottom, SRCCOPY);
		EndPaint(hWnd, &ps);
		SelectObject(hBackDC, hOldBitmap);
		break;
	}
	case WM_SIZE:
	{
		clientRect.right = LOWORD(lParam);
		clientRect.bottom = HIWORD(lParam);
		break;
	}
	case WM_NOTIFYICON_MESSAGE:
	{
		switch (lParam)
		{
		case WM_MOUSEMOVE:break;
		case WM_LBUTTONDBLCLK:
		{
			SendMessage(hWnd, WM_COMMAND, MAKEWPARAM(gbCapturing ? ID_FILE_PAUSECAPTURINGTOAVI : ID_FILE_STARTCAPTURINGTOAVI, 0), 0);
			break;
		}
		case WM_RBUTTONDOWN:
		{
			POINT pt; GetCursorPos(&pt);
			SetForegroundWindow(hWnd);
			TrackPopupMenu(GetSubMenu(ghMenu, 0), TPM_LEFTALIGN | TPM_BOTTOMALIGN, pt.x, pt.y, 0, hWnd, NULL);
			break;
		}
		}
		break;
	}
	case WM_DESTROY:
	{
		if (nTimerId)
		{
			KillTimer(hWnd, nTimerId);
			nTimerId = 0;
		}
		Cleanup();
		if (pAviFile)
		{
			delete pAviFile;
			pAviFile = NULL;
		}
		if (hBackDC)
		{
			DeleteDC(hBackDC);
			hBackDC = NULL;
		}
		if (hBackBitmap)
		{
			DeleteObject(hBackBitmap);
			hBackBitmap = NULL;
		}
		Shell_NotifyIcon(NIM_DELETE, &nid);
		DestroyMenu(ghMenu);
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

void Render()
{
	HRESULT	hr;
	if (g_pd3dDevice)
	{
		hr = g_pd3dDevice->TestCooperativeLevel();//Check Device Status - if Alt+tab or some such thing have caused any trouble
		if (hr != D3D_OK)
		{
			if (hr == D3DERR_DEVICELOST)	return;	//Device is lost - Do not render now 
			if (hr == D3DERR_DEVICENOTRESET)		//Device is ready to be acquired
			{
				if (FAILED(Reset()))
				{
					DestroyWindow(ghWnd);		//If Unable to Reset Device - Close the Application
					return;
				}
			}
		}
		g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 200), 1.0f, 0);
		g_pd3dDevice->BeginScene();
		g_pd3dDevice->EndScene();
		g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
	}
}

void Cleanup()
{
	if (g_pSurface)
	{
		g_pSurface->Release();
		g_pSurface = NULL;
	}
	if (g_pd3dDevice)
	{
		g_pd3dDevice->Release();
		g_pd3dDevice = NULL;
	}
	if (g_pD3D)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

HRESULT	InitD3D(HWND hWnd)
{
	D3DDISPLAYMODE	ddm;
	D3DPRESENT_PARAMETERS	d3dpp;

	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
	{
		ErrorMessage("Unable to Create Direct3D ");
		return E_FAIL;
	}

	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &ddm)))
	{
		ErrorMessage("Unable to Get Adapter Display Mode");
		return E_FAIL;
	}

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.Windowed = WINDOW_MODE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	d3dpp.BackBufferFormat = ddm.Format;
	d3dpp.BackBufferHeight = nDisplayHeight = gScreenRect.bottom = ddm.Height;
	d3dpp.BackBufferWidth = nDisplayWidth = gScreenRect.right = ddm.Width;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pd3dDevice)))
	{
		ErrorMessage("Unable to Create Device");
		return E_FAIL;
	}

	if (FAILED(g_pd3dDevice->CreateOffscreenPlainSurface(ddm.Width, ddm.Height, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &g_pSurface, NULL)))
	{
		ErrorMessage("Unable to Create Surface");
		return E_FAIL;
	}

	return S_OK;
}

HRESULT Reset()
{
	D3DDISPLAYMODE	ddm;
	D3DPRESENT_PARAMETERS	d3dpp;

	if (g_pSurface)														//Release the Surface - we need to get the latest surface
	{
		g_pSurface->Release();
		g_pSurface = NULL;
	}

	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &ddm)))	//User might have changed the mode - Get it afresh
	{
		ErrorMessage("Unable to Get Adapter Display Mode");
		return E_FAIL;
	}

	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	d3dpp.Windowed = WINDOW_MODE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	d3dpp.BackBufferFormat = ddm.Format;
	d3dpp.BackBufferHeight = nDisplayHeight = gScreenRect.bottom = ddm.Height;
	d3dpp.BackBufferWidth = nDisplayWidth = gScreenRect.right = ddm.Width;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = ghWnd;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;

	if (FAILED(g_pd3dDevice->Reset(&d3dpp)))
	{
		//ErrorMessage("Unable to Create Device");				//Dont present messages when device is lost
		return E_FAIL;
	}

	if (FAILED(g_pd3dDevice->CreateOffscreenPlainSurface(ddm.Width, ddm.Height, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &g_pSurface, NULL)))
	{
		//ErrorMessage("Unable to Create Surface");
		return E_FAIL;
	}

	return S_OK;
}

void	SaveBitmap(char* szFilename, HBITMAP hBitmap)
{
	HDC					hdc = NULL;
	FILE* fp = NULL;
	LPVOID				pBuf = NULL;
	BITMAPINFO			bmpInfo;
	BITMAPFILEHEADER	bmpFileHeader;

	do {

		hdc = GetDC(NULL);
		ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
		bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		GetDIBits(hdc, hBitmap, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS);

		if (bmpInfo.bmiHeader.biSizeImage <= 0)
			bmpInfo.bmiHeader.biSizeImage = bmpInfo.bmiHeader.biWidth * abs(bmpInfo.bmiHeader.biHeight) * (bmpInfo.bmiHeader.biBitCount + 7) / 8;

		if ((pBuf = malloc(bmpInfo.bmiHeader.biSizeImage)) == NULL)
		{
			MessageBox(NULL, _T("Unable to Allocate Bitmap Memory"), _T("Error"), MB_OK | MB_ICONERROR);
			break;
		}

		bmpInfo.bmiHeader.biCompression = BI_RGB;
		GetDIBits(hdc, hBitmap, 0, bmpInfo.bmiHeader.biHeight, pBuf, &bmpInfo, DIB_RGB_COLORS);

		if ((fp = fopen(szFilename, "wb")) == NULL)
		{
			MessageBox(NULL, _T("Unable to Create Bitmap File"), _T("Error"), MB_OK | MB_ICONERROR);
			break;
		}

		bmpFileHeader.bfReserved1 = 0;
		bmpFileHeader.bfReserved2 = 0;
		bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + bmpInfo.bmiHeader.biSizeImage;
		bmpFileHeader.bfType = 'MB';
		bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
		fwrite(&bmpInfo.bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp);
		fwrite(pBuf, bmpInfo.bmiHeader.biSizeImage, 1, fp);

	} while (false);

	if (hdc)
		ReleaseDC(NULL, hdc);

	if (pBuf)
		free(pBuf);

	if (fp)
		fclose(fp);
}