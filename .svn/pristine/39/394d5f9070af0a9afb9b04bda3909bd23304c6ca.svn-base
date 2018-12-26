#pragma once
#ifndef INTROSCENE_H
#define INTROSCENE_H

#include <d3dx9.h>

class Intro {
public:
	LPDIRECT3DDEVICE9 g_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 intro_buf;
	LPDIRECT3DTEXTURE9 intro_tex;

	D3DXMATRIXA16 introMat;

public:
	Intro() {};
	~Intro() { CleanUp(); };
	void CleanUp();
	bool Init(LPDIRECT3DDEVICE9 device);
	bool LoadTexture();
	void Render();
	bool SetUpMatrices();
};

#endif