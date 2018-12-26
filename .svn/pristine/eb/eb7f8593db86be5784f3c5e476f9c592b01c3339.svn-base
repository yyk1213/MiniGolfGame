#pragma once
#ifndef POWER_H
#define POWER_H

#include "GolfGameApp.h"
class PowerProcess
{
public:
	PowerProcess();
	~PowerProcess();

	void setUp(IDirect3DDevice9* pd3dDevice);
	void Display(int powerNum);
	void Cleanup();
	void keyState(int* power);
protected:
	ID3DXFont* m_font;
	ID3DXSprite* m_sprite;
	IDirect3DDevice9* g_pd3dDevice;
	char numString[10];
	bool m_bKeyState[6];
};


#endif