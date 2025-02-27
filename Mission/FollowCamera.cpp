#include "Camera.h"
#include "FollowCamera.h"


FollowCamera::FollowCamera()				//FollowCamera는 공이 shot중일 때 사용하는 클래스
{
	m_cameraType = AIRCRAFT;

	m_vPos = D3DXVECTOR3(0.0f, 0.0f, -10.0f);
	m_vRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

void FollowCamera::update(D3DXVECTOR3 ballPosition, WaitCamera* WaitCamera,D3DXVECTOR3 moveDistance,bool* g_bDrop, D3DXVECTOR3 m_cameraDir,int g_iPower)//wait 카메라 포즈를 가져와서 거기서부터 추적..
{

	//공 정보 받아와서 계속해서 따라간다.
	if (m_cameraDir.y > 7.0 && g_iPower >7)
	{
		if (*g_bDrop == true)		//떨어지는 중에는 카메라이동이 밑과 같이 이루어짐...
		{
			m_vPos = D3DXVECTOR3(ballPosition.x + 10.0f, ballPosition.y + 10.0f, ballPosition.z + 10.0f);
		}
		else if (*g_bDrop == false)
		{
			m_vPos = WaitCamera->m_vPos;
		}
	}
	else
	{
		m_vPos = D3DXVECTOR3(ballPosition.x + 30.0f, ballPosition.y+30.0f, ballPosition.z );
	}

	D3DXMatrixLookAtLH(&V, &m_vPos, &ballPosition, &m_vUp);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &V);
	
}
void FollowCamera::setUp()	
{
	//투영
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI * 0.5, 1024 / float(768), 1.0f, 1000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}
void FollowCamera::CleanUp()
{
	g_pd3dDevice->Release();
}
void FollowCamera::setDevice(IDirect3DDevice9* pd3dDevice)
{
	g_pd3dDevice = pd3dDevice;
}




FollowCamera::~FollowCamera()
{
}
