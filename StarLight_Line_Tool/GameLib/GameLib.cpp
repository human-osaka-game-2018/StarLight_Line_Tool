#include "GameLib.h"

#include <windows.h>

#include <d3dx9.h>

#include "DX\DX.h"
#include "Wnd\Wnd.h"

Wnd* GameLib::m_pWnd = nullptr;

DX* GameLib::m_pDX = nullptr;

VOID GameLib::RunFunc(VOID(*pMainFunc)())
{
	timeBeginPeriod(1);	//時間の有効数字の設定

	DWORD frameSyncPrev		= timeGetTime();
	DWORD frameSyncCurrent	= timeGetTime();

	while (!m_pWnd->IsPostedQuitMessage())
	{
		if (m_pWnd->ExistsWinMSG()) continue;

		frameSyncCurrent = timeGetTime();

		const INT DEFAULT_FPS = 60;
		if (frameSyncCurrent - frameSyncPrev < 1000 / DEFAULT_FPS)
		{
			continue;
		}

		m_pWnd->ResizeWnd();

		m_pDX->PrepareMessageLoop();

		pMainFunc();

		m_pDX->CleanUpMessageLoop();

		frameSyncPrev = frameSyncCurrent;
	}
}
