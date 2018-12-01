/// <filename>
/// StageEditSceneObj.cpp
/// </filename>
/// <summary>
/// ステージ作成シーンで扱うオブジェクトのクラスのソース
/// </summary>
/// <author>
/// Harutaka-Tsujino
/// </author>

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
	data.m_center		= { m_WND_SIZE.m_x * 0.5f, m_WND_SIZE.m_y * 0.5f, m_z };	//! 現物合わせ
	data.m_halfScale	= { m_WND_SIZE.m_x * 0.5f, m_WND_SIZE.m_y * 0.5f, 0.0f };	//! 現物合わせ

	CustomVertex back[4];
	m_rGameLib.CreateRect(back, data);
	m_rGameLib.Render(back, m_rGameLib.GetTex(_T("Back")));

	data.m_texUV =																	//! スクロールさせる
	{
		0.0f,
		(m_rScrollBottom + m_WND_SIZE.m_y) / m_WND_SIZE.m_y,
		1.0f,
		m_rScrollBottom / m_WND_SIZE.m_y,
	};

	CustomVertex noteBack[4];
	m_rGameLib.CreateRect(noteBack, data);

	LPDIRECT3DTEXTURE9 backTex;

	switch (m_rNoteNum)
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

	m_rGameLib.Render(noteBack, backTex);
}

VOID StageEditNote::Update()
{
	m_isChanged = FALSE;

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
		if (!m_rGameLib.CollidesRects(&m_noteNumButton[4 * i], cursor)) continue;

		if (m_noteNumButton[4 * i].m_aRGB == 0x00000000) continue;	//!ボタンが消されているときは押せない

		m_noteNum = (i == YV_LEFT) ? m_noteNum / 2 : m_noteNum * 2;	//!二倍ずつ増え 

		m_isChanged = TRUE;

		return;
	}
}

VOID StageEditNote::Render()
{
	m_pStageEditBack->Render();

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
		noteDigitData[i].m_center		= { m_WND_SIZE.m_x * (0.10f + 0.0142f * i), m_WND_SIZE.m_y * 0.5f, m_z };	//! 現物合わせ
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

	ObjData NoteNumButtonData[YV_MAX];

	const FLOAT NOTES_NUM_BUTTON_HALF_SCALE = m_WND_SIZE.m_x * 0.01f;												//! 現物合わせ
	const FLOAT BUTTON_NUM_GAP = NOTES_NUM_BUTTON_HALF_SCALE * 2.0f;

	for (INT i = 0; i < YV_MAX; ++i)
	{
		NoteNumButtonData[i].m_center = noteDigitData[i].m_center;													//! 現物合わせ
		NoteNumButtonData[i].m_center.x += BUTTON_NUM_GAP * ((i == YV_LEFT) ? -1 : 1);

		NoteNumButtonData[i].m_halfScale = { NOTES_NUM_BUTTON_HALF_SCALE, NOTES_NUM_BUTTON_HALF_SCALE, 0.0f };

		if ((i == YV_LEFT && m_noteNum == 1) ||
			(i == YV_RIGHT && m_noteNum == 16)) NoteNumButtonData[i].m_aRGB = 0x00000000;							//! 分の最大最少を選んでいるときボタンを消す

		if (i == YV_LEFT) NoteNumButtonData[i].m_deg.z = 180.0f;													//! 画像を反転させるため

		m_rGameLib.CreateRect(&m_noteNumButton[4 * i], NoteNumButtonData[i]);
		m_rGameLib.Render(&m_noteNumButton[4 * i], m_rGameLib.GetTex(_T("BeatesNumButton")));
	}
}

VOID StageEditMeasures::Update()
{
	m_isChanged = FALSE;

	if (!m_rGameLib.MouseIsPressed(DIM_LEFT)) return;

	POINT cursorPos;
	m_rGameLib.CursorPos(&cursorPos);

	ObjData cursorData;
	cursorData.m_center = { static_cast<FLOAT>(cursorPos.x), static_cast<FLOAT>(cursorPos.y), 0.0f };
	const FLOAT CURSOR_HALF_SCALE = 0.5f;
	cursorData.m_halfScale = { CURSOR_HALF_SCALE, CURSOR_HALF_SCALE, 0.0f };

	CustomVertex cursor[4];
	m_rGameLib.CreateRect(cursor, cursorData);

	for (INT i = 0; i < m_measuresNum; ++i)
	{
		SetBPMDealWithClickedSetter(i, cursor);
	}

	if (m_rGameLib.CollidesRects(cursor, m_measureAdder))
	{
		++m_measuresNum;
		m_pMeasureVec.push_back(new Measure);

		m_isChanged = TRUE;

		return;
	}

	if (m_measuresNum <= 1) return;

	if (m_rGameLib.CollidesRects(cursor, m_measureDeleter))
	{
		delete m_pMeasureVec[m_measuresNum - 1];
		m_pMeasureVec.pop_back();

		--m_measuresNum;

		m_isChanged = TRUE;

		return;
	}
}

VOID StageEditMeasures::Render()
{
	for (INT i = 0; i < m_measuresNum; ++i)
	{
		RenderMeasure(i);
		RenderBPMSetter(i);
	}

	ObjData adderData;
	adderData.m_center		= { m_WND_SIZE.m_x * 0.0427f, m_WND_SIZE.m_y * 0.1f, m_z };		//! 現物合わせ
	adderData.m_halfScale	= { m_WND_SIZE.m_x * 0.04f, m_WND_SIZE.m_y * 0.03f, 0.0f };		//! 現物合わせ

	m_rGameLib.CreateRect(m_measureAdder, adderData);
	m_rGameLib.Render(m_measureAdder, m_rGameLib.GetTex(_T("AddMeasure")));

	ObjData deleterData;
	deleterData.m_center	= { m_WND_SIZE.m_x * 0.0427f, m_WND_SIZE.m_y * 0.035f, m_z };	//! 現物合わせ
	deleterData.m_halfScale = adderData.m_halfScale;

	m_rGameLib.CreateRect(m_measureDeleter, deleterData);
	m_rGameLib.Render(m_measureDeleter, m_rGameLib.GetTex(_T("DeleteMeasure")));
}

VOID StageEditMeasures::RenderMeasure(INT elementNum)
{
	const FLOAT DIGITS_GAP = m_WND_SIZE.m_x * 0.01f;

	const FLOAT DIGITS_HALF_SCALE = m_WND_SIZE.m_x * 0.01f;

	ObjData* pBPMData = m_pMeasureVec[elementNum]->m_bPMDatas;
	CustomVertex* pBPM = m_pMeasureVec[elementNum]->m_bPM;

	INT digitBPM = 0;

	const FLOAT NUMS_ILLUST_SCALE	= 32.0f;
	const INT NUMS_NUM_IN_ROW		= 8;
	const INT NUMS_NUM_IN_COLUMN	= 2;

	const INT BEATS_NUM_IN_MEASURE = 4;

	for (INT i = 0; i < Measure::m_DIGITS_MAX; ++i)
	{
		pBPMData[i].m_center =
		{
			m_WND_SIZE.m_x * (0.8f + 0.02f * i),												//! 現物合わせ
			(m_rScrollBottom / BEATS_NUM_IN_MEASURE) - m_WND_SIZE.m_y * (elementNum - 0.05f),	//! 節と節のBPMをウィンドウの縦の長さ分ずらす スクロールする
			m_z
		};

		pBPMData[i].m_halfScale = { DIGITS_HALF_SCALE, DIGITS_HALF_SCALE, 0.0f };

		digitBPM = m_pMeasureVec[elementNum]->m_digitBPMs[i];
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

VOID StageEditMeasures::RenderBPMSetter(INT elementNum)
{
	ObjData* pBPMData = m_pMeasureVec[elementNum]->m_bPMDatas;

	const FLOAT BPM_SETTER_HALF_SCALE = m_WND_SIZE.m_x * 0.007f;

	ObjData* pBPMUpperDatas = m_pMeasureVec[elementNum]->m_bPMUpperDatas;
	CustomVertex* pBPMUppers = m_pMeasureVec[elementNum]->m_bPMUppers;

	ObjData* pBPMDownerDatas = m_pMeasureVec[elementNum]->m_bPMDownerDatas;
	CustomVertex* pBPMDowners = m_pMeasureVec[elementNum]->m_bPMDowners;

	for (INT di = 0; di < Measure::m_DIGITS_MAX; ++di)
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

VOID StageEditMeasures::SetBPMDealWithClickedSetter(INT elementNum, const CustomVertex* pCursor)
{
	CustomVertex* pBPMUppers = m_pMeasureVec[elementNum]->m_bPMUppers;

	CustomVertex* pBPMDowners = m_pMeasureVec[elementNum]->m_bPMDowners;

	INT* pDigitBpm = 0;

	for (INT di = 0; di < Measure::m_DIGITS_MAX; ++di)
	{
		pDigitBpm = &(m_pMeasureVec[elementNum]->m_digitBPMs[di]);

		if (m_rGameLib.CollidesCircles(pCursor, &pBPMUppers[4 * di]))
		{
			*pDigitBpm = (*pDigitBpm + 1) % 10;						//! 1桁の数字なので9の次は0

			m_isChanged = TRUE;

			return;
		}

		if (m_rGameLib.CollidesCircles(pCursor, &pBPMDowners[4 * di]))
		{
			*pDigitBpm = (*pDigitBpm == 0) ? 9 : *pDigitBpm - 1;	//! 1桁の数字なので0の次は9

			m_isChanged = TRUE;

			return;
		}
	}
}

VOID StageEditStars::Update()
{
	m_pStageEditNote->Update();
	m_pStageEditMeasures->Update();

	if (m_pStageEditNote->IsChanged() || m_pStageEditMeasures->IsChanged())return;

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

		return;
	}

	SetStarDealWithCollidesCursorDivide(m_pStageEditNote->NoteNum(), cursorPos);
}

VOID StageEditStars::Render()
{
	m_rGameLib.SetCameraTransform();

	m_pStageEditNote->Render();
	m_pStageEditMeasures->Render();

	ObjData selectData[STAR_MAX];

	const DWORD STAR_COLORS[STAR_MAX] =																				//! 白 青 緑
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

	ObjData* pStarNumDigitData = nullptr;

	CustomVertex* pStarDigitNum = nullptr;

	const FLOAT NUM_HALF_SCALE = m_WND_SIZE.m_x * 0.009f;

	const FLOAT NUMS_GAP = -NUM_HALF_SCALE * 1.2f;

	INT digitNum = 0;

	const FLOAT NUMS_ILLUST_SCALE = 32.0f;
	const INT NUMS_NUM_IN_ROW = 8;
	const INT NUMS_NUM_IN_COLUMN = 2;

	for (INT si = 0; si < STAR_MAX; ++si)
	{
		selectData[si].m_center		= { m_WND_SIZE.m_x * 0.05f, m_WND_SIZE.m_y * (0.7f + 0.11f * si), m_z };			//! 現物合わせ
		selectData[si].m_halfScale	= { SELECT_HALF_SCALE, SELECT_HALF_SCALE, 0.0f };									//! 現物合わせ

		selectData[si].m_aRGB = STAR_COLORS[si];
		if (si == m_selectingStarType) selectData[si].m_aRGB = SELECTING_STAR_COLORS[si];

		m_rGameLib.CreateRect(&m_select[4 * si], selectData[si]);
		m_rGameLib.Render(&m_select[4 * si], m_rGameLib.GetTex(_T("StarSelectIcon")));

		for (INT di = 0; di < StarNum::m_DIGITS_NUM; ++di)
		{
			pStarNumDigitData = &m_starNums[si].m_objData[di];

			pStarNumDigitData->m_center = selectData[si].m_center;
			pStarNumDigitData->m_center.x += 3 * SELECT_HALF_SCALE + di * NUMS_GAP;

			pStarNumDigitData->m_halfScale = { NUM_HALF_SCALE, NUM_HALF_SCALE, 0.0f };

			digitNum = m_starNums[si].m_digitNums[di] = (m_starNums[si].m_num / static_cast<INT>(pow(10, di))) % 10;	//! 一桁ずつの値を抽出

			pStarNumDigitData->m_texUV =
			{
				NUMS_ILLUST_SCALE * (digitNum % NUMS_NUM_IN_ROW)		/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_ROW),
				NUMS_ILLUST_SCALE * (digitNum / NUMS_NUM_IN_ROW)		/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_COLUMN),
				NUMS_ILLUST_SCALE * (digitNum % NUMS_NUM_IN_ROW + 1)	/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_ROW),
				NUMS_ILLUST_SCALE * (digitNum / NUMS_NUM_IN_ROW + 1)	/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_COLUMN),
			};

			pStarDigitNum = &m_starNums[si].m_customVertices[4 * di];

			m_rGameLib.CreateRect(pStarDigitNum, *pStarNumDigitData);
			m_rGameLib.Render(pStarDigitNum, m_rGameLib.GetTex(_T("Nums")));
		}
	}

	const FLOAT STAR_HALF_SCALE = m_WND_SIZE.m_x *0.025f;

	const INT BEATS_NUM_IN_MEASURE = 4;

	const INT BEAT_LENGTH = m_WND_SIZE.m_y;
	const INT MEASURE_LENGTH = static_cast<INT>(BEAT_LENGTH * BEATS_NUM_IN_MEASURE);
	INT noteLength = NULL;

	for (StarData* pSi : m_pStarDataVec)
	{
		if (pSi->m_measureNum < m_measureElementNum ||
			pSi->m_measureNum > m_measureElementNum + 2) continue;

		noteLength = BEAT_LENGTH / pSi->m_noteNum;

		pSi->m_objData.m_center =
		{
			pSi->m_x,
			-(	MEASURE_LENGTH	* (pSi->m_measureNum	- 1)					+
				BEAT_LENGTH		* (pSi->m_beatNum		- 1)					+
				noteLength		* (pSi->m_divideNum		- 1) + noteLength / 2)	+
				m_rScrollBottom	+ m_WND_SIZE.m_y,
			m_z + 0.01f	//! UIよりも後ろに描画
		};

		pSi->m_objData.m_halfScale = { STAR_HALF_SCALE ,STAR_HALF_SCALE ,0.0f };
		
		pSi->m_objData.m_aRGB = SELECTING_STAR_COLORS[pSi->m_starType];

		m_rGameLib.CreateRect(pSi->m_customVertices, pSi->m_objData);
		m_rGameLib.Render(pSi->m_customVertices, m_rGameLib.GetTex(_T("StarNote")));
	}
}

VOID StageEditStars::SetStarDealWithCollidesCursorDivide(INT noteNum, POINT cursorPoint)
{
	StarData* pStarData = nullptr;

	const INT BEATS_NUM_IN_MEASURE = 4;

	/// <summary>
	/// 節 拍 分の長さを求める
	/// </summary>
	const INT BEAT_LENGTH = m_WND_SIZE.m_y;
	const INT MEASURE_LENGTH = static_cast<INT>(BEAT_LENGTH * BEATS_NUM_IN_MEASURE);
	INT noteLength = BEAT_LENGTH / m_pStageEditNote->NoteNum();

	/// <summary>
	/// 押された節が何番目なのかを調べるが、
	/// スクロールするので2パターンて発生してしまうので
	/// 押された位置をとりどちらかを判断する
	/// </summary>
	INT estimatedMeasureElementNum = static_cast<INT>(m_rScrollBottom) / MEASURE_LENGTH;
	FLOAT estimatedMeasureBottom = -MEASURE_LENGTH * estimatedMeasureElementNum + m_rScrollBottom + m_WND_SIZE.m_y;
	BOOL isClickedUpMeasureElementNum = cursorPoint.y < estimatedMeasureBottom - MEASURE_LENGTH;
	m_measureElementNum = estimatedMeasureElementNum + ((isClickedUpMeasureElementNum) ? 1 : 0);

	/// <summary>
	/// 押されたのがどの拍かを判断する
	/// そのため選択された節のボトムのウィンドウ座標を調べる
	/// </summary>
	FLOAT clickedMeasureBottom = -MEASURE_LENGTH * m_measureElementNum + m_rScrollBottom + m_WND_SIZE.m_y;

	for (INT i = 0; i < BEATS_NUM_IN_MEASURE; ++i)
	{
		if ((cursorPoint.y <=	clickedMeasureBottom - BEAT_LENGTH * i) &&
			(cursorPoint.y >	clickedMeasureBottom - BEAT_LENGTH * (i + 1)))
		{
			m_beatElementNum = i;
		}
	}

	const FLOAT COLLISION_TOLERANCE = 15.0f;															//! 誤差なので+-

	FLOAT FIRST_DIVIDE_POS = clickedMeasureBottom - BEAT_LENGTH * m_beatElementNum - noteLength / 2;	//! 初めの区切りの位置は半分のサイズずれる
	FLOAT DIVIDE_POS = NULL;

	for (INT i = 0; i < noteNum; ++i)
	{
		DIVIDE_POS = FIRST_DIVIDE_POS - i * noteLength;

		if (cursorPoint.y >=	DIVIDE_POS - COLLISION_TOLERANCE &&
			cursorPoint.y <		DIVIDE_POS + COLLISION_TOLERANCE)
		{
			pStarData = new StarData;

			pStarData->m_measureNum = m_measureElementNum + 1;
			pStarData->m_beatNum	= m_beatElementNum + 1;

			pStarData->m_noteNum	= noteNum;
			pStarData->m_divideNum	= i + 1;

			pStarData->m_x = static_cast<FLOAT>(cursorPoint.x);
			
			pStarData->m_starType = m_selectingStarType;

			m_pStarDataVec.push_back(pStarData);

			++m_starNums[m_selectingStarType].m_num;
		}
	}
}
