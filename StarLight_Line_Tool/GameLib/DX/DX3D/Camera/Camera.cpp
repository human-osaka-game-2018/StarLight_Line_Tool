#include "Camera.h"

#include <windows.h>

#include <d3dx9.h>

VOID Camera::SetTransform()
{
	D3DXMatrixLookAtLH(
		&m_view,
		&m_cameraPos,
		&m_eyePt,
		&m_cameraOverhead);

	m_rpDX3D_DEV->SetTransform(D3DTS_VIEW, &m_view);

	D3DVIEWPORT9 viewPort;
	m_rpDX3D_DEV->GetViewport(&viewPort);
	FLOAT aspect = (FLOAT)viewPort.Width / (FLOAT)viewPort.Height;

	const INT DEFAULT_EYE_RADIAN = 60;

	const FLOAT DEFAULT_NEAR	= 0.01f;	//0.0fにすると全ての物体のz値が0.0fになる
	const FLOAT DEFAULT_FAR		= 10000.0f;

	D3DXMATRIX projection;
	D3DXMatrixPerspectiveFovLH(
		&projection,
		D3DXToRadian(DEFAULT_EYE_RADIAN),
		aspect,
		DEFAULT_NEAR,
		DEFAULT_FAR);

	m_rpDX3D_DEV->SetTransform(D3DTS_PROJECTION, &projection);
}
