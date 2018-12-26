#pragma once

/// DirectX ���� �ʱ�ȭ , ��������

class CD3dSystem
{
private:

	/// DX9 ����̽�
	LPDIRECT3D9         m_pD3D = NULL; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9   m_pd3dDevice = NULL; // Our rendering device

public:

	/// ������
	CD3dSystem();

	/// �Ҹ���
	~CD3dSystem();

	/// �ʱ�ȭ
	bool Initialrize();
};