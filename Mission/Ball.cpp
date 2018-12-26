#include "Ball.h"

#define PHYSICS_FRAMELENGTH 0.03f
#define GRAVITY 0.003f
#define AMMO_SIZE 0.25f
#define zPlusBound 789.5
#define zMinusBound -789.5
#define xPlusbound 789.5
#define xMinusBound -789.5

void Ball::CleanUp()
{
	g_pTexture->Release();
	//지형 클린업 생기면 여기다도 추가
	g_pd3dDevice->Release();
}
bool Ball::Setup(IDirect3DDevice9* pd3dDevice, Terrain* terrain)
{
	g_pd3dDevice = pd3dDevice;
	g_terrain = terrain;
	ballHeight = g_terrain->getHeight(0.0f, 0.0f);
	Ball::vPosition = D3DXVECTOR3(100.0f, ballHeight + 0.5f, -600.0f);

	Ball::Ground = false;

	return true;
}

D3DXVECTOR3 Ball::update(float timeDelta, float timeElapsed, FollowCamera * cam, Hole * holeCup,WaitCamera* WaitCamera,D3DXVECTOR3 preBallPosition, bool* g_bDrop, int g_iPower, RelayCamera* relayCamera)	//power를 받아와서 힘을 바꿔주고 그에대한 수식이필요
{
	//공 날아가는 과정
	float fTimeLeft = 2;	//잠시 2로 고정시키기위함
	float fElapsedFrameTime;	//프레임이 너무많은시간이 흘렀으면 고정시간으로 변경
	ballHeight = g_terrain->getHeight(vPosition.x, vPosition.z);
	while (fTimeLeft > 0.0f)
	{
		fElapsedFrameTime = min(fTimeLeft, PHYSICS_FRAMELENGTH);

		for (int i = 0; i < 30000; i++)
		{
			//볼 날라가는 것이 너무 빨리 작동되어서 갱신시간을 늦춤. 
		}

		fTimeLeft -= fElapsedFrameTime;

		Ball::vPosition += Ball::vVelocity * fElapsedFrameTime;
	
		if (!Ball::Ground)	//땅에 닿지 않았다면, 중력을 계속주는 작업
		{
			Ball::vVelocity.y -= GRAVITY * fElapsedFrameTime;//v1 = v0-gt;			//땅을 닿고서 gravity가 초기화되야함.
			if (Ball::vVelocity.y < 0 && Ball::vPosition.y>15.0f && m_cameraDir.y>3)
			{
				*g_bDrop = true;
			}
		}
		if (!Ball::Ground)		//(height+radius == vPosition.y이런 식이 만족되면 충돌..
		{
			if (Ball::vPosition.y < ballHeight+0.5)
			{
				Ball::vPosition.y = ballHeight+0.5 ;
				Ball::vVelocity.y = -Ball::vVelocity.y * (0.5);	//힘을 작게
				Ball::vVelocity.x *= 0.5f;		//마찰
				Ball::vVelocity.z *= 0.5f;
			}
		}
		else  //처음에 볼들어올때 y축이 0이라면 ball grand에 true줘서 굴러가게 작성		(아마 카메라 위치에 따라 달라질듯)Q!!@!@!@!@!@!#!@#!@#
		{
			Ball::vVelocity.x *= 0.9f;
			Ball::vVelocity.z *= 0.9f;
		}

		holeCup->checkHole(vPosition);		//움직임이 바뀔때마다 홀컵과의 거리를 처리해서 게임종료를 알림
		
		m_newCameraPosition = vPosition - preBallPosition; //볼이 움직인만큼 카메라가 똑같이 움직인다.

		D3DXVECTOR3 holePosition = holeCup->getHolePosition();
		D3DXVECTOR3 ball_holeD = vPosition - holePosition;
		float holeDistance = D3DXVec3Length(&ball_holeD);

		if ((-100.0f <= holeDistance && holeDistance <0.f) || (0.f < holeDistance && holeDistance <= 100.0f))
		{
			relayCamera->SetUp();
			relayCamera->Update(vPosition);
		}

		else {

			cam->update(vPosition, WaitCamera, m_newCameraPosition, g_bDrop, m_cameraDir, g_iPower);		//wait카메라의 포지션을이용하고 움직인 거리만큼 계속해서 팔로우카메라 수정

			D3DXMatrixTranslation(&m_matTranslation, Ball::vPosition.x, Ball::vPosition.y, Ball::vPosition.z);
			g_pd3dDevice->SetTransform(D3DTS_WORLD, &m_matTranslation);		}
		
		if (vPosition.z <zMinusBound)		//지형을 벗어나는 경우 충돌처리
		{
			vPosition.z = zMinusBound;
			vVelocity.z = -vVelocity.z*0.5;
		}
		if (vPosition.z > zPlusBound)
		{
			vPosition.z = zPlusBound;
			vVelocity.z = -vVelocity.z * 0.5;
		}

		if (vPosition.x <xMinusBound)
		{
			vPosition.x = xMinusBound;
			vVelocity.x = -vVelocity.x *0.5;
		}
		if (vPosition.x > zPlusBound)
		{
			vPosition.x = zPlusBound;
			vVelocity.x = -vVelocity.x * 0.5;
		}
	}

	return m_newCameraPosition;
}


bool ::Ball::display(LPCSTR _texFileName)
{
	D3DXMatrixIdentity(&m_matScale);//초기화
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixScaling(&m_matScale, 0.5f, 0.5f, 0.5f);		//행렬 조합은 스케일 자전 이동 순으로		//반지름 0.5로 세팅(크기 변환)
	D3DXMatrixTranslation(&m_matTranslation, Ball::vPosition.x, Ball::vPosition.y, Ball::vPosition.z);

	m_matWorld = m_matScale * m_matTranslation;		//스케일행렬 * 자전행렬 * 이동행렬 * 공전행렬 * 부모행렬순서
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	DrawSphere(_texFileName);

	g_pTexture->Release();//텍스처계속생성방지
	return true;
}

bool Ball::mini_display(LPCSTR _texFileName)
{
	D3DXMATRIXA16 _matScale;
	D3DXMATRIXA16 _matTrans;

	D3DXMatrixScaling(&_matScale, 12, 12, 12);

	D3DXMatrixTranslation(&_matTrans, Ball::vPosition.x, Ball::vPosition.y, Ball::vPosition.z);

	D3DXMatrixMultiply(&m_matTranslation, &_matScale, &_matTrans);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &m_matTranslation);
	DrawSphere(_texFileName);
	g_pTexture -> Release();
	return true;
}


D3DXVECTOR3 Ball::getBallPosition()
{
	return Ball::vPosition;
}

void Ball::setInitVec(D3DXVECTOR3 tempVec,float power) 
{
	m_cameraDir = tempVec;
	float mediate = power * 0.02;
	Ball::vVelocity = (vPosition - tempVec);	// 날라가야 하는 지점 -  좌표
	
	if (Ball::vVelocity.y == 0.0f)		//지형에 수평인 벡터라면 
	{
		Ball::Ground = true;
	}
	else
		Ball::Ground = false;
	D3DXVec3Normalize(&vVelocity, &vVelocity);		//단위벡터
	vVelocity *= (power * mediate);
	
}

void Ball::DrawSphere(LPCSTR _texFileName)
{
	DWORD D3DFVF_CUSTOMVERTEX = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE);
	struct CUSTOMVERTEX
	{
		FLOAT x, y, z;                //  position
		FLOAT nx, ny, nz;       //Direction the vertex is facing(normal)
		FLOAT tu, tv;
		DWORD colour;                 //The vertex colour.
	};
	int nRings = 30;
	int nSegments =30;

	DWORD dwNumOfVertices = (nRings + 1) * (nSegments + 1);	
	DWORD dwNumOfIndices = 2 * nRings * (nSegments + 1);

	LPDIRECT3DVERTEXBUFFER9 pVertexBuffer = NULL;
	IDirect3DIndexBuffer9* pIndexBuffer = NULL;

	g_pd3dDevice->CreateVertexBuffer(dwNumOfVertices * sizeof(CUSTOMVERTEX),0, D3DFVF_CUSTOMVERTEX,	D3DPOOL_DEFAULT, &pVertexBuffer,0);
	g_pd3dDevice->CreateIndexBuffer(dwNumOfIndices * sizeof(WORD),0, D3DFMT_INDEX16, D3DPOOL_MANAGED,&pIndexBuffer,NULL);

	WORD *pIndices;
	CUSTOMVERTEX *pVertex;

	pVertexBuffer->Lock(0, 0, (void**)&pVertex, 0);
	pIndexBuffer->Lock(0, dwNumOfIndices, (void**)&pIndices, 0);

	WORD wVertexIndex = 0;
	D3DXVECTOR3 vNormal;

	//Setup some angles
	float rDeltaRingAngle = (D3DX_PI / nRings);
	float rDeltaSegAngle = (2.0f * D3DX_PI / nSegments);

	for (int nCurrentRing = 0; nCurrentRing < nRings + 1; nCurrentRing++)
	{
		float r0 = sinf(nCurrentRing * rDeltaRingAngle);
		float y0 = cosf(nCurrentRing * rDeltaRingAngle);
		for (int nCurrentSegment = 0; nCurrentSegment < nSegments + 1; nCurrentSegment++)
		{
			float x0 = r0 * sinf(nCurrentSegment * rDeltaSegAngle);
			float z0 = r0 * cosf(nCurrentSegment * rDeltaSegAngle);

			vNormal.x = x0;
			vNormal.y = y0;
			vNormal.z = z0;
			D3DXVec3Normalize(&vNormal, &vNormal);

			pVertex->x = x0;
			pVertex->y = y0;
			pVertex->z = z0;
			pVertex->nx = 1.0f - ((float)nCurrentSegment / (float)nSegments);		//정점이만나는지점을텍스트좌표로?
			pVertex->ny = (float)nCurrentRing / (float)nRings;

				
			pVertex++;

			if (nCurrentRing != nRings)
			{
				*pIndices = wVertexIndex;
				pIndices++;
				*pIndices = wVertexIndex + (WORD)(nSegments + 1);
				pIndices++;
				wVertexIndex++;
			}
		}
	}
	pIndexBuffer->Unlock();
	pVertexBuffer->Unlock();
	g_pd3dDevice->SetStreamSource(0, pVertexBuffer,0 ,sizeof(CUSTOMVERTEX));
	g_pd3dDevice->SetIndices(pIndexBuffer);

	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	D3DXCreateTextureFromFile(g_pd3dDevice, _texFileName, &g_pTexture);
	g_pd3dDevice->SetTexture(0, g_pTexture);
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);


	DWORD dwNumOfPolygons = dwNumOfIndices - 2;
	g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0,dwNumOfVertices ,0, dwNumOfPolygons);

	pIndexBuffer->Release();
	pVertexBuffer->Release();

}

void Ball::ColorKeyState(LPCSTR* g_texFileName )
{
	if (GetAsyncKeyState('5')& 0x0001f)		
	{
		*g_texFileName = "ballTex/lowball_sky.dds";
	}
	else if (GetAsyncKeyState('6') & 0x0001f)	
	{
		*g_texFileName = "ballTex/lowball_lemon.dds";
	}
	else if (GetAsyncKeyState('7') & 0x0001f)		
	{
		*g_texFileName = "ballTex/lowball_violet.dds";
	}
	else if (GetAsyncKeyState('8') & 0x0001f)		
	{
		*g_texFileName = "ballTex/lowball_orange.dds";
	}
	else if (GetAsyncKeyState('9') & 0x0001f)		
	{
		*g_texFileName = "ballTex/lowball_purple.dds";
	}
	else if (GetAsyncKeyState('0') & 0X0001f)
	{
		*g_texFileName = "ballTex/lowball.dds";
	}
}


void Ball::setBallPos(D3DXVECTOR3 moveVec)
{
	vPosition = moveVec;
}
D3DXVECTOR3 Ball::GetScreenPos(D3DXVECTOR3 ballPosition)
{
	return D3DXVECTOR3();
}
 
Ball::Ball()
{
}


Ball::~Ball()
{
} 


