#include "DisplayCount.h"





void DisplayCount::setUp(IDirect3DDevice9* pd3dDevice)
{
	g_pd3dDevice = pd3dDevice;
	D3DXCreateFont(g_pd3dDevice, 25, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &m_font);
	D3DXCreateSprite(g_pd3dDevice, &m_sprite);
	D3DXCreateSprite(g_pd3dDevice, &m_sprite);
}
void DisplayCount::Display(int Count)		//int a 넘겨와서 %d로 10 20 30 40 50 넘기게해보자
{
	if (m_sprite)
	{
		sprintf(numString, "Count : %d", Count);
		m_sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
		RECT rt = { 800,125,0,0 };
		m_font->DrawText(m_sprite, numString, -1, &rt, DT_NOCLIP, D3DCOLOR_XRGB(0, 0, 0));

		m_sprite->End();
	}
}
void DisplayCount::Cleanup()
{
	m_font->Release();
	m_sprite->Release();
	g_pd3dDevice->Release();
}

DisplayCount::DisplayCount()
{
}


DisplayCount::~DisplayCount()
{
}
