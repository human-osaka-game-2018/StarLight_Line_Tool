#include "Keyboard.h"

#include <windows.h>

#include <dinput.h>

VOID Keyboard::Create(LPDIRECTINPUT8 pDXInput)
{
	pDXInput->CreateDevice(
				GUID_SysKeyboard, &m_pDInputDev,
				NULL);

	m_pDInputDev->SetDataFormat(&c_dfDIKeyboard);

	m_pDInputDev->SetCooperativeLevel(
					m_HWND,
					DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
}

VOID Keyboard::CheckInputStateDetatils()
{
	BOOL isInputtedPrev	= FALSE;
	BOOL isInputted		= FALSE;

	const INT KEYBOARD_KEYS_MAX = 256;
	ZeroMemory(m_details, sizeof(BOOL) * KEYBOARD_KEYS_MAX);
	for (INT i = 0; i < KEYBOARD_KEYS_MAX; ++i)
	{
		isInputtedPrev	= IsInputted(m_prevDiks[i]);
		isInputted		= IsInputted(m_diks[i]);

		if (isInputtedPrev)
		{
			m_details[i] = (isInputted) ? IND_HOLD : IND_RELEASE;

			continue;
		}

		m_details[i] = (isInputted) ? IND_PRESS : IND_NEUTRAL;
	}
}
