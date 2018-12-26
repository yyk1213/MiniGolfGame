#pragma once
#include "GolfGameApp.h"
#include "Vertex.h"
class BeginScreen
{
public:
	void Setup(IDirect3DDevice9* pd3dDevice);
	void CleanUp();
	void display();
	BeginScreen();
	~BeginScreen();
protected:
	IDirect3DDevice9*     g_pd3dDevice = 0;

	const int Width = 640;
	const int Height = 480;

	IDirect3DVertexBuffer9* m_vb = 0;
	IDirect3DTexture9*      m_tex = 0;
};

