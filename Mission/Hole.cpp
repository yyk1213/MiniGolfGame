#include "Hole.h"

void Hole::setUp(IDirect3DDevice9* pd3dDevice)
{
	g_pd3dDevice = pd3dDevice;
	D3DXCreateCylinder(pd3dDevice, 2.0f, 2.0f, 0.5f, 10, 5, &m_HoleMesh, 0);
	m_holePos.x = 200.0f; m_holePos.y = 0.0f;m_holePos.z = 270.0f;

}
void Hole::display()
{
	D3DXCreateTextureFromFile(g_pd3dDevice, "ballTex/lowball.dds", &g_pTexture);
	g_pd3dDevice->SetTexture(0, g_pTexture);
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	D3DXMATRIXA16 matTrans;
	D3DXMATRIXA16 matRotate;

	D3DXMatrixIdentity(&_matObj);

	D3DXMatrixRotationX(&matRotate, (D3DX_PI / 2));
	D3DXMatrixTranslation(&matTrans, m_holePos.x, m_holePos.y, m_holePos.z);

	D3DXMatrixMultiply(&_matObj, &matRotate, &matTrans);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &_matObj);

	m_HoleMesh->DrawSubset(0);

	g_pTexture->Release();
	return;
}
void Hole::mini_display()
{
	D3DXMATRIXA16 matScale;
	D3DXMATRIXA16 matTrans;
	D3DXMATRIXA16 matRotate;

	D3DXMatrixIdentity(&_matObj);

	D3DXMatrixScaling(&matScale, 3,3, 3);

	D3DXMatrixRotationX(&matRotate, (D3DX_PI / 2));
	D3DXMatrixTranslation(&matTrans, m_holePos.x, m_holePos.y, m_holePos.z);

	D3DXMatrixMultiply(&_matObj, &matScale, &matRotate);
	D3DXMatrixMultiply(&_matObj, &_matObj, &matTrans);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &_matObj);
	m_HoleMesh->DrawSubset(0);


	return;
}
bool Hole::checkHole(D3DXVECTOR3 ballPos)
{
	ballPos = ballPos - m_holePos;
	float distance = D3DXVec3Length(&ballPos);
	if (distance <= 2.0f)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
void Hole::CleanUp()
{
	m_HoleMesh->Release();
	g_pd3dDevice->Release();
}
Hole::Hole()
{
}


Hole::~Hole()
{
}
