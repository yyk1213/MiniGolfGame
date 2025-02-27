#include "ObjMesh.h"

ObjMesh::ObjMesh()
{
	position = D3DXVECTOR3(0, 0, 0);
	scale = D3DXVECTOR3(1, 1, 1);
}

ObjMesh::~ObjMesh()
{
}

bool ObjMesh::Init(LPDIRECT3DDEVICE9 device)
{
	g_pd3dDevice = device;
	return true;
}

bool ObjMesh::LoadTexture(LPCSTR textFileName)
{
	if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice, textFileName, &g_meshTextures)))
		return E_FAIL;
	
	return true;
}

HRESULT ObjMesh::Setup(LPCSTR fileName)
{
	// Now attempt to load the file.
	_objFileName = fileName;

	TObjMesh objMesh;
	if (0 > LoadObj(fileName, &objMesh))
	{
		OutputDebugString(TEXT("Failed to load the specified obj file!"));
		return E_FAIL;
	}

	CD3DMesh* pD3DMesh = new CD3DMesh;
	if (FAILED(pD3DMesh->Create(g_pd3dDevice, objMesh)))
	{
		delete pD3DMesh;
		return E_FAIL;
	}

	g_mesh = pD3DMesh;

	return S_OK;
}

bool ObjMesh::SetupMatrices()
{
	D3DXMATRIXA16 _matPosition;//평행이동
	D3DXMATRIXA16 _matScale;//크기

	D3DXMatrixTranslation(&_matPosition, position.x, position.y, position.z);
	D3DXMatrixScaling(&_matScale, scale.x, scale.y, scale.z);

	D3DXMatrixIdentity(&_matObj);
	D3DXMatrixMultiply(&_matObj, &_matScale, &_matPosition);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &_matObj);

	return true;
}
//다시
void ObjMesh::Draw()
{
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);

		SetUpTexture();
	g_pd3dDevice->SetStreamSource(0, g_mesh->pVB, 0, g_mesh->vertexSize);
	g_pd3dDevice->SetFVF(g_mesh->FVF);

	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, g_mesh->triCount);

	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, true);
}
bool ObjMesh::SetUpTexture()
{
	if (FAILED(g_pd3dDevice->SetTexture(0, g_meshTextures)))
		return false;
}

