#pragma once

#ifndef CAMERA_H
#define CAMERA_H
#include "GolfGameApp.h"
class Camera
{
public:
	Camera();
	~Camera();

public:
	enum CameraType { LANDOBJECT, AIRCRAFT };

	virtual D3DXVECTOR3 getPosition();
	D3DXVECTOR3 getLook();

	virtual void setDevice(IDirect3DDevice9* pd3dDevice) = 0;
	void setUp() ;


	D3DXMATRIX* getViewMatrixPointer() { return &V; }

	friend class FollowCamera;		//변수 직접 접근을 가능케함
	friend class ReadyCamera;
	friend class RelayCamera;

protected:
	CameraType  m_cameraType;
	D3DXVECTOR3 m_vRight;
	D3DXVECTOR3 m_vUp;
	D3DXVECTOR3 m_vLook;
	D3DXVECTOR3 m_vPos;  
	D3DXVECTOR3 m_vLookVec;

	D3DXVECTOR3  m_rot;

	// 스케일 * 로데이션 * 이동좌표 = 

	IDirect3DDevice9* g_pd3dDevice;		//get으로 세팅하여 멤버로 사용?
	D3DXMATRIX V;
	D3DXVECTOR3 _target;
	D3DXMATRIXA16   m_matProj;		//투영행렬세팅..
	D3DXVECTOR3 _offset;
	POINT m_pmousePoint;
	D3DXMATRIX T;
};

#endif