#include "BeginScreen.h"


void BeginScreen::Setup(IDirect3DDevice9* pd3dDevice)
{
	g_pd3dDevice = pd3dDevice;

	g_pd3dDevice->CreateVertexBuffer(
		6 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		FVF1,
		D3DPOOL_MANAGED,
		&m_vb,
		0);

	Vertex* v;
	m_vb->Lock(0, 0, (void**)&v, 0);

	// quad built from two triangles, note texture coordinates:
	v[0] = Vertex(-1.75f, -1.75f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[1] = Vertex(-1.75f, 1.75f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	v[2] = Vertex(1.75f, 1.75f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

	v[3] = Vertex(-1.75f, -1.75f, 1.25f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
	v[4] = Vertex(1.75f, 1.75f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
	v[5] = Vertex(1.75f, -1.75f, 1.25f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

	m_vb->Unlock();

	//
	// Create the texture and set filters.
	//

	D3DXCreateTextureFromFile(
		g_pd3dDevice,
		"key.png",
		&m_tex);

	g_pd3dDevice->SetTexture(0, m_tex);

	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);


	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, false);

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, // 90 - degree
		(float)Width / (float)Height,
		1.0f,
		1000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &proj);
;
}

void BeginScreen::CleanUp()
{
	d3d::Release<IDirect3DVertexBuffer9*>(m_vb);
	d3d::Release<IDirect3DTexture9*>(m_tex);
}

void BeginScreen::display( )
{
	if (g_pd3dDevice)
	{
		g_pd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		g_pd3dDevice->BeginScene();

		g_pd3dDevice->SetStreamSource(0, m_vb, 0, sizeof(Vertex));
		g_pd3dDevice->SetFVF(FVF1);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		g_pd3dDevice->EndScene();
		g_pd3dDevice->Present(0, 0, 0, 0);
	}
}

BeginScreen::BeginScreen()
{
}


BeginScreen::~BeginScreen()
{
}
