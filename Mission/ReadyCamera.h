#pragma once
#ifndef READYCAMERA_H
#define READYCAMERA_H
#include "Camera.h"
class ReadyCamera :public Camera
{

public:
	virtual D3DXMATRIXA16* strafe(float units); //왼쪽 오른쪽
	virtual D3DXMATRIXA16* fly(float units); //위 아래
	virtual D3DXMATRIXA16* walk(float units);//앞 뒤
	void CleanUp();
	virtual D3DXMATRIXA16* pitch(float angle);
	virtual D3DXMATRIXA16* yaw(float angle, IDirect3DDevice9* device);
	virtual void setDevice(IDirect3DDevice9* pd3dDevice);
	virtual void setUp(D3DXVECTOR3 ballPosition);
	virtual void update(D3DXVECTOR3 ballPosition);
	void getViewMatrix(D3DXMATRIX* V);
	void keyState(float timeDelta, gameType* g_eType);
	D3DXMATRIXA16* SetView(D3DXVECTOR3* pvEye, D3DXVECTOR3 *pvLookat, D3DXVECTOR3 *pvUp);
	D3DXMATRIXA16* MoveTo(D3DXVECTOR3* pv);
	ReadyCamera();
	~ReadyCamera();
protected:
	D3DXVECTOR3 m_vView;
	D3DXVECTOR3 m_vCross;

	D3DXMATRIXA16 m_matView;
	D3DXMATRIXA16 m_matBill;

};

#endif