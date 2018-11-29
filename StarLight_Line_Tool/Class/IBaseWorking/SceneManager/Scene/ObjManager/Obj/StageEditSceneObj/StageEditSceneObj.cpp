#include "StageEditSceneObj.h"

#include <windows.h>
#include <tchar.h>
#include <math.h>

#include <vector>

#include <d3dx9.h>

#include "../Obj.h"
#include "Enum\STAR_TYPE.h"
#include "Enum\Y_VEC.h"

VOID StageEditBack::Render()
{
	m_rGameLib.SetCameraTransform();

	ObjData data;
	data.m_center		= { m_WND_SIZE.m_x * 0.5f, m_WND_SIZE.m_y * 0.5f, m_Z };	//! 現物合わせ
	data.m_halfScale	= { m_WND_SIZE.m_x * 0.5f, m_WND_SIZE.m_y * 0.5f, 0.0f };	//! 現物合わせ

	data.m_texUV =
	{
		0.0f,
		(m_rScrollBottom + m_WND_SIZE.m_y) / m_WND_SIZE.m_y,
		1.0f,
		m_rScrollBottom / m_WND_SIZE.m_y,
	};

	CustomVertex back[4];
	m_rGameLib.CreateRect(back, data);

	LPDIRECT3DTEXTURE9 backTex;

	switch (m_rBeatsNum)
	{
	case 16:
		backTex = m_rGameLib.GetTex(_T("16Back"));

		break;

	case 8:
		backTex = m_rGameLib.GetTex(_T("8Back"));

		break;

	case 4:
		backTex = m_rGameLib.GetTex(_T("4Back"));

		break;

	case 2:
		backTex = m_rGameLib.GetTex(_T("2Back"));

		break;

	case 1:
		backTex = m_rGameLib.GetTex(_T("1Back"));

		break;

	default:
		break;
	}

	m_rGameLib.Render(back, backTex);
}

VOID StageEditStars::Update()
{
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
		if (!m_rGameLib.CollidesCircles(&m_select[4 * i], cursor)) continue;

		m_selectingStarType = i;
	}
}

VOID StageEditStars::Render()
{
	m_rGameLib.SetCameraTransform();

	ObjData selectData[STAR_MAX];

	const DWORD STAR_COLORS[STAR_MAX] =																		//! 白 青 緑
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
	
	const FLOAT SELECT_HALF_SCALE = m_WND_SIZE.m_y * 0.04f;

	for (INT i = 0; i < STAR_MAX; ++i)
	{
		selectData[i].m_center		= { m_WND_SIZE.m_x * 0.05f, m_WND_SIZE.m_y * (0.7f + 0.11f * i), m_Z };	//! 現物合わせ
		selectData[i].m_halfScale	= { SELECT_HALF_SCALE, SELECT_HALF_SCALE, 0.0f };						//! 現物合わせ

		selectData[i].m_aRGB = STAR_COLORS[i];
		if (i == m_selectingStarType) selectData[i].m_aRGB = SELECTING_STAR_COLORS[i];

		m_rGameLib.CreateRect(&m_select[4 * i], selectData[i]);
		m_rGameLib.Render(&m_select[4 * i], m_rGameLib.GetTex(_T("StarSelectIcon")));
	}
}

VOID StageEditMeusurs::Update()
{
	if (!m_rGameLib.MouseIsPressed(DIM_LEFT)) return;

	POINT cursorPos;
	m_rGameLib.CursorPos(&cursorPos);

	ObjData cursorData;
	cursorData.m_center = { static_cast<FLOAT>(cursorPos.x), static_cast<FLOAT>(cursorPos.y), 0.0f };
	const FLOAT CURSOR_HALF_SCALE = 0.5f;
	cursorData.m_halfScale = { CURSOR_HALF_SCALE, CURSOR_HALF_SCALE, 0.0f };

	CustomVertex cursor[4];
	m_rGameLib.CreateRect(cursor, cursorData);

	for (INT i = 0; i < m_meusursNum; ++i)
	{
		SetBPMDealWithClickedSetter(i, cursor);
	}

	if (m_rGameLib.CollidesRects(cursor, m_meusurAdder))
	{
		++m_meusursNum;
		m_pMeusurVec.push_back(new Meusur);
	}

	if (m_meusursNum <= 1) return;

	if (m_rGameLib.CollidesRects(cursor, m_meusurDeleter))
	{
		delete m_pMeusurVec[m_meusursNum - 1];
		m_pMeusurVec.pop_back();

		--m_meusursNum;
	}
}

VOID StageEditMeusurs::Render()
{
	for (INT i = 0; i < m_meusursNum; ++i)
	{
		RenderMeusur(i);
		RenderBPMSetter(i);
	}

	ObjData adderData;
	adderData.m_center		= { m_WND_SIZE.m_x * 0.05f, m_WND_SIZE.m_y * 0.05f, m_Z };	//! 現物合わせ
	adderData.m_halfScale	= { m_WND_SIZE.m_x * 0.05f, m_WND_SIZE.m_y * 0.05f, 0.0f };	//! 現物合わせ

	m_rGameLib.CreateRect(m_meusurAdder, adderData);
	m_rGameLib.Render(m_meusurAdder, m_rGameLib.GetTex(_T("AddMeusur")));

	ObjData deleterData;
	deleterData.m_center	= { m_WND_SIZE.m_x * 0.15f, m_WND_SIZE.m_y * 0.05f, m_Z };	//! 現物合わせ
	deleterData.m_halfScale = adderData.m_halfScale;

	m_rGameLib.CreateRect(m_meusurDeleter, deleterData);
	m_rGameLib.Render(m_meusurDeleter, m_rGameLib.GetTex(_T("DeleteMeusur")));
}

VOID StageEditMeusurs::RenderMeusur(INT elementNum)
{
	const FLOAT DIGITS_GAP = m_WND_SIZE.m_x * 0.01f;

	const FLOAT DIGITS_HALF_SCALE = m_WND_SIZE.m_x * 0.01f;

	ObjData* pBPMData = m_pMeusurVec[elementNum]->m_bPMDatas;
	CustomVertex* pBPM = m_pMeusurVec[elementNum]->m_bPM;

	INT digitBPM = 0;

	const FLOAT NUMS_ILLUST_SCALE = 32.0f;
	const INT NUMS_NUM_IN_ROW = 8;
	const INT NUMS_NUM_IN_COLUMN = 2;

	const INT BEATS_NUM_IN_MEUSUR = 4;

	for (INT i = 0; i < Meusur::m_DIGITS_MAX; ++i)
	{
		pBPMData[i].m_center =
		{
			m_WND_SIZE.m_x * (0.8f + 0.02f * i),									//! 現物合わせ
			(m_rScrollBottom / BEATS_NUM_IN_MEUSUR) - m_WND_SIZE.m_y * elementNum,	//! 節と節のBPMをウィンドウの縦の長さ分ずらす スクロールする
			m_Z
		};

		pBPMData[i].m_halfScale = { DIGITS_HALF_SCALE, DIGITS_HALF_SCALE, 0.0f };

		digitBPM = m_pMeusurVec[elementNum]->m_digitBPMs[i];
		pBPMData[i].m_texUV =
		{
			NUMS_ILLUST_SCALE * (digitBPM % NUMS_NUM_IN_ROW)		/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_ROW),
			NUMS_ILLUST_SCALE * (digitBPM / NUMS_NUM_IN_ROW)		/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_COLUMN),
			NUMS_ILLUST_SCALE * (digitBPM % NUMS_NUM_IN_ROW + 1)	/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_ROW),
			NUMS_ILLUST_SCALE * (digitBPM / NUMS_NUM_IN_ROW + 1)	/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_COLUMN),
		};

		m_rGameLib.CreateRect(&pBPM[4 * i], pBPMData[i]);
		m_rGameLib.Render(&pBPM[4 * i], m_rGameLib.GetTex(_T("Nums")));
	}
}

VOID StageEditMeusurs::RenderBPMSetter(INT elementNum)
{
	ObjData* pBPMData = m_pMeusurVec[elementNum]->m_bPMDatas;

	const FLOAT BPM_SETTER_HALF_SCALE = m_WND_SIZE.m_x * 0.007f;

	ObjData* pBPMUpperDatas = m_pMeusurVec[elementNum]->m_bPMUpperDatas;
	CustomVertex* pBPMUppers = m_pMeusurVec[elementNum]->m_bPMUppers;

	ObjData* pBPMDownerDatas = m_pMeusurVec[elementNum]->m_bPMDownerDatas;
	CustomVertex* pBPMDowners = m_pMeusurVec[elementNum]->m_bPMDowners;

	for (INT di = 0; di < Meusur::m_DIGITS_MAX; ++di)
	{
		pBPMUpperDatas[di].m_center = pBPMData[di].m_center;
		SetBPMSetterUpDown(&pBPMUpperDatas[di], FALSE);
		pBPMUpperDatas[di].m_halfScale = { BPM_SETTER_HALF_SCALE, BPM_SETTER_HALF_SCALE, 0.0f };

		m_rGameLib.CreateRect(&pBPMUppers[4 * di], pBPMUpperDatas[di]);
		m_rGameLib.Render(&pBPMUppers[4 * di], m_rGameLib.GetTex(_T("BPMButton")));

		pBPMDownerDatas[di].m_center = pBPMData[di].m_center;
		SetBPMSetterUpDown(&pBPMDownerDatas[di], TRUE);
		pBPMDownerDatas[di].m_halfScale = { BPM_SETTER_HALF_SCALE, BPM_SETTER_HALF_SCALE, 0.0f };

		m_rGameLib.CreateRect(&pBPMDowners[4 * di], pBPMDownerDatas[di]);
		m_rGameLib.Render(&pBPMDowners[4 * di], m_rGameLib.GetTex(_T("BPMButton")));
	}
}

VOID StageEditMeusurs::SetBPMDealWithClickedSetter(INT elementNum, const CustomVertex* pCursor)
{
	CustomVertex* pBPMUppers = m_pMeusurVec[elementNum]->m_bPMUppers;

	CustomVertex* pBPMDowners = m_pMeusurVec[elementNum]->m_bPMDowners;

	INT* pDigitBpm = 0;

	for (INT di = 0; di < Meusur::m_DIGITS_MAX; ++di)
	{
		pDigitBpm = &(m_pMeusurVec[elementNum]->m_digitBPMs[di]);

		if (m_rGameLib.CollidesCircles(pCursor, &pBPMUppers[4 * di]))
		{
			*pDigitBpm = (*pDigitBpm + 1) % 10;

			return;
		}

		if (m_rGameLib.CollidesCircles(pCursor, &pBPMDowners[4 * di]))
		{
			*pDigitBpm = (*pDigitBpm == 0) ? 9 : *pDigitBpm - 1;

			return;
		}
	}
}

VOID StageEditNote::Update()
{
	m_pStageEditBack->Update();

	POINT cursorPos;
	m_rGameLib.CursorPos(&cursorPos);

	ObjData cursorData;
	cursorData.m_center = { static_cast<FLOAT>(cursorPos.x), static_cast<FLOAT>(cursorPos.y), 0.0f };
	const FLOAT CURSOR_HALF_SCALE = 0.5f;
	cursorData.m_halfScale = { CURSOR_HALF_SCALE, CURSOR_HALF_SCALE, 0.0f };

	CustomVertex cursor[4];
	m_rGameLib.CreateRect(cursor, cursorData);

	if (!m_rGameLib.MouseIsPressed(DIM_LEFT)) return;

	for (INT i = 0; i < YV_MAX; ++i)
	{
		if (!m_rGameLib.CollidesCircles(&m_noteNumButton[4 * i], cursor)) continue;

		if (m_noteNumButton[4 * i].m_aRGB == 0x00000000) continue;	//!ボタンが消されているときは押せない

		m_noteNum = (i == YV_LEFT) ? m_noteNum / 2 : m_noteNum * 2;	//!二倍ずつ増え 
	}
}

VOID StageEditNote::Render()
{
	m_pStageEditBack->Render();

	ObjData NoteNumButtonData[YV_MAX];

	const FLOAT NOTES_NUM_BUTTON_HALF_SCALE = m_WND_SIZE.m_x * 0.016f;												//! 現物合わせ

	for (INT i = 0; i < YV_MAX; ++i)
	{
		NoteNumButtonData[i].m_center		= { m_WND_SIZE.m_x * (0.04f + 0.07f * i), m_WND_SIZE.m_y * 0.6f, m_Z };	//! 現物合わせ
		NoteNumButtonData[i].m_halfScale	= { NOTES_NUM_BUTTON_HALF_SCALE, NOTES_NUM_BUTTON_HALF_SCALE, 0.0f };

		if ((i == YV_LEFT && m_noteNum == 1) || 
			(i == YV_RIGHT && m_noteNum == 16)) NoteNumButtonData[i].m_aRGB = 0x00000000;							//! 分の最大最少を選んでいるときボタンを消す

		if (i == YV_LEFT) NoteNumButtonData[i].m_deg.z = 180.0f;													//! 画像を反転させるため

		m_rGameLib.CreateRect(&m_noteNumButton[4 * i], NoteNumButtonData[i]);
		m_rGameLib.Render(&m_noteNumButton[4 * i], m_rGameLib.GetTex(_T("BeatesNumButton")));
	}

	const INT DIGITS_MAX = 2;

	INT NoteDigitNum[DIGITS_MAX] =																					//! 表示用の一桁ずつの分数
	{
		m_noteNum / 10,
		m_noteNum % 10,
	};

	ObjData noteDigitData[DIGITS_MAX];
	const FLOAT NOTE_DIGIT_HALF_SCALE = m_WND_SIZE.m_x * 0.01f;

	CustomVertex noteDigit[4 * DIGITS_MAX];

	const FLOAT NUMS_ILLUST_SCALE = 32.0f;
	const INT NUMS_NUM_IN_ROW = 8;
	const INT NUMS_NUM_IN_COLUMN = 2;

	for (INT i = 0; i < DIGITS_MAX; ++i)
	{
		noteDigitData[i].m_center		= { m_WND_SIZE.m_x * (0.0658f + 0.018f * i), m_WND_SIZE.m_y * 0.6f, m_Z };	//! 現物合わせ
		noteDigitData[i].m_halfScale	= { NOTE_DIGIT_HALF_SCALE, NOTE_DIGIT_HALF_SCALE, 0.0f };

		noteDigitData[i].m_texUV =
		{
			NUMS_ILLUST_SCALE * (NoteDigitNum[i] % NUMS_NUM_IN_ROW)		/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_ROW),
			NUMS_ILLUST_SCALE * (NoteDigitNum[i] / NUMS_NUM_IN_ROW)		/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_COLUMN),
			NUMS_ILLUST_SCALE * (NoteDigitNum[i] % NUMS_NUM_IN_ROW + 1)	/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_ROW),
			NUMS_ILLUST_SCALE * (NoteDigitNum[i] / NUMS_NUM_IN_ROW + 1)	/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_COLUMN),
		};

		m_rGameLib.CreateRect(&noteDigit[4 * i], noteDigitData[i]);
		m_rGameLib.Render(&noteDigit[4 * i], m_rGameLib.GetTex(_T("Nums")));
	}
}
