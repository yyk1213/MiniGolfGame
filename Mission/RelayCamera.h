#pragma once

#ifndef RELAYCAMERA_H
#define RELLAYCAMERA_H
#include "Camera.h"

class RelayCamera : public Camera
{
public: 
	virtual void Update(D3DXVECTOR3 ballPosition);
	virtual void SetUp();
	virtual void setDevice(IDirect3DDevice9* pd3dDevice);
	void CleanUp();

	RelayCamera();
	~RelayCamera() {};
};

#endif