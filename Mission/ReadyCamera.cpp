#include "Camera.h"
#include "ReadyCamera.h"

D3DXMATRIXA16* ReadyCamera::SetView(D3DXVECTOR3* pvEye, D3DXVECTOR3 *pvLookat, D3DXVECTOR3 *pvUp)
{
	m_vPos = *pvEye;
	m_vLook = *pvLookat;
	m_vUp = *pvUp;
	// vView(카메라가 향하는 단위방향벡터) = | Look - Eye |
	// vView는 카메라의 z축의 해당. (전진 할때)
	D3DXVec3Normalize(&m_vView, &(m_vLook - m_vPos));
	// m_vCross(카메라의 측면벡터 cross( view, up )). 카메라의 x축의 해당
	// Cross = UP × View
	D3DXVec3Cross(&m_vCross, &m_vUp, &m_vView);
	D3DXMatrixLookAtLH(&m_matView, &m_vPos, &m_vLook, &m_vUp);
	// 역행렬
	D3DXMatrixInverse(&m_matBill, NULL, &m_matView);
	m_matBill._41 = 0.0f;
	m_matBill._42 = 0.0f;
	m_matBill._43 = 0.0f;
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &m_matView);
	return&m_matView;
}



D3DXMATRIXA16* ReadyCamera::strafe(float units) //왼쪽 오른쪽
{
	D3DXVECTOR3 vNewEye = m_vPos;
	D3DXVECTOR3 vNewDst = m_vLook;
	D3DXVECTOR3 vMove;
	// X축 방향이므로
	// m_vCross를 단위벡터로
	D3DXVec3Normalize(&vMove, &m_vCross);
	// 이동할 거리에 실수값을 곱해서 거리를 구함.
	vMove *= units;
	// 카메라의 위치를 나타내는 변수에 거리값을 더함.
	vNewEye += vMove;
	vNewDst += vMove;
	return SetView(&vNewEye, &vNewDst, &m_vUp);
}
D3DXMATRIXA16* ReadyCamera::fly(float units) //위 아래
{
	D3DXVECTOR3 vNewEye = m_vPos;
	D3DXVECTOR3 vNewDst = m_vLook;
	D3DXVECTOR3 vMove;
	// Y축 방향이므로 
	// m_vUp을 단위벡터로.
	D3DXVec3Normalize(&vMove, &m_vUp);
	// 이동할 거리에 해당하는 실수값을 곱해서 거리를 구함.
	vMove *= units;
	// 카메라의 위치를 나타내는 변수에 더해줌.
	vNewEye += vMove;
	vNewDst += vMove;
	return SetView(&vNewEye, &vNewDst, &m_vUp);
}
D3DXMATRIXA16* ReadyCamera::walk(float units)//앞 뒤
{
	D3DXVECTOR3 vNewEye = m_vPos;
	D3DXVECTOR3 vNewDst = m_vLook;
	D3DXVECTOR3 vMove;
	// Z축 방향으로의 전진이므로
	// m_vView를 단위벡터로.
	D3DXVec3Normalize(&vMove, &m_vView);
	// 이동할 거리에 해당하는 실수값을 곱함
	vMove *= units;
	// 카메라의 위치를 나타내는 변수에 더해줌.
	vNewEye += vMove;
	vNewDst += vMove;
	return SetView(&vNewEye, &vNewDst, &m_vUp);
}

D3DXMATRIXA16* ReadyCamera::pitch(float angle)		//pitch,yaw ,roll수정 ㄱ
{

	D3DXMATRIXA16 matRot;
	// nNewDst = vView * M(rotation) + vEye;
	// m_vCross는 x축에 해당.
	// M(rotation)을 구하는 식.
	D3DXMatrixRotationAxis(&matRot, &m_vCross, angle);
	D3DXVECTOR3 vNewDst;
	// vView * M(rotation)을 구하는 식.
	D3DXVec3TransformCoord(&vNewDst, &m_vView, &matRot);
	// +Eye를 구함.
	vNewDst += m_vPos;
	return SetView(&m_vPos, &vNewDst, &m_vUp);
}
D3DXMATRIXA16* ReadyCamera::yaw(float angle, IDirect3DDevice9* device)
{
	D3DXMATRIXA16 mRot;
	// nNewDst = vView * M(rotation) + vEye
	// m_vUp은 y축에 해당. 
	// M(rotation)을 구하는 식.
	D3DXMatrixRotationAxis(&mRot, &m_vUp, angle);
	D3DXVECTOR3 vNewDst;
	// vView * M(rotation)을 구하는 식.
	D3DXVec3TransformCoord(&vNewDst, &m_vView, &mRot);
	// +vEye를 구하는 식.
	vNewDst += m_vPos;
	return SetView(&m_vPos, &vNewDst, &m_vUp);
}


void ReadyCamera::CleanUp()
{
	g_pd3dDevice->Release();
}
void ReadyCamera::setDevice(IDirect3DDevice9* pd3dDevice)
{
	g_pd3dDevice = pd3dDevice;
}

void ReadyCamera::setUp(D3DXVECTOR3 ballPosition)
{
	//투영
	
	m_vPos = D3DXVECTOR3(ballPosition.x, ballPosition.y + 50.0f, ballPosition.z - 100.0f);
	
	m_vLook = ballPosition;
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI * 0.5, 1024 / float(768), 1.0f, 1000.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);
}

void ReadyCamera::update(D3DXVECTOR3 ballPosition)
{
	m_vPos = D3DXVECTOR3(ballPosition.x, ballPosition.y + 50.0f, ballPosition.z - 100.0f);
	m_vView = -m_vPos;/*****************************어설프게 고쳐짐 (다시 보기 )*/			//카메라 vView가 0 인 탓에 처음 방향키누르면 고장났었음
	m_vLook = ballPosition;
	D3DXMatrixLookAtLH(&V, &m_vPos, &m_vLook, &m_vUp);		//공이 제자리 오기 때문에 0,0,-100에서 바라보게한다

	g_pd3dDevice->SetTransform(D3DTS_VIEW, &V);
}
void ReadyCamera::keyState(float timeDelta, gameType* g_eType)
{

	if (GetAsyncKeyState('W') & 0x8000f)	//z축
		ReadyCamera::walk(100.0f * timeDelta);
	if (GetAsyncKeyState('S') & 0x8000f)
		ReadyCamera::walk(-100.0f * timeDelta);
	if (GetAsyncKeyState('A') & 0x8000f)	//x축
		ReadyCamera::strafe(-100.0f * timeDelta);

	if (GetAsyncKeyState('D') & 0x8000f)
		ReadyCamera::strafe(100.0f * timeDelta);

	if (GetAsyncKeyState('R') & 0x8000f)		//y축 
		ReadyCamera::fly(100.0f * timeDelta);

	if (GetAsyncKeyState('F') & 0x8000f)
		ReadyCamera::fly(-100.0f * timeDelta);


	if (GetAsyncKeyState(VK_DOWN) & 0x8000f)
		ReadyCamera::pitch(5.0f * timeDelta);
	if (GetAsyncKeyState(VK_LEFT) & 0x8000f)
		ReadyCamera::yaw(-5.0f * timeDelta, g_pd3dDevice);
	if (GetAsyncKeyState(VK_UP) & 0x8000f)
		ReadyCamera::pitch(-5.0f * timeDelta);
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		ReadyCamera::yaw(5.0f * timeDelta, g_pd3dDevice);
}
// 카메라 행렬을 생성하기 위한 기본 벡터값들을 설정한다.
D3DXMATRIXA16* ReadyCamera::MoveTo(D3DXVECTOR3* pv)
{
	D3DXVECTOR3 dv = *pv - m_vPos;
	m_vPos = *pv;
	m_vLook += dv;
	return SetView(&m_vPos, &m_vLook, &m_vUp);
}

ReadyCamera::ReadyCamera()
{
	m_cameraType = AIRCRAFT;

	
	m_vPos = D3DXVECTOR3(0.0f, 100.0f, -100.0f);
	m_vRight = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_vUp = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_vLook = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}


ReadyCamera::~ReadyCamera()
{
}
