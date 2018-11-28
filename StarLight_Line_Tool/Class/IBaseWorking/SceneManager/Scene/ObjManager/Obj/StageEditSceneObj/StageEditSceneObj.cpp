#include "StageEditSceneObj.h"

#include <windows.h>
#include <tchar.h>
#include <math.h>

#include <vector>

#include <d3dx9.h>

#include "../Obj.h"

VOID StageEditStars::Update()
{
	const FLOAT SCROLL_MAX = 1000.0f;
	const FLOAT SCROLL_SPEED = 5.0f;

	if (m_rGameLib.KeyboardIsHeld(DIK_UP))
	{
		m_scrollBottom = (m_scrollBottom + SCROLL_SPEED > SCROLL_MAX) ? SCROLL_MAX : m_scrollBottom + SCROLL_SPEED;
	}

	if (m_rGameLib.KeyboardIsHeld(DIK_DOWN))
	{
		m_scrollBottom = (m_scrollBottom - SCROLL_SPEED < 0.0f) ? 0.0f : m_scrollBottom - SCROLL_SPEED;
	}
	
	POINT cursorPos;
	m_rGameLib.CursorPos(&cursorPos);

	ObjData cursorData;
	cursorData.m_center = { static_cast<FLOAT>(cursorPos.x), static_cast<FLOAT>(cursorPos.y), 0.0f };
	const FLOAT CURSOR_HALF_SCALE = 0.5f;
	cursorData.m_halfScale = { CURSOR_HALF_SCALE, CURSOR_HALF_SCALE, 0.0f };

	CustomVertex cursor[4];
	m_rGameLib.CreateRect(cursor, cursorData);

	if (!m_rGameLib.MouseIsPressed(DIM_LEFT)) return;

	for (INT i = 0; i < STAR_MAX; ++i)
	{
		if (!CollidesCircles(&m_select[4 * i], cursor)) continue;

		m_selectingStarType = i;
	}
}

VOID StageEditStars::Render()
{
	m_rGameLib.SetCameraTransform();

	ObjData selectData[STAR_MAX];

	const DWORD STAR_COLORS[STAR_MAX] =	//! ”’ Â —Î
	{
		0x70FEFEFE,
		0x7058FA82,
		0x702ECCFA
	};

	const DWORD SELECTING_STAR_COLORS[STAR_MAX] =
	{
		0xFFFEFEFE,
		0xFF58FA82,
		0xFF2ECCFA
	};
	
	for (INT i = 0; i < STAR_MAX; ++i)
	{
		selectData[i].m_center		= { m_WND_SIZE.m_x * 0.05f, m_WND_SIZE.m_y * (0.7f + 0.11f * i), m_Z };	//! Œ»•¨‡‚í‚¹
		selectData[i].m_halfScale	= { m_WND_SIZE.m_y * 0.04f, m_WND_SIZE.m_y * 0.04f, 0.0f };				//! Œ»•¨‡‚í‚¹

		selectData[i].m_aRGB = STAR_COLORS[i];
		if (i == m_selectingStarType) selectData[i].m_aRGB = SELECTING_STAR_COLORS[i];

		m_rGameLib.CreateRect(&m_select[4 * i], selectData[i]);
		m_rGameLib.Render(&m_select[4 * i], m_rGameLib.GetTex(_T("StarSelectIcon")));
	}
}
