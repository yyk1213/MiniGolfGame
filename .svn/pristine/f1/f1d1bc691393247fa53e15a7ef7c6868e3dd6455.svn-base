#include "Sky.h"

void Sky::CleanUp()
{
	sky_Buf->Release();
	
}

bool Sky::Init(LPDIRECT3DDEVICE9 device)
{
	g_pd3dDevice = device;

	sky_texList[0] = "Skybox/Top.bmp";
	sky_texList[1] = "Skybox/Front.bmp";
	sky_texList[2] = "Skybox/Back.bmp";
	sky_texList[3] = "Skybox/Right.bmp";
	sky_texList[4] = "Skybox/Left.bmp";
	sky_texList[5] = "Skybox/Bottom.bmp";

	if (FAILED(g_pd3dDevice->CreateVertexBuffer(sizeof(Sky_Vertex) * 24, 0, D3DFVF_XYZ | D3DFVF_TEX1, D3DPOOL_DEFAULT,
		&sky_Buf, NULL)))
		return false;
	
	Sky_Vertex _vertices[24] =
	{
		{ -1000.0f, 1000.0f, -1000.0f, 0.0f, 0.0f },		// Top vertices
		{ 1000.0f, 1000.0f, -1000.0f, 1.0f, 0.0f },
		{ -1000.0f, 1000.0f,  1000.0f, 0.0f, 1.0f },
		{ 1000.0f, 1000.0f,  1000.0f, 1.0f, 1.0f },

		{ -1000.0f,  1000.0f, 1000.0f, 0.0f, 0.0f },		// Front vertices
		{ 1000.0f,  1000.0f, 1000.0f, 1.0f, 0.0f },
		{ -1000.0f, -1000.0f, 1000.0f, 0.0f, 1.0f },
		{ 1000.0f, -1000.0f, 1000.0f, 1.0f, 1.0f },

		{ 1000.0f,  1000.0f, -1000.0f, 0.0f, 0.0f },		// Back vertices
		{ -1000.0f,  1000.0f, -1000.0f, 1.0f, 0.0f },
		{ 1000.0f, -1000.0f, -1000.0f, 0.0f, 1.0f },
		{ -1000.0f, -1000.0f, -1000.0f, 1.0f, 1.0f },

		{ -1000.0f,  1000.0f, -1000.0f, 0.0f, 0.0f },		// Left vertices
		{ -1000.0f,  1000.0f,  1000.0f, 1.0f, 0.0f },
		{ -1000.0f, -1000.0f, -1000.0f, 0.0f, 1.0f },
		{ -1000.0f, -1000.0f,  1000.0f, 1.0f, 1.0f },

		{ 1000.0f,  1000.0f,  1000.0f, 0.0f, 0.0f },		// Right vertices
		{ 1000.0f,  1000.0f, -1000.0f, 1.0f, 0.0f },
		{ 1000.0f, -1000.0f,  1000.0f, 0.0f, 1.0f },
		{ 1000.0f, -1000.0f, -1000.0f, 1.0f, 1.0f },

		{ -1000.0f, -1000.0f,  1000.0f, 0.0f, 0.0f },		// Bottom vertices
		{ 1000.0f, -1000.0f,  1000.0f, 1.0f, 0.0f },
		{ -1000.0f, -1000.0f, -1000.0f, 0.0f, 1.0f },

		{ 1000.0f, -1000.0f, -1000.0f, 1.0f, 1.0f }
	};

	VOID* _pVertices = NULL;

	sky_Buf->Lock(0, sizeof(Sky_Vertex) * 24, (void**)&_pVertices, 0);

	memcpy(_pVertices, _vertices, sizeof(Sky_Vertex) * 24);

	sky_Buf->Unlock();

	if (!LoadTexture(sky_texList))
		return false;

	return true;
}

bool Sky::LoadTexture(LPCSTR sky_texList[TEX_NUM])
{
	for (int i = 0; i < 6; i++)
	{
	 if(FAILED(D3DXCreateTextureFromFile(g_pd3dDevice,sky_texList[i],&sky_Tex[i])))
			return false;
	}
	return true;
}

void Sky::Render()
{
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);//텍스쳐 한번만 입히고 엣지부분 늘려서 칠하기
	
	g_pd3dDevice->SetStreamSource(0, sky_Buf, 0, sizeof(Sky_Vertex));

	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	for (int i = 0; i < 6; i++)
	{
		g_pd3dDevice->SetTexture(0, sky_Tex[i]);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, i * 4, 2);
	}

	g_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);//텍스쳐 바둑판 모양 반복
	
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, true);
}

bool Sky::SetUpMatrices()
{
	//D3DXMATRIXA16 skyMat;
	D3DXMatrixIdentity(&skyMat);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &skyMat);

	return true;
}
