#include "DX3D.h"

#include <windows.h>
#include <tchar.h>

#include <d3dx9.h>

#include "D3DPP\D3DPP.h"
#include "ColorBlender\ColorBlender.h"
#include "Light\Light.h"
#include "TexStorage\TexStorage.h"
#include "Camera\Camera.h"
#include "CustomVertexEditor\CustomVertexEditor.h"
#include "Renderer\Renderer.h"
#include "../../Wnd/Data/RectSize.h"
#include "CustomVertexEditor\Data\CustomVertex.h"
#include "CustomVertexEditor\Data\ObjData.h"
#include "FbxStorage\FbxStorage.h"
#include "FbxStorage\FbxRelated\FbxRelated.h"

DX3D::DX3D(HWND hWnd, RectSize wndSize, LPDIRECT3D9 pD3D)
	:m_HWND(hWnd), m_D3DPP(new D3DPP(m_HWND, wndSize))
{
	Create(pD3D);

	m_pDX3DDev->SetRenderState(D3DRS_ZENABLE, TRUE);
	m_pDX3DDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pColorBlender = new ColorBlender(m_pDX3DDev);
	m_pColorBlender->DefaultColorBlending();

	m_pLight = new Light(m_pDX3DDev);
	m_pLight->DefaultLighting();

	m_pTexStorage = new TexStorage(m_pDX3DDev);

	m_pCamera = new Camera(m_pDX3DDev);

	m_pCustomVertex = new CustomVertexEditor(m_pDX3DDev);

	m_pRenderer = new Renderer(m_pDX3DDev);

	m_pFbxStorage = new FbxStorage(m_pDX3DDev);

	InitViewPort();
}

VOID DX3D::ToggleWndMode()
{
	D3DPRESENT_PARAMETERS D3DPP = m_D3DPP->ToggleD3DPPWndMode();

	HRESULT hr = m_pDX3DDev->Reset(&D3DPP);	//スワップチェーンのタイプ、サイズ、およびフォーマットをリセット
	if (FAILED(hr))
	{
		OnFailedChangeWndMode(hr);

		return;
	}

	LONG overlapWindowStyle	= (D3DPP.Windowed) ? WS_OVERLAPPEDWINDOW : WS_POPUP;
	LONG windowStyle		= overlapWindowStyle | WS_VISIBLE;

	SetWindowLong(
		m_HWND,
		GWL_STYLE,
		windowStyle);

	m_pColorBlender->DefaultColorBlending();

	InitViewPort();
}

VOID DX3D::Create(LPDIRECT3D9 pD3D)
{
	D3DPRESENT_PARAMETERS D3DPP = m_D3DPP->GetD3DPRESENT_PARAMETERS();

	if (SUCCEEDED(pD3D->CreateDevice(	//描画をハードウェアに依存させる 軽い
						D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
						m_HWND,
						D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
						&D3DPP,
						&m_pDX3DDev)))
	{
		return;
	}

	MessageBox(
		0, 
		_T("HALモードでDIRECT3Dデバイスを作成できません\nREFモードで再試行します"),
		NULL, 
		MB_OK);

	if (SUCCEEDED(pD3D->CreateDevice(	//描画をソフトウェアに依存させる 重い
						D3DADAPTER_DEFAULT, D3DDEVTYPE_REF,
						m_HWND,
						D3DCREATE_MIXED_VERTEXPROCESSING | D3DCREATE_MULTITHREADED,
						&D3DPP,
						&m_pDX3DDev)))
	{
		return;
	}

	MessageBox(
		0,
		_T("DIRECT3Dデバイスの作成に失敗しました"),
		NULL, 
		MB_OK);

	DestroyWindow(m_HWND);	//WM_DESTROYをWndProcに投げる
}

VOID DX3D::OnFailedChangeWndMode(HRESULT resetRetVal)
{
	if (resetRetVal == D3DERR_DEVICELOST)
	{
		return;				//デバイスがロストとしたときの対処
	}

	DestroyWindow(m_HWND);	//WM_DESTROYをWndProcに投げる
}

inline VOID DX3D::InitViewPort()
{
	D3DPRESENT_PARAMETERS D3DPP = m_D3DPP->GetD3DPRESENT_PARAMETERS();

	D3DVIEWPORT9 viewPort;
	viewPort.Width	= D3DPP.BackBufferWidth;
	viewPort.Height = D3DPP.BackBufferHeight;
	viewPort.MinZ	= 0.0f;
	viewPort.MaxZ	= 1.0f;
	viewPort.X		= 0;
	viewPort.Y		= 0;
	m_pDX3DDev->SetViewport(&viewPort);
}
