#include "Mouse.h"

#include <windows.h>

#include <dinput.h>

#include "Enum\DIM.h"

VOID Mouse::Create(LPDIRECTINPUT8 pDXInput)
{
	pDXInput->CreateDevice(
				GUID_SysMouse,
				&m_pDInputDev,
				NULL);

	m_pDInputDev->SetDataFormat(&c_dfDIMouse);

	m_pDInputDev->SetCooperativeLevel(
					m_HWND,
					DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);

	DIPROPDWORD mouseProp;
	mouseProp.diph.dwSize		= sizeof(DIPROPDWORD);
	mouseProp.diph.dwHeaderSize = sizeof(mouseProp.diph);
	mouseProp.diph.dwObj		= 0;
	mouseProp.diph.dwHow		= DIPH_DEVICE;
	mouseProp.dwData			= DIPROPAXISMODE_REL;

	m_pDInputDev->SetProperty(DIPROP_AXISMODE, &mouseProp.diph);
}

VOID Mouse::CheckInputStateDetatils()
{
	BOOL isInputtedPrev	= FALSE;
	BOOL isInputted		= FALSE;

	ZeroMemory(&m_details, sizeof(BOOL) * DIM_MAX);
	for (INT i = 0; i < DIM_MAX; ++i)
	{
		isInputtedPrev	= IsInputted(m_prevState.rgbButtons[i]);
		isInputted		= IsInputted(m_state.rgbButtons[i]);

		if (isInputtedPrev)
		{
			m_details[i] = (isInputted) ? IND_HOLD : IND_RELEASE;

			continue;
		}

		m_details[i] = (isInputted) ? IND_PRESS : IND_NEUTRAL;
	}
}
