#include "PowerProcess.h"



void PowerProcess::setUp(IDirect3DDevice9* pd3dDevice)
{
	g_pd3dDevice = pd3dDevice;
	D3DXCreateFont(g_pd3dDevice, 25, 0, FW_BOLD, 1, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &m_font);
	D3DXCreateSprite(g_pd3dDevice, &m_sprite);
	D3DXCreateSprite(g_pd3dDevice, &m_sprite);
}
void PowerProcess::Display(int powerNum)		//int a �Ѱܿͼ� %d�� 10 20 30 40 50 �ѱ���غ���
{
	if (m_sprite)
	{
		sprintf(numString, "power : %d", powerNum);
		m_sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
		RECT rt = { 800,150,0,0 };			//�ؽ�Ʈ ��ġ ����
		rt.bottom = 1000;
		m_font->DrawText(m_sprite, numString, -1, &rt, DT_NOCLIP, D3DCOLOR_XRGB(0, 0, 0));

		m_sprite->End();
	}
}
void PowerProcess::Cleanup()
{
	m_font->Release();
	m_sprite->Release();
	g_pd3dDevice->Release();
}
void PowerProcess::keyState(int* power)		//ball ��ü �޾ƿͼ� ���� �����ϰ� �Ŀ� ��ġ ����ش�.
{

	if (GetAsyncKeyState('1') & 0x0001f && *power<20)	//x��		//�� �ȿ��� �Ŀ�ó���� �ؾߵ�		//Ű �ѹ��� ó�� 0001 ���� �ϸ�ũ ����
	{							
		*power+=1;
	}
	if (GetAsyncKeyState('2') & 0x0001f && *power>1)
	{
		*power-=1;

	}
	
}

PowerProcess::PowerProcess()
{

}


PowerProcess::~PowerProcess()
{
}
