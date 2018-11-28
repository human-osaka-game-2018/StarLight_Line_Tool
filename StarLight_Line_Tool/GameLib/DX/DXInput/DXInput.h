/**
* @file DXInput.h
* @brief 入力デバイス関係クラスのFacadeのヘッダ
* @author Harutaka-Tsujino
*/

#ifndef DX_INPUT_H
#define DX_INPUT_H

#include <windows.h>

#include <d3dx9.h>
#include <dinput.h>

#include "InputDev\InputDev.h"
#include "InputDev\Mouse\Mouse.h"
#include "InputDev\Keyboard\Keyboard.h"

/**
* @brief 入力デバイス関係クラスのFacade
*/
class DXInput
{
public:
	DXInput(HWND hWnd) :m_HWND(hWnd)
	{
		Create();

		m_pMouse	= new Mouse(m_HWND,m_pDXInput);
		m_pKeyboard	= new Keyboard(m_HWND, m_pDXInput);
	}

	~DXInput()
	{
		delete m_pMouse;
		delete m_pKeyboard;

		m_pDXInput->Release();
	}

	/**
	* @brief マウスとキーボードの入力状態の更新を行う,メッセージループの始まりで呼ぶ
	*/
	inline VOID UpdataInputState()
	{
		m_pMouse->UpdataInputState();
		m_pKeyboard->UpdataInputState();
	}

	/**
	* @brief マウスとキーボードの入力状態の保存を行う,メッセージループの終わりで呼ぶ
	*/
	inline VOID StorePrevInputState()
	{
		m_pMouse->StorePrevInputState();
		m_pKeyboard->StorePrevInputState();
	}

	inline BOOL MouseIsPressed(INT key) const
	{
		return m_pMouse->IsPressed(key);
	}

	inline BOOL MouseIsHeld(INT key) const
	{
		return m_pMouse->IsHeld(key);
	}

	inline BOOL MouseIsReleased(INT key) const
	{
		return m_pMouse->IsReleased(key);
	}

	inline BOOL MouseIsNeutral(INT key) const
	{
		return m_pMouse->IsNeutral(key);
	}

	inline BOOL MouseAnyKeyIsPressed() const
	{
		return m_pMouse->AnyKeyIsPressed();
	}

	inline VOID CursorPos(POINT* pPos) const
	{
		m_pMouse->CursorPos(pPos);
	}

	inline LONG MouseWheelScrollingFromPrev() const
	{
		return m_pMouse->WheelScrollingFromPrev();
	}

	inline BOOL KeyboardIsPressed(INT key) const
	{
		return m_pKeyboard->IsPressed(key);
	}

	inline BOOL KeyboardIsHeld(INT key) const
	{
		return m_pKeyboard->IsHeld(key);
	}

	inline BOOL KeyboardIsReleased(INT key) const
	{
		return m_pKeyboard->IsReleased(key);
	}

	inline BOOL KeyboardIsNeutral(INT key) const
	{
		return m_pKeyboard->IsNeutral(key);
	}

	inline BOOL KeyboardAnyKeyIsPressed() const
	{
		return m_pKeyboard->AnyKeyIsPressed();
	}

private:
	VOID Create() 
	{
		DirectInput8Create(
			GetModuleHandle(NULL),
			DIRECTINPUT_VERSION, IID_IDirectInput8,
			(VOID**)&m_pDXInput,
			NULL);
	}

	const HWND m_HWND = nullptr;

	LPDIRECTINPUT8 m_pDXInput = nullptr;

	Mouse* m_pMouse			= nullptr;
	Keyboard* m_pKeyboard	= nullptr;
};

#endif //! DX_INPUT_H
