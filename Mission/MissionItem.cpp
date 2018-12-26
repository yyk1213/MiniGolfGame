#include "MissionItem.h"

bool MissionItem::checkItem(D3DXVECTOR3 ballPos)
{
	ballPos = ballPos - m_ItemPos;
	float distance = D3DXVec3Length(&ballPos);
	if (distance <= 7.0f)
	{
		m_ItemMesh->Release();
		m_bIsInItem = true;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
void MissionItem::setUp(IDirect3DDevice9* pd3dDevice)
{
	g_pd3dDevice = pd3dDevice;
	D3DXCreateCylinder(pd3dDevice, 2.0f, 2.0f, 1.0f, 20, 5, &m_ItemMesh, 0);
	m_ItemPos.x = -610.0f; m_ItemPos.y = 10.0f;m_ItemPos.z = -750.0f;
}
void MissionItem::display( )
{
	D3DXCreateTextureFromFile(g_pd3dDevice, "ballTex/lowball_deepyellow.dds", &g_pTexture);
	g_pd3dDevice->SetTexture(0, g_pTexture);
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);


	D3DXMatrixIdentity(&_matObj);
	D3DXMatrixRotationX(&matRotate, (D3DX_PI));
	D3DXMatrixRotationY(&matRotate, GetTickCount()*0.004f);
	D3DXMatrixTranslation(&matTrans, m_ItemPos.x, m_ItemPos.y, m_ItemPos.z);

	D3DXMatrixMultiply(&_matObj, &matRotate, &matTrans);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &_matObj);
	if(m_bIsInItem==false)
	m_ItemMesh->DrawSubset(0);

	g_pTexture->Release();
	return;
}
void MissionItem::mini_display()
{
	D3DXCreateTextureFromFile(g_pd3dDevice, "ballTex/lowball_deepyellow.dds", &g_pTexture);
	g_pd3dDevice->SetTexture(0, g_pTexture);
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	D3DXMATRIXA16 _matScale;
	D3DXMATRIXA16 _matTrans;

	D3DXMatrixScaling(&_matScale, 3, 3, 3);

	D3DXMatrixTranslation(&_matTrans, m_ItemPos.x, m_ItemPos.y, m_ItemPos.z);
	D3DXMatrixMultiply(&_matObj, &_matScale, &_matTrans);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &_matObj);

	m_ItemMesh->DrawSubset(0);
	g_pTexture->Release();

}
void MissionItem::CleanUp()
{
	if(m_ItemMesh)
	m_ItemMesh->Release();
}

MissionItem::MissionItem()
{
}


MissionItem::~MissionItem()
{
}
