#pragma once
#include "Camera.h"
#include <windowsx.h>
#include <iostream>

class WaitCamera :
	public Camera
{
public:
	void strafe(float units); //왼쪽 오른쪽
	void fly(float units); //위 아래
	virtual void setDevice(IDirect3DDevice9* pd3dDevice);
	virtual void update(D3DXVECTOR3 ballPosition, D3DXVECTOR3 g_moveWaitCamera, bool isNoKey);
	void setUp(D3DXVECTOR3 ballPosition);
	void keyState(float timeDelta, D3DXVECTOR3 tempBallPos);
	D3DXVECTOR3 getPosition();
	void setPosition(D3DXVECTOR3 aa) { m_vPos = aa; }


	// 마우스
	bool isLBClicked;//왼쪽 버튼 클릭
	float m_xInitPos; float m_yInitPos;
	float m_xCrntPos; float m_yCrntPos;
	float m_xAngle; float m_yAngle;

	//마우스 좌표받기
	void SaveMouseInitPos(HWND HWnd, LPARAM lParam);
	void CalcMouseAngle(HWND hWnd, LPARAM lParam);

	void CleanUp();
	WaitCamera();
	~WaitCamera();
protected:
	D3DXVECTOR3 m_vTarget;	//ball 포지션 받아와서 그 타겟에서 움직일수있게만들자.
	D3DXVECTOR3 m_vCross;

	D3DXVECTOR3 g_ballPosition;
	float g_fTempY;
	D3DXMATRIX m_ballMatrix;

};