#pragma once
#include "GolfGameApp.h"
class MissionItem
{
protected:
	LPD3DXMESH m_ItemMesh = NULL;
	D3DXVECTOR3 m_ItemPos;
	D3DXMATRIXA16 _matObj;
	IDirect3DDevice9* g_pd3dDevice;
	IDirect3DTexture9* g_pTexture = 0;
	bool m_bIsInItem;
	D3DXMATRIXA16 matTrans;
	D3DXMATRIXA16 matRotate;
public:
	bool checkItem(D3DXVECTOR3 ballPos);
	void setUp(IDirect3DDevice9* pd3dDevice);
	void display();
	void CleanUp();	
	void mini_display();
	MissionItem();
	~MissionItem();
};

