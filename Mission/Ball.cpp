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
	//���� Ŭ���� ����� ����ٵ� �߰�
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

D3DXVECTOR3 Ball::update(float timeDelta, float timeElapsed, FollowCamera * cam, Hole * holeCup,WaitCamera* WaitCamera,D3DXVECTOR3 preBallPosition, bool* g_bDrop, int g_iPower, RelayCamera* relayCamera)	//power�� �޾ƿͼ� ���� �ٲ��ְ� �׿����� �������ʿ�
{
	//�� ���ư��� ����
	float fTimeLeft = 2;	//��� 2�� ������Ű������
	float fElapsedFrameTime;	//�������� �ʹ������ð��� �귶���� �����ð����� ����
	ballHeight = g_terrain->getHeight(vPosition.x, vPosition.z);
	while (fTimeLeft > 0.0f)
	{
		fElapsedFrameTime = min(fTimeLeft, PHYSICS_FRAMELENGTH);

		for (int i = 0; i < 30000; i++)
		{
			//�� ���󰡴� ���� �ʹ� ���� �۵��Ǿ ���Žð��� ����. 
		}

		fTimeLeft -= fElapsedFrameTime;

		Ball::vPosition += Ball::vVelocity * fElapsedFrameTime;
	
		if (!Ball::Ground)	//���� ���� �ʾҴٸ�, �߷��� ����ִ� �۾�
		{
			Ball::vVelocity.y -= GRAVITY * fElapsedFrameTime;//v1 = v0-gt;			//���� ��� gravity�� �ʱ�ȭ�Ǿ���.
			if (Ball::vVelocity.y < 0 && Ball::vPosition.y>15.0f && m_cameraDir.y>3)
			{
				*g_bDrop = true;
			}
		}
		if (!Ball::Ground)		//(height+radius == vPosition.y�̷� ���� �����Ǹ� �浹..
		{
			if (Ball::vPosition.y < ballHeight+0.5)
			{
				Ball::vPosition.y = ballHeight+0.5 ;
				Ball::vVelocity.y = -Ball::vVelocity.y * (0.5);	//���� �۰�
				Ball::vVelocity.x *= 0.5f;		//����
				Ball::vVelocity.z *= 0.5f;
			}
		}
		else  //ó���� �����ö� y���� 0�̶�� ball grand�� true�༭ �������� �ۼ�		(�Ƹ� ī�޶� ��ġ�� ���� �޶�����)Q!!@!@!@!@!@!#!@#!@#
		{
			Ball::vVelocity.x *= 0.9f;
			Ball::vVelocity.z *= 0.9f;
		}

		holeCup->checkHole(vPosition);		//�������� �ٲ𶧸��� Ȧ�Ű��� �Ÿ��� ó���ؼ� �������Ḧ �˸�
		
		m_newCameraPosition = vPosition - preBallPosition; //���� �����θ�ŭ ī�޶� �Ȱ��� �����δ�.

		D3DXVECTOR3 holePosition = holeCup->getHolePosition();
		D3DXVECTOR3 ball_holeD = vPosition - holePosition;
		float holeDistance = D3DXVec3Length(&ball_holeD);

		if ((-100.0f <= holeDistance && holeDistance <0.f) || (0.f < holeDistance && holeDistance <= 100.0f))
		{
			relayCamera->SetUp();
			relayCamera->Update(vPosition);
		}

		else {

			cam->update(vPosition, WaitCamera, m_newCameraPosition, g_bDrop, m_cameraDir, g_iPower);		//waitī�޶��� ���������̿��ϰ� ������ �Ÿ���ŭ ����ؼ� �ȷο�ī�޶� ����

			D3DXMatrixTranslation(&m_matTranslation, Ball::vPosition.x, Ball::vPosition.y, Ball::vPosition.z);
			g_pd3dDevice->SetTransform(D3DTS_WORLD, &m_matTranslation);		}
		
		if (vPosition.z <zMinusBound)		//������ ����� ��� �浹ó��
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
	D3DXMatrixIdentity(&m_matScale);//�ʱ�ȭ
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixScaling(&m_matScale, 0.5f, 0.5f, 0.5f);		//��� ������ ������ ���� �̵� ������		//������ 0.5�� ����(ũ�� ��ȯ)
	D3DXMatrixTranslation(&m_matTranslation, Ball::vPosition.x, Ball::vPosition.y, Ball::vPosition.z);

	m_matWorld = m_matScale * m_matTranslation;		//��������� * ������� * �̵���� * ������� * �θ���ļ���
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
	DrawSphere(_texFileName);

	g_pTexture->Release();//�ؽ�ó��ӻ�������
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
	Ball::vVelocity = (vPosition - tempVec);	// ���󰡾� �ϴ� ���� -  ��ǥ
	
	if (Ball::vVelocity.y == 0.0f)		//������ ������ ���Ͷ�� 
	{
		Ball::Ground = true;
	}
	else
		Ball::Ground = false;
	D3DXVec3Normalize(&vVelocity, &vVelocity);		//��������
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
			pVertex->nx = 1.0f - ((float)nCurrentSegment / (float)nSegments);		//�����̸������������ؽ�Ʈ��ǥ��?
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


