#include"DX.h"

#include <windows.h>
#include <tchar.h>

#include <d3dx9.h>

VOID DX::Create()
{
	if (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION))
	{
		return;
	}

	MessageBox(
		0,
		_T("Direct3Dの作成に失敗しました"),
		NULL,
		MB_OK);
}
