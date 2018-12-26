#pragma once
#include "GolfGameApp.h"
#include "BasicFunction.h"

#include <conio.h>

//-----------------------------------------------------------------------------//

// WndProc

//-----------------------------------------------------------------------------//

LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) // ESC 키를 누르면 프로그램을 종료한다
			DestroyWindow(hwnd);
		break;
	case WM_MOUSEMOVE:
		if(g_eType==POWERWAIT)
		g_WaitCamera.CalcMouseAngle(hwnd, lParam);
		break;
	case WM_LBUTTONDOWN:
		g_WaitCamera.SaveMouseInitPos(hwnd, lParam);
		break;
	case WM_LBUTTONUP:
		ReleaseCapture();
		g_WaitCamera.isLBClicked = FALSE;
		break;
	default:
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


//-----------------------------------------------------------------------------//

// WinMain

//-----------------------------------------------------------------------------//

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int showCmd)
{
	if (!d3d::InitD3D(hInstance, Width, Height, true, D3DDEVTYPE_HAL, &g_pd3dDevice))
	{
		MessageBox(0, TEXT("InitD3D() - FAILED"), 0, 0);

		return 0;

	}
	
	BeginScreen g_Begin;		//시작화면 세팅
	g_Begin.Setup(g_pd3dDevice);
	g_Begin.display();

	while(1)
	{
 		if (GetAsyncKeyState(VK_SPACE)& 0x0001f)		//스페이스바 누르면 넘어가게 만듬
		{
			break;
		}
	}
	g_Begin.CleanUp();
	//Sleep(3000);
	if (!Setup())
	{
		MessageBox(0, TEXT("Setup() - FAILED"), 0, 0);

		return 0;

	}

	d3d::EnterMsgLoop(Display); // 메시지 루프
								// 메시지 루프를 빠져나오면 종료 작업
	Cleanup();
	GameOver g_GameOver;
	if (g_bisInHall)
	{


		if (g_iSwingCount == 1)
		{
			g_GameOver.Setup(g_pd3dDevice, "5.png");
			g_GameOver.display();
		}
		else if (g_iSwingCount == 2)
		{
			g_GameOver.Setup(g_pd3dDevice, "4.png");
			g_GameOver.display();
		}
		else if (g_iSwingCount == 3)
		{
			g_GameOver.Setup(g_pd3dDevice, "3.png");
			g_GameOver.display();
		}
		else if (g_iSwingCount == 4)
		{
			g_GameOver.Setup(g_pd3dDevice, "2.png");
			g_GameOver.display();
		}
		else if (g_iSwingCount >= 5)
		{
			g_GameOver.Setup(g_pd3dDevice, "1.png");
			g_GameOver.display();
		}
	}
	else 
	{
		g_GameOver.Setup(g_pd3dDevice, "gameover.png");
		g_GameOver.display();
	}
	
	
  	while (1)
	{
		if (GetAsyncKeyState(VK_SPACE) & 0x0001f)		//스페이스바 누르면 넘어가게 만듬
		{
			break; 
		}
	}
	g_GameOver.CleanUp();
	g_pd3dDevice->Release();
	return 0;

}
