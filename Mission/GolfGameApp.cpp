#pragma once
#include "GolfGameApp.h"
#include <string>
#include <Windows.h>
#include <stdio.h>
#include <math.h>

bool d3d::InitD3D(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType, IDirect3DDevice9** device)
{
	//메인 윈도우
	WNDCLASS wc;



	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)d3d::WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = TEXT("D3DSample");

	if (!RegisterClass(&wc))
	{
		MessageBox(0, TEXT("FAILED"), 0, 0);
		return false;
	}

	HWND hwnd = 0;

	hwnd = CreateWindow(TEXT("D3DSample"), TEXT("GolfGame"), WS_EX_TOPMOST, 0, 0, width, height, 0, 0, hInstance, 0);




	if (!hwnd)
	{
		MessageBox(0, TEXT("FAILE11111D"), 0, 0);
		return false;
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	//Init D3D
	HRESULT hr = 0;
	//1.IDirect3D9 객체 생성

	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d9)
	{
		MessageBox(0, TEXT("FAILE2222D"), 0, 0);
		return false;
	}

	D3DCAPS9 caps;	//기본 구조체 생성		(장치특성알아봄)
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);
	//첫번째 인자로 기본디스플레이 어뎁터 지정,2. 장치 타입 지정,3. 초기화된 특성 구조체 리턴

	int vp = 0;

	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)		//하드웨어 버텍스 프로세싱을 이용가능한가?
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING; //가능하면 하드웨어 버텍스프로세싱 저장
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;	//불가능하면 소프트웨어 버텍스프로세싱저장
													//(vp는 객체만들 때 필요하기 때문에 저장)
	d3d9->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, deviceType, D3DFMT_A8R8G8B8, false, D3DMULTISAMPLE_16_SAMPLES, NULL);


	//D3DPRESENT_PARAMETERS 구조체 채우기	(IDirect3DDevice9 객체 특성 결정하는데 이용)
	D3DPRESENT_PARAMETERS d3dpp;

	d3dpp.BackBufferWidth = width;		//후면버퍼 너비
	d3dpp.BackBufferHeight = height;	//후면버퍼 높이
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;	//픽셀 포맷
	d3dpp.BackBufferCount = 1;	//버퍼의 수
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;	//멀티샘플링을 사용하는지(2~16단계) ->계단현상제거->성능저하
	d3dpp.MultiSampleQuality = 0; //멀티 샘플링 레벨
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//버퍼 교환되는방법 (고정)
	d3dpp.hDeviceWindow = hwnd;	//애플리케이션 윈도우 지정
	d3dpp.Windowed = windowed;	//true면 윈도우모두 false면 전체화면모드
	d3dpp.EnableAutoDepthStencil = true;	// 깊이/스텐실버퍼 자동관리 true
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;	//24깊이버퍼 8스텐실버퍼예약
	d3dpp.Flags = 0;	//부가적특성
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//화면재생률
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;	//시연간격 (보통 수직동기화)


																//장치 생성(IDirect3DDevice9 객체 생성)
	hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hwnd, vp, &d3dpp, device);

	if (FAILED(hr))
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hwnd, vp, &d3dpp, device);

		if (FAILED(hr))
		{
			d3d9->Release();
			MessageBox(0, TEXT("FAILED"), 0, 0);
			return false;
		}
	}

	d3d9->Release();

	return true;
}

int d3d::EnterMsgLoop(bool(&ptr_display)(float timeDelta))
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	static float lastTime = (float)timeGetTime();
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			float currTime = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;

			int ff   = ptr_display(timeDelta);
			if (!ff)
			{
				break;
			}lastTime = currTime;
		}
	}

	return msg.wParam;
}

