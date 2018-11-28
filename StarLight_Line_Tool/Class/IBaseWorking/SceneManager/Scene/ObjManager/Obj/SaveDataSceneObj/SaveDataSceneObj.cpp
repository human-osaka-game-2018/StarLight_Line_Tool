#include "SaveDataSceneObj.h"

#include <windows.h>
#include <tchar.h>

#include <d3dx9.h>

#include "../Obj.h"

VOID SaveDataSaveDatas::Render()
{
	ObjData frameData;
	frameData.m_halfScale = { m_WND_SIZE.m_x * 0.2f, m_WND_SIZE.m_y * 0.12f, 0.0f };				//! 現物合わせ

	CustomVertex frame[4];

	for (INT i = 0; i < m_SAVE_DATAS_MAX; ++i)
	{
		frameData.m_center = { m_WND_SIZE.m_x * 0.25f, m_WND_SIZE.m_y * (0.2f + 0.3f * i), m_Z };	//! 現物合わせ

		m_rGameLib.CreateRect(frame, frameData);

		m_rGameLib.Render(frame, m_rGameLib.GetTex(_T("Frame")));
	}

	ObjData detailFrameData;
	detailFrameData.m_center	= { m_WND_SIZE.m_x * 0.75f, m_WND_SIZE.m_y * 0.5f, m_Z };
	detailFrameData.m_halfScale = { m_WND_SIZE.m_x * 0.2f, m_WND_SIZE.m_y * 0.4f, 0.0f };			//! 現物合わせ

	CustomVertex detailFrame[4];
	m_rGameLib.CreateRect(detailFrame, detailFrameData);

	m_rGameLib.Render(detailFrame, m_rGameLib.GetTex(_T("DetailFrame")));
}
