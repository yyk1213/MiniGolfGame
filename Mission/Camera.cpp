#include "Camera.h"

Camera::Camera()
{


}
D3DXVECTOR3 Camera::getPosition()
{
	return m_vPos;
}

void Camera::setDevice(IDirect3DDevice9* pd3dDevice)
{

}


Camera::~Camera()
{
}