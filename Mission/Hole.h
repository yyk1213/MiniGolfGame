#pragma once
#include "GolfGameApp.h"
class Hole
{

protected:
	LPD3DXMESH m_HoleMesh = NULL;
	D3DXVECTOR3 m_holePos;
	D3DXMATRIXA16 _matObj;
	IDirect3DDevice9* g_pd3dDevice;
	IDirect3DTexture9* g_pTexture = 0;
	bool m_bIsInHole=false;
public:
	bool checkHole(D3DXVECTOR3 ballPos);
	void setUp(IDirect3DDevice9* pd3dDevice);
	void display();
	void mini_display();
	void CleanUp();
	D3DXVECTOR3 getHolePosition() { return m_holePos; };
	Hole();
	~Hole();
};

