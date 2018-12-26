#pragma once

/// DirectX 관련 초기화 , 렌더링등

class CD3dSystem
{
private:

	/// DX9 디바이스
	LPDIRECT3D9         m_pD3D = NULL; // Used to create the D3DDevice
	LPDIRECT3DDEVICE9   m_pd3dDevice = NULL; // Our rendering device

public:

	/// 생성자
	CD3dSystem();

	/// 소멸자
	~CD3dSystem();

	/// 초기화
	bool Initialrize();
};