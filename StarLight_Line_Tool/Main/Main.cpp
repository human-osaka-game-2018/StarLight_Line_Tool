#include "Main.h"

#include <windows.h>
#include <tchar.h>
#include <crtdbg.h>

#include <cstdio>
#include <cstdlib>

#include "../Class/Singleton/Singleton.h"
#include "../GameLib/GameLib.h"
#include "../Class/IBaseWorking/SceneManager/SceneManager.h"

#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

VOID Func()
{
	GameLib& rGameLib = GameLib::GetInstance();

	if (rGameLib.KeyboardIsHeld(DIK_LALT) &&
		(rGameLib.KeyboardIsPressed(DIK_RALT)))
	{
		rGameLib.ToggleWndMode();
	}

	SceneManager& rSceneManager = SceneManager::GetInstance();
	rSceneManager.Factory();
	rSceneManager.Update();
	rSceneManager.Render();
}

INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT iCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	GameLib::Create(hInst, _T("StarLight Line"));

	GameLib& rGameLib = GameLib::GetInstance();

	rGameLib.RunFunc(Func);
}
