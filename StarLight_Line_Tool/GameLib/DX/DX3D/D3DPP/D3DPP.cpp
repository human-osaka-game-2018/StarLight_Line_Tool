#include "D3DPP.h"

#include <windows.h>

#include <d3dx9.h>

#include "../../../Wnd/Data/RectSize.h"

VOID D3DPP::InitD3DPP(D3DPRESENT_PARAMETERS* pD3DPP)
{
	ZeroMemory(pD3DPP, sizeof(D3DPRESENT_PARAMETERS));

	pD3DPP->Windowed				= m_isWindow;
	pD3DPP->BackBufferFormat		= D3DFMT_X8R8G8B8;
	pD3DPP->BackBufferCount			= 1;
	pD3DPP->SwapEffect				= D3DSWAPEFFECT_DISCARD;
	pD3DPP->EnableAutoDepthStencil	= TRUE;
	pD3DPP->AutoDepthStencilFormat	= D3DFMT_D16;
	pD3DPP->BackBufferWidth			= m_WND_SIZE.m_x;
	pD3DPP->BackBufferHeight		= m_WND_SIZE.m_y;

	if (m_isWindow) return;

	pD3DPP->hDeviceWindow			= m_HWND;
	pD3DPP->PresentationInterval	= D3DPRESENT_INTERVAL_DEFAULT;
}
