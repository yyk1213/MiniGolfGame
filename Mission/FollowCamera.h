#pragma once
#ifndef FOLLOWCAMERA_H
#define FOLLOWCAMERA_H
#include "Camera.h"
#include "WaitCamera.h"
class FollowCamera : public Camera
{
public:

	virtual void update(D3DXVECTOR3 ballPosition,WaitCamera* WaitCamera, D3DXVECTOR3 moveDistance, bool* g_bDrop,D3DXVECTOR3 m_cameraDir, int g_iPower);
	virtual void setUp();
	virtual void setDevice(IDirect3DDevice9* pd3dDevice);
	void CleanUp();
	FollowCamera();
	~FollowCamera();
};
#endif

