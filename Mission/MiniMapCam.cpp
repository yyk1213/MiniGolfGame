#include "MiniMapCam.h"

MiniMapCam::MiniMapCam()
{
}

MiniMapCam::~MiniMapCam()
{
}

void MiniMapCam::setDevice(IDirect3DDevice9* pd3dDevice)
{
	g_pd3dDevice = pd3dDevice;

	m_vPos = D3DXVECTOR3(1.0f, 650.0f, 0.0f);
	m_vRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);//물체가 오른쪽 방향을 가리키도록 정의
	m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);//
	m_vLook = D3DXVECTOR3(0.0f, -1.0f, 0.0f);//물체가 가리키는 방향
}

bool MiniMapCam::SetupMatrices()
{
	
	D3DXVECTOR3 target = D3DXVECTOR3(0, 0, 0);
	D3DXMatrixLookAtLH(&mini_mat, &m_vPos, &target, &m_vUp);

	g_pd3dDevice->SetTransform(D3DTS_VIEW, &mini_mat);

	return S_OK;

}

void MiniMapCam::CleanUp()
{
	g_pd3dDevice->Release();
}
