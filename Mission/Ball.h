#pragma once
#ifndef BALL_H
#define BALL_H
#include "GolfGameApp.h"
#include "Camera.h"
#include "FollowCamera.h"
#include "Hole.h"
#include "Terrain.h"
#include "WaitCamera.h"
#include "Vertex.h"
#include "ReadyCamera.h"
#include "RelayCamera.h"

class Ball
{
protected:

	D3DMATERIAL9 Mtrls;

	D3DXVECTOR3 vPosition;
	D3DXVECTOR3 vVelocity;
	D3DXVECTOR3 tempVec;
	D3DXVECTOR3 target;		//바라보는지점

	Terrain* g_terrain;

	D3DXVECTOR3 m_newCameraPosition;
	IDirect3DTexture9* g_pTexture = 0;
	LPD3DXMESH pTempMesh = NULL;
	LPD3DXMESH pCloneMesh = NULL;
	LPDIRECT3DVERTEXBUFFER9 m_pVertexBuffer;
	LPDIRECT3DINDEXBUFFER9 m_pIndexBuffer;
	D3DXVECTOR3 m_cameraDir;

	D3DXMATRIX m_matScale;
	D3DXMATRIX m_matWorld;

	D3DXMATRIX m_matTranslation;
	IDirect3DDevice9* g_pd3dDevice;
	bool Ground = false;
	float ballHeight;
	float m_fRadius;
public:
	Ball();
	~Ball();

public:
	void CleanUp();		//종료 시 정리
	bool Setup(IDirect3DDevice9* pd3dDevice, Terrain* terrain);		//셋업 과정
	D3DXVECTOR3 update(float timeDelta, float timeElapsed, FollowCamera * cam,Hole * holeCup,WaitCamera* waitCamera, D3DXVECTOR3 preBallPosition,bool* g_bDrop, int g_iPower,RelayCamera* relayCamera);		//움직임 업데이트과정
	bool display(LPCSTR _texFileName);
	//미니맵 ball display
	bool mini_display(LPCSTR);

	D3DXVECTOR3 getBallPosition();
	void setTarget(D3DXVECTOR3 v) { target = v; }
	void setInitVec(D3DXVECTOR3,float);
	D3DMATRIX getMatTran() {
		return m_matTranslation;
	};
	void setBallPos(D3DXVECTOR3);
	float getHeight() { return ballHeight; }
	D3DXVECTOR3 GetScreenPos(D3DXVECTOR3 ballPosition);
	void DrawSphere(LPCSTR _texFileName);
	void ColorKeyState(LPCSTR*);
};

#endif