#pragma once
#include <d3dx9.h>
#include <stdio.h>
#include <Windows.h>
//업데이트 함수 상태별 표시

enum gameType { READY, SHOT, POWERWAIT,BASIC,CLOSE };
extern gameType g_eType;

namespace d3d
{
	//함수
	bool InitD3D(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType, IDirect3DDevice9** device);
	int EnterMsgLoop(bool(&ptr_display)(float timeDelta));

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	template<class T>void Release(T t)
	{
		if (t)
		{
			t->Release();
			t = 0;
		}
	}
	template<class T> void Delete(T t)
	{
		if (t)
		{
			t->Delete();
			t = 0;
		}
	}
}