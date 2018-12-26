#pragma once
#ifndef SKY_H
#define SKY_H

#include<d3dx9.h>
#define TEX_NUM 6

class Sky {
protected:
	LPDIRECT3DDEVICE9 g_pd3dDevice;
	LPDIRECT3DVERTEXBUFFER9 sky_Buf;
	LPDIRECT3DTEXTURE9     sky_Tex[TEX_NUM];

	LPCSTR sky_texList[TEX_NUM];

	D3DXMATRIXA16 skyMat;
public:
	Sky() {};
	~Sky() {
		CleanUp();
	};
public:
	void CleanUp();
	bool Init(LPDIRECT3DDEVICE9 device);
	bool LoadTexture(LPCSTR sky_texList[TEX_NUM]);
	void Render();
	bool SetUpMatrices();

	struct Sky_Vertex
	{
		Sky_Vertex() {}
		Sky_Vertex(
			float x, float y, float z,
			float u, float v)
		{
			_x = x;  _y = y;  _z = z;
			_u = u;  _v = v;
		}
		float _x, _y, _z;
		float _u, _v; // texture coordinates
	};

	//const DWORD Sky_Vertex::FVF = D3DFVF_XYZ | D3DFVF_TEX1;

};

#endif