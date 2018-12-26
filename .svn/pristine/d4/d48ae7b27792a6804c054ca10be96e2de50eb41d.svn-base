#pragma once

#ifndef MINIMAP_H
#define MINIMAP_H

#include <d3dx9.h>
#include "Camera.h"

class MiniMap : public Camera
{
public:
	MiniMap();
	~MiniMap();

	void setDevice(IDirect3DDevice9* pd3dDevice);
	bool createTexture();
	void setUp();
	bool SetupMatrices();
	
	IDirect3DTexture9* texture;
protected:
	
};

#endif
