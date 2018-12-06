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

		m_noteNum = (i == YV_LEFT) ? m_noteNum / 2 : m_noteNum * 2;	//!二倍ずつ増える

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

	const FLOAT NUMS_ILLUST_SCALE	= 32.0f;
	const INT NUMS_NUM_IN_ROW		= 8;
	const INT NUMS_NUM_IN_COLUMN	= 2;

	for (INT i = 0; i < DIGITS_MAX; ++i)
	{
		noteDigitData[i].m_center		= { m_WND_SIZE.m_x * (0.10f + 0.0142f * i), m_WND_SIZE.m_y * 0.5f, m_z };	//! 現物合わせ
		noteDigitData[i].m_halfScale	= { NOTE_DIGIT_HALF_SCALE, NOTE_DIGIT_HALF_SCALE, 0.0f };

		noteDigitData[i].m_texUV =																					//! 統合ファイルのテクスチャの座標
		{
			NUMS_ILLUST_SCALE * (NoteDigitNum[i] % NUMS_NUM_IN_ROW)		/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_ROW),
			NUMS_ILLUST_SCALE * (NoteDigitNum[i] / NUMS_NUM_IN_ROW)		/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_COLUMN),
			NUMS_ILLUST_SCALE * (NoteDigitNum[i] % NUMS_NUM_IN_ROW + 1)	/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_ROW),
			NUMS_ILLUST_SCALE * (NoteDigitNum[i] / NUMS_NUM_IN_ROW + 1)	/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_COLUMN),
		};

		m_rGameLib.CreateRect(	&noteDigit[4 * i], noteDigitData[i]);
		m_rGameLib.Render(		&noteDigit[4 * i], m_rGameLib.GetTex(_T("Nums")));
	}

	ObjData NoteNumButtonData[YV_MAX];

	const FLOAT NOTES_NUM_BUTTON_HALF_SCALE = m_WND_SIZE.m_x * 0.01f;
	const FLOAT BUTTON_NUM_GAP = NOTES_NUM_BUTTON_HALF_SCALE * 2.0f;

	for (INT i = 0; i < YV_MAX; ++i)
	{
		NoteNumButtonData[i].m_center	=	noteDigitData[i].m_center;
		NoteNumButtonData[i].m_center.x	+=	BUTTON_NUM_GAP * ((i == YV_LEFT) ? -1 : 1);

		NoteNumButtonData[i].m_halfScale = { NOTES_NUM_BUTTON_HALF_SCALE, NOTES_NUM_BUTTON_HALF_SCALE, 0.0f };

		if ((i == YV_LEFT	&& m_noteNum == 1) ||
			(i == YV_RIGHT	&& m_noteNum == 16)) NoteNumButtonData[i].m_aRGB = 0x00000000;							//! 分の最大最少を選んでいるときボタンを消す

		if (i == YV_LEFT) NoteNumButtonData[i].m_deg.z = 180.0f;													//! 画像を反転させるため

		m_rGameLib.CreateRect(	&m_noteNumButton[4 * i], NoteNumButtonData[i]);
		m_rGameLib.Render(		&m_noteNumButton[4 * i], m_rGameLib.GetTex(_T("BeatesNumButton")));
	}
}

VOID StageEditMeasureBPM::Update()
{
	INT measureBPMNum = static_cast<INT>(m_pMeasureBPMVec.size());
	INT measuresNumGap = m_MEASURES_NUM - measureBPMNum;

	if (measuresNumGap > 0) //! 一度に複数増えたり減ったりはしない
	{
		m_pMeasureBPMVec.push_back(new MeasureBPM);
	}

	if (measuresNumGap < 0)
	{
		delete m_pMeasureBPMVec[measureBPMNum - 1];
		m_pMeasureBPMVec.pop_back();
	}

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

	for (INT i = 0; i < m_MEASURES_NUM; ++i)
	{
		SetBPMDealWithClickedSetter(i, cursor);
	}
}

VOID StageEditMeasureBPM::SetBPMDealWithClickedSetter(INT elementNum, const CustomVertex* pCursor)
{
	CustomVertex* pBPMUppers	= m_pMeasureBPMVec[elementNum]->m_bPMUppers;
	CustomVertex* pBPMDowners	= m_pMeasureBPMVec[elementNum]->m_bPMDowners;

	INT* pDigitBpm = 0;

	for (INT di = 0; di < MeasureBPM::m_DIGITS_MAX; ++di)
	{
		pDigitBpm = &(m_pMeasureBPMVec[elementNum]->m_digitBPMs[di]);

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

INT StageEditMeasureBPM::CurrentMeasureBPM() const
{
	const INT BEAT_LENGTH = m_WND_SIZE.m_y;
	const INT BEATS_NUM_IN_MEASURE = 4;
	const INT MEASURE_LENGTH = static_cast<INT>(BEAT_LENGTH * BEATS_NUM_IN_MEASURE);

	INT measureElementNum = static_cast<INT>(m_rScrollBottom / MEASURE_LENGTH);

	INT bpm = NULL;

	const INT DIGIT_MAX = MeasureBPM::m_DIGITS_MAX;

	for (INT i = 0; i < DIGIT_MAX; ++i)
	{
		bpm +=	m_pMeasureBPMVec[measureElementNum]->m_digitBPMs[i] * 
				static_cast<INT>(pow(10, DIGIT_MAX - 1 - i));
	}

	return bpm;
}

VOID StageEditMeasureBPM::RenderMeasureBPM(INT elementNum)
{
	const FLOAT DIGITS_GAP = m_WND_SIZE.m_x * 0.01f;

	const FLOAT DIGITS_HALF_SCALE = m_WND_SIZE.m_x * 0.01f;

	ObjData* pBPMData = m_pMeasureBPMVec[elementNum]->m_bPMDatas;
	CustomVertex* pBPM = m_pMeasureBPMVec[elementNum]->m_bPM;

	INT digitBPM = 0;

	const FLOAT NUMS_ILLUST_SCALE = 32.0f;
	const INT NUMS_NUM_IN_ROW = 8;
	const INT NUMS_NUM_IN_COLUMN = 2;

	const INT BEATS_NUM_IN_MEASURE = 4;

	for (INT i = 0; i < MeasureBPM::m_DIGITS_MAX; ++i)
	{
		pBPMData[i].m_center =
		{
			m_WND_SIZE.m_x * (0.8f + 0.02f * i),												//! 現物合わせ
			(m_rScrollBottom / BEATS_NUM_IN_MEASURE) - m_WND_SIZE.m_y * (elementNum - 0.05f),	//! 節と節のBPMをウィンドウの縦の長さ分ずらす スクロールする
			m_z
		};

		pBPMData[i].m_halfScale = { DIGITS_HALF_SCALE, DIGITS_HALF_SCALE, 0.0f };

		digitBPM = m_pMeasureBPMVec[elementNum]->m_digitBPMs[i];
		pBPMData[i].m_texUV =
		{
			NUMS_ILLUST_SCALE * (digitBPM % NUMS_NUM_IN_ROW)		/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_ROW),
			NUMS_ILLUST_SCALE * (digitBPM / NUMS_NUM_IN_ROW)		/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_COLUMN),
			NUMS_ILLUST_SCALE * (digitBPM % NUMS_NUM_IN_ROW + 1)	/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_ROW),
			NUMS_ILLUST_SCALE * (digitBPM / NUMS_NUM_IN_ROW + 1)	/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_COLUMN),
		};

		m_rGameLib.CreateRect(	&pBPM[4 * i], pBPMData[i]);
		m_rGameLib.Render(		&pBPM[4 * i], m_rGameLib.GetTex(_T("Nums")));
	}
}

VOID StageEditMeasureBPM::RenderBPMSetter(INT elementNum)
{
	ObjData* pBPMData = m_pMeasureBPMVec[elementNum]->m_bPMDatas;

	const FLOAT BPM_SETTER_HALF_SCALE = m_WND_SIZE.m_x * 0.007f;

	ObjData* pBPMUpperDatas	= m_pMeasureBPMVec[elementNum]->m_bPMUpperDatas;
	CustomVertex* pBPMUppers = m_pMeasureBPMVec[elementNum]->m_bPMUppers;

	ObjData* pBPMDownerDatas = m_pMeasureBPMVec[elementNum]->m_bPMDownerDatas;
	CustomVertex* pBPMDowners = m_pMeasureBPMVec[elementNum]->m_bPMDowners;

	for (INT di = 0; di < MeasureBPM::m_DIGITS_MAX; ++di)
	{
		pBPMUpperDatas[di].m_center = pBPMData[di].m_center;
		SetBPMSetterUpDown(&pBPMUpperDatas[di], FALSE);
		pBPMUpperDatas[di].m_halfScale = { BPM_SETTER_HALF_SCALE, BPM_SETTER_HALF_SCALE, 0.0f };

		m_rGameLib.CreateRect(	&pBPMUppers[4 * di], pBPMUpperDatas[di]);
		m_rGameLib.Render(		&pBPMUppers[4 * di], m_rGameLib.GetTex(_T("BPMButton")));

		pBPMDownerDatas[di].m_center = pBPMData[di].m_center;
		SetBPMSetterUpDown(&pBPMDownerDatas[di], TRUE);
		pBPMDownerDatas[di].m_halfScale = { BPM_SETTER_HALF_SCALE, BPM_SETTER_HALF_SCALE, 0.0f };

		m_rGameLib.CreateRect(	&pBPMDowners[4 * di], pBPMDownerDatas[di]);
		m_rGameLib.Render(		&pBPMDowners[4 * di], m_rGameLib.GetTex(_T("BPMButton")));
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

	if (m_rGameLib.CollidesRects(cursor, m_measureAdder))
	{
		++m_measuresNum;

		m_isChanged = TRUE;
	}

	if (m_rGameLib.CollidesRects(cursor, m_measureDeleter) && m_measuresNum > 1)
	{
		--m_measuresNum;

		m_isChanged = TRUE;
	}

	m_pStageEditMeasureBPM->Update();
}

VOID StageEditMeasures::Render()
{
	ObjData adderData;
	adderData.m_center		= { m_WND_SIZE.m_x * 0.0427f, m_WND_SIZE.m_y * 0.1f, m_z };		//! 現物合わせ
	adderData.m_halfScale	= { m_WND_SIZE.m_x * 0.04f, m_WND_SIZE.m_y * 0.03f, 0.0f };		//! 現物合わせ

	m_rGameLib.CreateRect(	m_measureAdder, adderData);
	m_rGameLib.Render(		m_measureAdder, m_rGameLib.GetTex(_T("AddMeasure")));

	ObjData deleterData;
	deleterData.m_center	= { m_WND_SIZE.m_x * 0.0427f, m_WND_SIZE.m_y * 0.035f, m_z };	//! 現物合わせ
	deleterData.m_halfScale = adderData.m_halfScale;

	m_rGameLib.CreateRect(	m_measureDeleter, deleterData);
	m_rGameLib.Render(		m_measureDeleter, m_rGameLib.GetTex(_T("DeleteMeasure")));

	m_pStageEditMeasureBPM->Render();
}

VOID StageEditStarSelecter::Update()
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

	for (INT i = 0; i < STAR_MAX; ++i)
	{
		if (!m_rGameLib.CollidesCircles(m_starIcons[i].m_icon, cursor)) continue;

		m_selectingStarType = i;

		m_isChanged = TRUE;

		return;
	}
}

VOID StageEditStarSelecter::Render()
{
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

	ObjData* pIconData = nullptr;
	CustomVertex* pIcon = nullptr;

	for (INT si = 0; si < STAR_MAX; ++si)
	{
		pIconData = &m_starIcons[si].m_iconData;

		pIconData->m_center		= { m_WND_SIZE.m_x * 0.05f, m_WND_SIZE.m_y * (0.7f + 0.11f * si), m_z };	//! 現物合わせ
		pIconData->m_halfScale	= { SELECT_HALF_SCALE, SELECT_HALF_SCALE, 0.0f };							//! 現物合わせ

		pIconData->m_aRGB = STAR_COLORS[si];
		if (si == m_selectingStarType) pIconData->m_aRGB = SELECTING_STAR_COLORS[si];

		pIcon = m_starIcons[si].m_icon;

		m_rGameLib.CreateRect(	pIcon, *pIconData);
		m_rGameLib.Render(		pIcon, m_rGameLib.GetTex(_T("StarSelectIcon")));

		RenderStarNum(si);
	}
}

VOID StageEditStarSelecter::RenderStarNum(INT StarIconArrayNum)
{
	ObjData* pStarNumDigitData = nullptr;
	CustomVertex* pStarDigitNum = nullptr;

	const FLOAT NUM_HALF_SCALE = m_WND_SIZE.m_x * 0.009f;

	const FLOAT NUMS_GAP = -NUM_HALF_SCALE * 1.2f;

	INT digitNum = 0;

	const FLOAT NUMS_ILLUST_SCALE	= 32.0f;
	const INT NUMS_NUM_IN_ROW		= 8;
	const INT NUMS_NUM_IN_COLUMN	= 2;

	const INT DIGIT_OVER = 10;

	for (INT di = 0; di < StarIcon::m_DIGITS_NUM; ++di)
	{
		pStarNumDigitData = &m_starIcons[StarIconArrayNum].m_digitNumData[di];

		pStarNumDigitData->m_center		=	m_starIcons[StarIconArrayNum].m_iconData.m_center;
		pStarNumDigitData->m_center.x	+=	7 * NUM_HALF_SCALE + di * NUMS_GAP;

		pStarNumDigitData->m_halfScale = { NUM_HALF_SCALE, NUM_HALF_SCALE, 0.0f };

		digitNum =	m_starIcons[StarIconArrayNum].m_digitNums[di] =
					(m_starIcons[StarIconArrayNum].m_num / static_cast<INT>(pow(DIGIT_OVER, di))) % DIGIT_OVER;	//! 一桁ずつの値を抽出

		pStarNumDigitData->m_texUV =															//! 統合ファイルのテクスチャの座標
		{
			NUMS_ILLUST_SCALE * (digitNum % NUMS_NUM_IN_ROW)		/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_ROW),
			NUMS_ILLUST_SCALE * (digitNum / NUMS_NUM_IN_ROW)		/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_COLUMN),
			NUMS_ILLUST_SCALE * (digitNum % NUMS_NUM_IN_ROW + 1)	/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_ROW),
			NUMS_ILLUST_SCALE * (digitNum / NUMS_NUM_IN_ROW + 1)	/ (NUMS_ILLUST_SCALE * NUMS_NUM_IN_COLUMN),
		};

		pStarDigitNum = &m_starIcons[StarIconArrayNum].m_digitNum[4 * di];

		m_rGameLib.CreateRect(	pStarDigitNum, *pStarNumDigitData);
		m_rGameLib.Render(		pStarDigitNum, m_rGameLib.GetTex(_T("Nums")));
	}
}

VOID StageEditStars::Update()
{
	m_pStageEditNote->Update();
	m_pStageEditMeasures->Update();
	m_pStageEditStarSelecter->Update();

	if (m_pStageEditNote->IsChanged()		|| 
		m_pStageEditMeasures->IsChanged()	||
		m_pStageEditStarSelecter->IsChanged())return;

	POINT cursorPos;
	m_rGameLib.CursorPos(&cursorPos);

	ObjData cursorData;
	cursorData.m_center	= { static_cast<FLOAT>(cursorPos.x), static_cast<FLOAT>(cursorPos.y), 0.0f };
	const FLOAT CURSOR_HALF_SCALE = 0.5f;
	cursorData.m_halfScale = { CURSOR_HALF_SCALE, CURSOR_HALF_SCALE, 0.0f };

	CustomVertex cursor[4];
	m_rGameLib.CreateRect(cursor, cursorData);

	if (m_rGameLib.KeyboardIsPressed(DIK_ESCAPE))
	{
		for (StarData* pI : m_pStarDataVec)
		{
			pI->m_isActiveChangingVector = FALSE;
		}
	}

	ChangeStarDataDeg();

	if (m_rGameLib.MouseIsPressed(DIM_RIGHT)) DeleteClickedStarData(cursor);

	if (!m_rGameLib.MouseIsPressed(DIM_LEFT)) return;

	FLOAT starYPos = NULL;

	for (StarData* pI : m_pStarDataVec)
	{
		starYPos = pI->m_objData.m_center.y;
		if (starYPos < -m_STAR_HALF_SCALE ||
			starYPos > m_WND_SIZE.m_y + m_STAR_HALF_SCALE) continue;

		if (!m_rGameLib.CollidesCircles(cursor, pI->m_customVertices)) continue;

		pI->m_isActiveChangingVector = !pI->m_isActiveChangingVector;

		return;
	}

	SetStarDealWithCollidesCursorDivide(m_pStageEditNote->NoteNum(), cursorPos);
}

VOID StageEditStars::Render()
{
	m_pStageEditNote->Render();
	m_pStageEditMeasures->Render();
	m_pStageEditStarSelecter->Render();

	RenderStarData();
	RenderStarVector();
}

VOID StageEditStars::StartPreview(FLOAT ySpeed)
{
	FLOAT xSpeed = NULL;

	for (StarData* pI : m_pStarDataVec)
	{
		if (pI->m_objData.m_center.y < 0.0f) continue;

		xSpeed = tan(D3DXToRadian(pI->m_deg)) * ySpeed;

		pI->m_xMovement -= xSpeed;
	}
}

VOID StageEditStars::SetStarDealWithCollidesCursorDivide(INT noteNum, POINT cursorPoint)
{
	StarData* pStarData = nullptr;

	const INT BEATS_NUM_IN_MEASURE = 4;

	/// <summary>
	/// 節 拍 分の長さを求める
	/// </summary>
	const INT BEAT_LENGTH		= m_WND_SIZE.m_y;
	const INT MEASURE_LENGTH	= static_cast<INT>(BEAT_LENGTH * BEATS_NUM_IN_MEASURE);
	INT noteLength				= BEAT_LENGTH / m_pStageEditNote->NoteNum();

	/// <summary>
	/// 押された節が何番目なのかを調べるが、
	/// スクロールするので2パターンて発生してしまうので
	/// 押された位置をとりどちらかを判断する
	/// </summary>
	INT estimatedMeasureElementNum		= static_cast<INT>(m_rScrollBottom) / MEASURE_LENGTH;
	FLOAT estimatedMeasureBottom		= -MEASURE_LENGTH * estimatedMeasureElementNum + m_rScrollBottom + m_WND_SIZE.m_y;
	BOOL isClickedUpMeasureElementNum	= cursorPoint.y < estimatedMeasureBottom - MEASURE_LENGTH;
	INT measureElementNum				= estimatedMeasureElementNum + ((isClickedUpMeasureElementNum) ? 1 : 0);

	/// <summary>
	/// 押されたのがどの拍かを判断する
	/// そのため選択された節のボトムのウィンドウ座標を調べる
	/// </summary>
	FLOAT clickedMeasureBottom = -MEASURE_LENGTH * measureElementNum + m_rScrollBottom + m_WND_SIZE.m_y;

	for (INT i = 0; i < BEATS_NUM_IN_MEASURE; ++i)
	{
		if ((cursorPoint.y <=	clickedMeasureBottom - BEAT_LENGTH * i) &&
			(cursorPoint.y >	clickedMeasureBottom - BEAT_LENGTH * (i + 1)))
		{
			m_beatElementNum = i;
		}
	}

	const FLOAT COLLISION_TOLERANCE = 15.0f;															//! 誤差なので+-

	FLOAT FIRST_DIVIDE_POS	= clickedMeasureBottom - BEAT_LENGTH * m_beatElementNum - noteLength / 2;	//! 初めの区切りの位置は半分のサイズずれる
	FLOAT DIVIDE_POS		= NULL;

	for (INT i = 0; i < noteNum; ++i)
	{
		DIVIDE_POS = FIRST_DIVIDE_POS - i * noteLength;

		if (cursorPoint.y >=	DIVIDE_POS - COLLISION_TOLERANCE &&
			cursorPoint.y <		DIVIDE_POS + COLLISION_TOLERANCE)
		{
			pStarData = new StarData;

			pStarData->m_measureNum = measureElementNum + 1;
			pStarData->m_beatNum	= m_beatElementNum + 1;

			pStarData->m_noteNum	= noteNum;
			pStarData->m_divideNum	= i + 1;

			pStarData->m_x = static_cast<FLOAT>(cursorPoint.x);
			
			pStarData->m_starType = m_pStageEditStarSelecter->StarType();

			m_pStarDataVec.push_back(pStarData);

			m_pStageEditStarSelecter->SetStarNum(pStarData->m_starType, TRUE);
		}
	}
}

VOID StageEditStars::ChangeStarDataDeg()
{
	const FLOAT DEG_MAX = 85.0f;

	FLOAT starYPos = NULL;

	for (StarData* pI : m_pStarDataVec)
	{
		starYPos = pI->m_objData.m_center.y;
		if (starYPos < -m_STAR_HALF_SCALE ||
			starYPos > m_WND_SIZE.m_y + m_STAR_HALF_SCALE) continue;

		if (!pI->m_isActiveChangingVector) continue;

		if (m_rGameLib.MouseWheelScrollingFromPrev() > 0) pI->m_deg += 5.0f;
		if (m_rGameLib.MouseWheelScrollingFromPrev() < 0) pI->m_deg -= 5.0f;

		pI->m_deg = min(max((pI->m_deg), -DEG_MAX), DEG_MAX);
	}
}

VOID StageEditStars::DeleteClickedStarData(CustomVertex* pCusrsor)
{
	INT starMeasureNum = NULL;

	INT noteLength = NULL;

	FLOAT starYPos = NULL;

	for (INT i = 0; i < m_pStarDataVec.size(); ++i)
	{
		starMeasureNum = m_pStarDataVec[i]->m_measureNum;

		starYPos = m_pStarDataVec[i]->m_objData.m_center.y;
		if (starYPos < -m_STAR_HALF_SCALE ||
			starYPos > m_WND_SIZE.m_y + m_STAR_HALF_SCALE) continue;

		if (!m_rGameLib.CollidesCircles(pCusrsor, m_pStarDataVec[i]->m_customVertices)) continue;

		m_pStageEditStarSelecter->SetStarNum(m_pStarDataVec[i]->m_starType, FALSE);

		delete m_pStarDataVec[i];

		m_pStarDataVec.erase(m_pStarDataVec.begin() + i);	//! vectorは可能

		return;
	}
}

VOID StageEditStars::RenderStarData()
{
	const DWORD STAR_COLORS[STAR_MAX] =
	{
		0xFFFEFEFE,
		0xFF58FA82,
		0xFF2ECCFA
	};

	const INT BEATS_NUM_IN_MEASURE = 4;

	const INT BEAT_LENGTH = m_WND_SIZE.m_y;
	const INT MEASURE_LENGTH = static_cast<INT>(BEAT_LENGTH * BEATS_NUM_IN_MEASURE);
	INT noteLength = NULL;

	FLOAT starYPos = NULL;

	for (StarData* pI : m_pStarDataVec)
	{
		noteLength = BEAT_LENGTH / pI->m_noteNum;

		pI->m_objData.m_center =
		{
			pI->m_x + pI->m_xMovement,
			-(	MEASURE_LENGTH	* (pI->m_measureNum - 1) +
				BEAT_LENGTH		* (pI->m_beatNum	- 1) +
				noteLength		* (pI->m_divideNum	- 1) + noteLength / 2) +
				m_rScrollBottom + m_WND_SIZE.m_y,
			m_z + 0.01f	//! UIよりも後ろに描画
		};

		starYPos = pI->m_objData.m_center.y;
		if (starYPos < -m_STAR_HALF_SCALE ||
			starYPos > m_WND_SIZE.m_y + m_STAR_HALF_SCALE) continue;

		pI->m_objData.m_halfScale = { m_STAR_HALF_SCALE ,m_STAR_HALF_SCALE ,0.0f };

		pI->m_objData.m_aRGB = STAR_COLORS[pI->m_starType];

		m_rGameLib.CreateRect(	pI->m_customVertices, pI->m_objData);
		m_rGameLib.Render(		pI->m_customVertices, m_rGameLib.GetTex(_T("StarNote")));
	}
}

VOID StageEditStars::RenderStarVector()
{
	FLOAT starYPos = NULL;

	const FLOAT VECTOR_Y_MOVEMENT = m_STAR_HALF_SCALE * 2.3f;

	D3DXVECTOR3 rotatationRelativePos;

	ObjData vectorData;
	CustomVertex vector[4];

	for (StarData* pI : m_pStarDataVec)
	{
		starYPos = pI->m_objData.m_center.y;
		if (starYPos < -m_STAR_HALF_SCALE ||
			starYPos > m_WND_SIZE.m_y + m_STAR_HALF_SCALE) continue;

		if (!pI->m_isActiveChangingVector) continue;

		vectorData.m_center		=	pI->m_objData.m_center;
		vectorData.m_center.y	+=	VECTOR_Y_MOVEMENT;
		vectorData.m_halfScale	=	{ m_STAR_HALF_SCALE *0.5f, m_STAR_HALF_SCALE * 1.5f, 0.0f };

		m_rGameLib.CreateRect(vector, vectorData);

		rotatationRelativePos = { 0.0f, -VECTOR_Y_MOVEMENT, 0.0f };

		m_rGameLib.RotateRectZ(vector, pI->m_deg, rotatationRelativePos);

		m_rGameLib.Render(vector, m_rGameLib.GetTex(_T("Vector")));
	}
}

VOID StageEditStageEditor::Update()
{
	const INT BEATS_NUM_IN_MEASURE = 4;
	const FLOAT MEASURE_LENGTH = static_cast<FLOAT>(m_WND_SIZE.m_y) * BEATS_NUM_IN_MEASURE;

	m_measuresNum = m_pStageEditStars->GetMeasuresNum();

	FLOAT scrollMax = static_cast<FLOAT>(MEASURE_LENGTH * m_measuresNum);
	FLOAT SCROLL_SPEED = 160.0f;

	if (!m_previews && !m_pStageEditStars->CanChangeVector())
	{
		if (m_rGameLib.MouseWheelScrollingFromPrev() > 0)
		{
			m_scrollBottom = (m_scrollBottom + SCROLL_SPEED > scrollMax) ? scrollMax : m_scrollBottom + SCROLL_SPEED;
		}

		if (m_rGameLib.MouseWheelScrollingFromPrev() < 0)
		{
			m_scrollBottom = (m_scrollBottom - SCROLL_SPEED < 0.0f) ? 0.0f : m_scrollBottom - SCROLL_SPEED;
		}
	}

	POINT cursorPos;
	m_rGameLib.CursorPos(&cursorPos);

	ObjData cursorData;
	cursorData.m_center = { static_cast<FLOAT>(cursorPos.x), static_cast<FLOAT>(cursorPos.y), 0.0f };
	const FLOAT CURSOR_HALF_SCALE = 0.5f;
	cursorData.m_halfScale = { CURSOR_HALF_SCALE, CURSOR_HALF_SCALE, 0.0f };

	CustomVertex cursor[4];
	m_rGameLib.CreateRect(cursor, cursorData);

	if (m_rGameLib.MouseIsPressed(DIM_LEFT))
	{
		if (m_rGameLib.CollidesRects(cursor, m_preview) && m_measuresNum)
		{
			if (m_previews) m_pStageEditStars->EndPreview();

			m_scrollBottom = NULL;

			m_previews = !m_previews;

			return;
		}

		if (m_rGameLib.CollidesRects(cursor, m_writeFileButton))
		{
			StageFile& rStageFile = StageFile::GetInstance();

			rStageFile.WriteStageData(
				m_pStageEditStars->GetMeasuresNum(),
				m_pStageEditStars->GetPMeasureBPMVec(),
				m_pStageEditStars->GetStarIcons(),
				m_pStageEditStars->GetPStarDataVec());

			return;
		}
	}

	if (m_previews)
	{
		INT bpm = m_pStageEditStars->CurrentMeasureBPM();

		const INT FRAME_NUM_IN_MINUTE = 3600;

		FLOAT scrollSpeed = static_cast<FLOAT>(m_WND_SIZE.m_y * bpm) / FRAME_NUM_IN_MINUTE;

		m_scrollBottom +=	scrollSpeed;
		m_scrollBottom =	min(max(m_scrollBottom, 0.0f), scrollMax - 1);

		m_pStageEditStars->StartPreview(scrollSpeed);

		return;
	}

	m_pStageEditStars->Update();
}

VOID StageEditStageEditor::Render()
{
	m_pStageEditStars->Render();

	ObjData previewData;

	const FLOAT HALF_SCALE = m_WND_SIZE.m_x * 0.02f;

	previewData.m_center	= { m_WND_SIZE.m_x * 0.941f, m_WND_SIZE.m_y * 0.9f, m_z };	//! 現物合わせ
	previewData.m_halfScale = { HALF_SCALE, HALF_SCALE, 0.0f };

	previewData.m_texUV =
	{
		0.0f,
		0.0f,
		0.5f,
		1.0f
	};

	if (m_previews)
	{
		previewData.m_texUV =
		{
			0.5f,
			0.0f,
			1.0f,
			1.0f
		};
	}

	m_rGameLib.CreateRect(	m_preview, previewData);
	m_rGameLib.Render(		m_preview, m_rGameLib.GetTex(_T("Preview")));

	ObjData writeFileButtonData;
	writeFileButtonData.m_center	= { m_WND_SIZE.m_x * 0.96f, m_WND_SIZE.m_y * 0.04f, m_z };
	writeFileButtonData.m_halfScale = { m_WND_SIZE.m_x * 0.04f, m_WND_SIZE.m_y * 0.04f, 0.0f };

	m_rGameLib.CreateRect(	m_writeFileButton, writeFileButtonData);
	m_rGameLib.Render(		m_writeFileButton, m_rGameLib.GetTex(_T("WriteFile")));
}
