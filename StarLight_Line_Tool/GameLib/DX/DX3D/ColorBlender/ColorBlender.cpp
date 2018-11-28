#include "ColorBlender.h"

#include <windows.h>

#include <d3dx9.h>

VOID ColorBlender::DefaultColorBlending() const
{
	DefaultBlendMode();
	m_rpDX3D_DEV->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_rpDX3D_DEV->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_rpDX3D_DEV->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_rpDX3D_DEV->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	m_rpDX3D_DEV->SetRenderState(D3DRS_ALPHAREF, 0x00);

	m_rpDX3D_DEV->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_rpDX3D_DEV->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	m_rpDX3D_DEV->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_rpDX3D_DEV->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
}
