
#pragma once
#include "GolfGameApp.h"
#include "Vertex.h"
class GameOver
{

	public:
		void Setup(IDirect3DDevice9* pd3dDevice, LPCSTR _texFileName);
		void CleanUp();
		void display();
		GameOver();
		~GameOver();
	protected:
		IDirect3DDevice9*     g_pd3dDevice = 0;

		const int Width = 640;
		const int Height = 480;

		IDirect3DVertexBuffer9* m_vb = 0;
		IDirect3DTexture9*      m_tex = 0;



};

