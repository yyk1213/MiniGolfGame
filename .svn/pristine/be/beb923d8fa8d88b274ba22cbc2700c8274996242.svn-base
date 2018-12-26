#pragma once
#include "GolfGameApp.h"
class DisplayCount
{
public:
	DisplayCount();
	~DisplayCount();
	void setUp(IDirect3DDevice9* pd3dDevice);
	void Display(int powerNum);
	void Cleanup();
protected:
	ID3DXFont* m_font;
	ID3DXSprite* m_sprite;
	IDirect3DDevice9* g_pd3dDevice;
	char numString[10];
};

