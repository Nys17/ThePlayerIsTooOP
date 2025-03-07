// The main Windows / DirectX Graphics / XInput entry point for Star Applications

#define WIN32_LEAN_AND_MEAN
#define WM_LBUTTONDOWN    0x0201

#include <Windows.h>
#include "Engine/Implementation/DirectX11/DirectX11Graphics.h"
#include "Engine/Implementation/XInput/DirectXInput.h"
#include "Engine/IRenderable.h"
#include "Engine/ITexture.h"
#include "Engine/IShader.h"
#include "Engine/IApplication.h"

#include <list>;
#include <string>

const char WindowClassName[] = "Star - Nikolay Sotirov";
const char WindowTitle[] = "The player is too OP - Search for a Star 2024";
const int WindowWidth = 1920;
const int WindowHeight = 1080;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
IApplication* GetSceneManager(IGraphics* Graphics, IInput* Input);

IApplication* GetApplication(IGraphics* Graphics, IInput* Input);
IApplication* GetMainMenu(IGraphics* Graphics, IInput* Input);
IApplication* GetStatusAssignScreen(IGraphics* Graphics, IInput* Input);
IApplication* GetTutorialLevel(IGraphics* Graphics, IInput* Input);
IApplication* RenderNumericals(IGraphics* Graphics, IInput* Input);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	
	WNDCLASSEX wc;
	HWND hwnd;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WindowClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, WindowClassName, WindowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WindowWidth, WindowHeight, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	msg.message = WM_NULL;
	msg.wParam = -1;
	IGraphics * Graphics = new DirectX11Graphics(hwnd);
	IInput* Input = new DirectXInput();
	IApplication* SceneManager = GetSceneManager(Graphics, Input);
	IApplication* Application = GetApplication(Graphics, Input);
	IApplication* TitleScreen = GetMainMenu(Graphics, Input);
	IApplication* StatsInvest = GetStatusAssignScreen(Graphics, Input);
	
	IApplication* MainGame = GetTutorialLevel(Graphics, Input);// change to i application 
	IApplication* RNumbers = RenderNumericals(Graphics,Input);
	

	
	
	
		if (Graphics && Graphics->IsValid() && SceneManager)
		{
			SceneManager->Load();
			
			while (msg.message != WM_QUIT && SceneManager->IsValid())
			{
				if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				Input->Update();
				SceneManager->Update();
				Graphics->Update();
				
			}

			SceneManager->Cleanup();
		}

		if (SceneManager)
		{
			delete	SceneManager;

		}

		if (Graphics)
		{
			delete Graphics;
		}
		
	


	return static_cast<int>(msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}
