#include "MiniMap.h"

MiniMap::MiniMap()
{
	m_cameraType = AIRCRAFT;

	m_vPos = D3DXVECTOR3(0.0f, 10.0f, -474.0f);
	m_vRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

MiniMap::~MiniMap()
{
}

void MiniMap::setDevice(IDirect3DDevice9* pd3dDevice)
{
	g_pd3dDevice = pd3dDevice;
}

bool MiniMap::createTexture()
{
	if (FAILED(D3DXCreateTexture(g_pd3dDevice, 180, 180, D3DX_DEFAULT, D3DUSAGE_RENDERTARGET | D3DUSAGE_AUTOGENMIPMAP, D3DFMT_X8R8G8B8,D3DPOOL_DEFAULT, &texture)))
	{
		return false;
	}
}

void MiniMap::setUp()
{
}

bool MiniMap::SetupMatrices()
{
	//// Keep camera's axes orthogonal to eachother
	//D3DXVec3Normalize(&m_vLook, &m_vLook);

	//D3DXVec3Cross(&m_vUp, &m_vLook, &m_vRight);
	//D3DXVec3Normalize(&m_vUp, &m_vUp);

	//D3DXVec3Cross(&m_vRight, &m_vUp, &m_vLook);
	//D3DXVec3Normalize(&m_vRight, &m_vRight);

	//// Build the view matrix:
	//float x = -D3DXVec3Dot(&m_vRight, &m_vPos);
	//float y = -D3DXVec3Dot(&m_vUp, &m_vPos);
	//float z = -D3DXVec3Dot(&m_vLook, &m_vPos);

	//V(0, 0) = m_vRight.x; V(0, 1) = m_vUp.x; V(0, 2) = m_vLook.x; V(0, 3) = 0.0f;
	//V(1, 0) = m_vRight.y; V(1, 1) = m_vUp.y; V(1, 2) = m_vLook.y; V(1, 3) = 0.0f;
	//V(2, 0) = m_vRight.z; V(2, 1) = m_vUp.z; V(2, 2) = m_vLook.z; V(2, 3) = 0.0f;
	//V(3, 0) = x;        V(3, 1) = y;     V(3, 2) = z;       V(3, 3) = 1.0f;

	//D3DXMATRIX * _matView = getViewMatrixPointer();
	//g_pd3dDevice->SetTransform(D3DTS_VIEW, _matView);

	////투영 행렬 계산 및 디바이스에 적용
	//D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4, 1.0f, 1.0f, 1000.0f);
	//g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);

	D3DXMATRIX   matProj;
	D3DXMATRIX	 matCameraView;
	D3DXVECTOR3  cameraTarget;

	//// Set where camera is viewing from
	//cameraPos.x = 0.0f;
	//cameraPos.y = 0.0f;
	//cameraPos.z = 10.0f;
	// Set what camera is looking at
	cameraTarget.x = 0.0f;
	cameraTarget.y = 0.0f;
	cameraTarget.z = 0.0f;

	// Setup Camera View
	D3DXMatrixLookAtLH(&matCameraView,
		&m_vPos,
		&cameraTarget,
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	// Tell the device to use the camera view for the viewport
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matCameraView);

	// Setup Projection 
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 25000.0f);
	// Tell the device to use the above matrix for projection
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);

	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, D3DXCOLOR(1.0, 1.0, 1.0, 1.0));

	return S_OK;

}
