#include "WaitCamera.h"

D3DXVECTOR3 temp;
void WaitCamera::update(D3DXVECTOR3 ballPosition, D3DXVECTOR3 g_moveWaitCamera,bool isNoKey)
{
	//ball뒤에서 그냥 바라보기 READY 상태 시에 1번만 호출할 수 있도록..
	//if(g_moveWaitCamera.y<5.0f)
	//m_vPos.y = ballPosition.y + 5.0f;					//ball의 위에서 바라보고
	if (isNoKey == true)		//key가 안눌렸을경우만 이동
	{
		m_vPos.x += g_moveWaitCamera.x; //이전 볼의 위치와 현재 볼의 위치를 고려하여 카메라를 움직여주기 때문에 볼의 방향을 이동시키는 카메라가 고정됨.
		m_vPos.z += g_moveWaitCamera.z;
	}
	m_vTarget = ballPosition;
	m_vTarget.y += 5.0f;
 	D3DXMatrixLookAtLH(&V, &m_vPos, &m_vTarget, &m_vUp);		//공이 제자리 오기 때문에 0,0,-100에서 바라보게한다

	g_pd3dDevice->SetTransform(D3DTS_VIEW, &V);
}

void WaitCamera::setUp(D3DXVECTOR3 ballPosition)
{
	m_vPos = D3DXVECTOR3(ballPosition.x + 10.0f, ballPosition.y + 5.0f, ballPosition.z);	//볼 정보에서 -100만큼 떨어진 곳을 가서 계속해서 추적.
	D3DXMatrixLookAtLH(&V, &m_vPos, &m_vTarget, &m_vUp);		//공이 제자리 오기 때문에 0,0,-100에서 바라보게한다
	
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &V);
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI * 0.5, 1024 / float(768), 1.0f, 2500.0f);	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void WaitCamera::setDevice(IDirect3DDevice9* pd3dDevice)
{
	g_pd3dDevice = pd3dDevice;
}

D3DXVECTOR3 WaitCamera::getPosition()
{
	return m_vPos;
}
void WaitCamera::strafe(float units) //왼쪽 오른쪽
{ 

	D3DXVECTOR3 vDist;
	vDist = m_vPos - m_vTarget;			//거리를 구함

	D3DXMATRIX matRotation;
	D3DXMatrixRotationY(&matRotation, units);
	D3DXVec3TransformCoord(&m_vLook, &m_vLook, &matRotation);
	D3DXVec3TransformCoord(&m_vRight, &m_vRight, &matRotation);
	D3DXVec3TransformCoord(&m_vPos, &m_vPos, &matRotation);

	D3DXVECTOR3 newPos;						
	D3DXVec3TransformCoord(&newPos, &vDist, &matRotation);		//오브젝트 기준으로한 카메라 위치회전
	newPos += m_vTarget;			//카메라 월드위치이동
	setPosition(newPos);			//새로운 좌표지정.

	D3DXMatrixLookAtLH(&V, &m_vPos, &m_vTarget, &D3DXVECTOR3(0.0f,0.1f,0.0f));
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &V);

}
void WaitCamera::fly(float units) //위 아래
{
	D3DXVECTOR3 vDist;
	vDist =  m_vPos- m_vTarget;			//거리를 구함


	D3DXVECTOR3 vView;			//이것의 수직인 벡터를 구해야하
	D3DXVec3Normalize(&vView, &(m_vTarget - m_vPos));
	D3DXVec3Cross(&m_vCross, &m_vUp, &vView);
	D3DXMATRIX matRotation;
	D3DXMatrixRotationAxis(&matRotation, &m_vCross, units);//cross대칭(카메라 x축)

	D3DXVECTOR3 newPos;
	D3DXVec3TransformCoord(&newPos, &vDist, &matRotation);		//오브젝트 기준으로한 카메라 위치회전
	newPos += m_vTarget;			//카메라 월드위치이동
	setPosition(newPos);

	D3DXMatrixLookAtLH(&V, &m_vPos, &m_vTarget, &D3DXVECTOR3(0.0f, 0.1f, 0.0f));
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &V);
}


void WaitCamera::keyState(float timeDelta, D3DXVECTOR3 tempBallPos)
{

	if (GetAsyncKeyState(VK_LEFT) & 0x8000f)	//x축
		WaitCamera::strafe(1.0f * timeDelta);
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		WaitCamera::strafe(-1.0f * timeDelta);
	if (GetAsyncKeyState(VK_DOWN) & 0x8000f)		//y축 
		if(m_vPos.y>3.0f)
			WaitCamera::fly(-1.0f * timeDelta);
	if (GetAsyncKeyState(VK_UP) & 0x8000f )
	{
			WaitCamera::fly(1.0f * timeDelta);
	}
}

void WaitCamera::SaveMouseInitPos(HWND HWnd, LPARAM lParam)
{
	m_xInitPos = GET_X_LPARAM(lParam);
	m_yInitPos = GET_Y_LPARAM(lParam);

	SetCapture(HWnd);
	isLBClicked = true;
}

void WaitCamera::CalcMouseAngle(HWND hWnd, LPARAM lParam)
{
	if (isLBClicked)
	{
		m_xCrntPos = GET_X_LPARAM(lParam);
		m_yCrntPos = GET_Y_LPARAM(lParam);

		m_xAngle = -(m_xInitPos - m_xCrntPos) / 200;
		m_yAngle = -(m_yInitPos - m_yCrntPos) / 200;

		m_xInitPos = m_xCrntPos;
		m_yInitPos = m_yCrntPos;

		if (abs(m_xAngle) > abs(m_yAngle))
		{
			
			WaitCamera::strafe(m_xAngle);
		}
		else
		{
			if (m_yAngle > 0)
			{
				WaitCamera::fly(m_yAngle);

			}
			else if (m_yAngle < 0)
			{
				if (m_vPos.y > 3.0f)
				{
					WaitCamera::fly(m_yAngle);
				}
			}
		}
		
	}
}
void WaitCamera::CleanUp()
{
	g_pd3dDevice->Release();
}
WaitCamera::WaitCamera()
{
	m_vPos = D3DXVECTOR3(0.0f, 50.0f, -70.0f);
	m_vRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	g_fTempY = 9999.0f;
}


WaitCamera::~WaitCamera()
{
}
