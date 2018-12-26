#pragma once

#ifndef MINIMAP_H
#define MINIMAP_H

#include <d3dx9.h>
#include "Camera.h"

class MiniMapCam : public Camera
{
public:
	MiniMapCam();
	~MiniMapCam();

	void setDevice(IDirect3DDevice9* pd3dDevice);
	bool SetupMatrices();
	void CleanUp();

	D3DXMATRIXA16 mini_mat;
};

#endif
