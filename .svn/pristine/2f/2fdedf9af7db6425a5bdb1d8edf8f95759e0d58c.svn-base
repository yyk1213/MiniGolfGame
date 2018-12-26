#include "Camera.h"
#include "RelayCamera.h"

RelayCamera::RelayCamera()
{
	m_cameraType = AIRCRAFT;

	m_vPos = D3DXVECTOR3(285.0f, 65.0f, 206.0f);
	m_vRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_vUp = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	m_vLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

void RelayCamera::setDevice(IDirect3DDevice9* pd3dDevice)
{
	g_pd3dDevice = pd3dDevice;
}

void RelayCamera::CleanUp()
{
	g_pd3dDevice->Release();
}

void RelayCamera::Update(D3DXVECTOR3 ballPosition)
{
	D3DXVECTOR3 hole = D3DXVECTOR3(200.0f, 0.0f, 270.0f);
	D3DXVECTOR3 n = D3DXVECTOR3((ballPosition.x + hole.x) / 2, 0, (ballPosition.z + hole.z) / 2);

	D3DXMatrixLookAtLH(&V, &m_vPos, &hole, &m_vUp);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &V);
}
//홀 컵에 공이 일정거리 이상 가까이 가면 카메라 위치를 고정시키고 공을 관찰한다.
void RelayCamera::SetUp()
{
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI*0.5, 1024 / float(768), 1.0f, 2500.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}