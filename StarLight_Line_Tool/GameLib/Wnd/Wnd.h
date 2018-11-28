/**
* @file Wnd.h
* @brief ウィンドウクラスのヘッダ
* @author Harutaka-Tsujino
*/

#ifndef WND_H
#define WND_H

#include <windows.h>

#include "Data\RectSize.h"

/**
* @brief ウィンドウクラス
*/
class Wnd
{
public:
	Wnd(const HINSTANCE hInst, const TCHAR* pAppName);                                      //WinMainからHINSTANCEを取得 Crate()を呼ぶ
	~Wnd() {};

	/**
	* @brief ウィンドウハンドルを取得する
	* @return ウィンドウハンドル
	*/
	inline const HWND GetHWND() const
	{
		return m_hWnd;
	}

	/**
	* @brief WinMessageの構造体を取得する
	* @return WinMessageの構造体
	*/
	inline MSG GetMSG() const
	{
		return m_msg;
	}

	/**
	* @brief ウィンドウのサイズを取得する
	* @return ウィンドウのサイズ
	*/
	inline RectSize GetWndSize() const
	{
		return m_WND_SIZE;
	}

	/**
	* @brief ウィンドウを終了するメッセージが投げられたか
	* @return 投げられていたらTRUE
	*/
	inline BOOL IsPostedQuitMessage() const
	{
		if (m_msg.message == WM_QUIT) return TRUE;

		return FALSE;
	}

	/**
	* @brief WinMessageが何かあるか
	* @return あればTRUE
	*/
	BOOL ExistsWinMSG();

	/**
	* @brief クライアント領域をm_WND_SIZEと同じにする
	*/
	VOID ResizeWnd() const;

private:
	VOID Create(const HINSTANCE hInst, const TCHAR* pAppName);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);    //ウィンドウを生成するときにProcの関数ポインタが必要なのでstatic
																							//IDX3D経由でDX3DのメソッドToggleWndMode()を呼ぶ
	HWND m_hWnd = nullptr;

	MSG m_msg;

	const RectSize m_WND_SIZE = { 1280, 720 };
};

#endif // !WND_H
