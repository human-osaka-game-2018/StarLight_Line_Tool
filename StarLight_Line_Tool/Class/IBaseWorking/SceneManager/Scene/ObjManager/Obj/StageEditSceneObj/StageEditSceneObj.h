/// <filename>
/// StageEditSceneObj.h
/// </filename>
/// </summary>
/// ステージ作成シーンで扱うオブジェクトのクラスのヘッダ
/// </summary>
/// <author>
/// Harutaka-Tsujino
/// </author>

#ifndef STAGE_EDIT_SCENE_OBJ_H
#define STAGE_EDIT_SCENE_OBJ_H

#include <windows.h>
#include <tchar.h>
#include <math.h>
#include <crtdbg.h>

#include <vector>
#include <cstdio>
#include <cstdlib>

#include <d3dx9.h>

#include "../Obj.h"
#include "Enum\STAR_TYPE.h"
#include "Enum\Y_VEC.h"

#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

/// <summary>
/// ステージ作成時の背景 分によって画像を切り替え スクロールする 
/// </summary>
class StageEditBack :public Obj
{
public:
	StageEditBack(const INT& rBeatsNum, const FLOAT& rScrollBottom) :Obj(OT_BACK, 1.0f), m_rNoteNum(rBeatsNum), m_rScrollBottom(rScrollBottom)
	{
		Init();
	}

	~StageEditBack()
	{
		m_rGameLib.ReleaseTex();
	}

	inline VOID Init() const
	{
		m_rGameLib.CreateTex(_T("Back"), _T("2DTextures/StageEdit/StageEditBack.png"));
		m_rGameLib.CreateTex(_T("16Back"), _T("2DTextures/StageEdit/16BeatsBack.png"));
		m_rGameLib.CreateTex(_T("8Back"), _T("2DTextures/StageEdit/8BeatsBack.png"));
		m_rGameLib.CreateTex(_T("4Back"), _T("2DTextures/StageEdit/4BeatsBack.png"));
		m_rGameLib.CreateTex(_T("2Back"), _T("2DTextures/StageEdit/2BeatsBack.png"));
		m_rGameLib.CreateTex(_T("1Back"), _T("2DTextures/StageEdit/1BeatBack.png"));
	}

	inline VOID Update() {};

	VOID Render();

private:
	const INT& m_rNoteNum;

	const FLOAT& m_rScrollBottom;
};

/// <summary>
/// 配置する星の分を設定する 背景クラスを保持する
/// </summary>
class StageEditNote :public Obj
{
public:
	StageEditNote(const FLOAT& scrollBottom) :Obj(OT_TRANSPARENCY, 0.9f), m_rScrollBottom(scrollBottom)
	{
		Init();
	}

	~StageEditNote()
	{
		delete m_pStageEditBack;
		m_rGameLib.ReleaseTex();
	}

	inline VOID Init()
	{
		m_pStageEditBack = new StageEditBack(m_noteNum, m_rScrollBottom);

		m_rGameLib.CreateTex(_T("BeatesNumButton"), _T("2DTextures/StageEdit/BeatsNumButton.png"));
		m_rGameLib.CreateTex(_T("Nums"), _T("2DTextures/StageEdit/Nums.png"));
	}

	VOID Update();

	VOID Render();

	inline const INT& NoteNum() const
	{
		return m_noteNum;
	}

	inline const BOOL IsChanged() const
	{
		return m_isChanged;
	}

private:
	const FLOAT& m_rScrollBottom;

	BOOL m_isChanged = FALSE;

	INT m_noteNum = 16;

	CustomVertex m_noteNumButton[4 * YV_MAX];

	StageEditBack* m_pStageEditBack = nullptr;
};

/// <summary>
/// ステージの節 節単位でBPMを変えられる
/// </summary>
class StageEditMeasures :public Obj
{
public:
	StageEditMeasures(const FLOAT& rScrollBottom) :Obj(OT_TRANSPARENCY, 0.9f), m_rScrollBottom(rScrollBottom)
	{
		Init();
	}

	~StageEditMeasures()
	{
		for (Measure* pI : m_pMeasureVec)
		{
			delete pI;
		}

		m_rGameLib.ReleaseTex();
	}

	inline VOID Init()
	{
		m_pMeasureVec.push_back(new Measure);

		m_rGameLib.CreateTex(_T("AddMeasure"), _T("2DTextures/StageEdit/AddMeasureButton.png"));
		m_rGameLib.CreateTex(_T("DeleteMeasure"), _T("2DTextures/StageEdit/DeleteMeasureButton.png"));
		m_rGameLib.CreateTex(_T("BPMButton"), _T("2DTextures/StageEdit/BPMNumButton.png"));
	}

	VOID Update();

	VOID Render();

	inline INT MeasuresNum() const
	{
		return m_measuresNum;
	}

	inline const BOOL IsChanged() const
	{
		return m_isChanged;
	}

private:
	struct Measure
	{
	public:
		static const INT m_DIGITS_MAX = 3;				//! BPMの桁数

		INT m_digitBPMs[m_DIGITS_MAX] = { 1, 5, 0 };	//! デフォルトのBPM

		ObjData m_bPMDatas[m_DIGITS_MAX];
		CustomVertex m_bPM[4 * m_DIGITS_MAX];

		ObjData m_bPMUpperDatas[m_DIGITS_MAX];
		CustomVertex m_bPMUppers[4 * m_DIGITS_MAX];

		ObjData m_bPMDownerDatas[m_DIGITS_MAX];
		CustomVertex m_bPMDowners[4 * m_DIGITS_MAX];
	};

	VOID SetBPMDealWithClickedSetter(INT elementNum, const CustomVertex* pCursor);

	VOID RenderMeasure(INT elementNum);

	VOID RenderBPMSetter(INT elementNum);

	inline VOID SetBPMSetterUpDown(ObjData* pBPMSetterData, BOOL isDown) const
	{
		const FLOAT BPM_BUTTON_GAP = m_WND_SIZE.m_x * 0.02f;

		if (isDown)
		{
			pBPMSetterData->m_center.y += BPM_BUTTON_GAP;

			pBPMSetterData->m_deg.z = 180.0f;	//! 下ボタンは画像を反転させるため

			return;
		}

		pBPMSetterData->m_center.y += -BPM_BUTTON_GAP;
	}

	const FLOAT& m_rScrollBottom;

	BOOL m_isChanged = FALSE;

	INT m_measuresNum = 1;
	std::vector<Measure*> m_pMeasureVec;

	CustomVertex m_measureAdder[4];
	CustomVertex m_measureDeleter[4];
};

/// <summary>
/// ステージ配置する星
/// </summary>
class StageEditStars :public Obj
{
public:
	StageEditStars(const FLOAT& rScrollBottom) :Obj(OT_TRANSPARENCY, 0.9f), m_rScrollBottom(rScrollBottom)
	{
		Init();
	}

	~StageEditStars()
	{
		for (StarData* pI : m_pStarDataVec)
		{
			delete pI;
		}
		
		delete m_pStageEditMeasures;
		delete m_pStageEditNote;
		m_rGameLib.ReleaseTex();
	}

	inline VOID Init()
	{
		m_pStageEditNote = new StageEditNote(m_rScrollBottom);

		m_pStageEditMeasures = new StageEditMeasures(m_rScrollBottom);

		m_rGameLib.CreateTex(_T("StarSelectIcon"), _T("2DTextures/StageEdit/StarSelectIcon.png"));
		m_rGameLib.CreateTex(_T("StarNote"), _T("2DTextures/StageEdit/StarNote.png"));
	}

	VOID Update();

	VOID Render();

	INT MeasuresNum()
	{
		return m_pStageEditMeasures->MeasuresNum();
	}

private:
	struct StarData
	{
		INT m_starType = STAR_SCORE;

		INT m_measureNum = 0;
		INT m_beatNum = 0;
		INT m_noteNum = 0;
		INT m_divideNum = 0;

		FLOAT m_x = 0.0f;

		ObjData m_objData;
		CustomVertex m_customVertices[4];
	};

	struct StarNum
	{
		static const INT m_DIGITS_NUM = 4;
		INT m_num = 0;
		INT m_digitNums[m_DIGITS_NUM];

		ObjData m_objData[m_DIGITS_NUM];
		CustomVertex m_customVertices[4 * m_DIGITS_NUM];
	};

	VOID SetStarDealWithCollidesCursorDivide(INT noteNum, POINT cursorPoint);

	const FLOAT& m_rScrollBottom;

	INT m_selectingStarType = STAR_DAMAGE;
	CustomVertex m_select[4 * STAR_MAX];

	StarNum m_starNums[STAR_MAX];

	std::vector<StarData*> m_pStarDataVec;

	INT m_measureElementNum = NULL;
	INT m_beatElementNum	= NULL;

	StageEditNote* m_pStageEditNote = nullptr;
	StageEditMeasures* m_pStageEditMeasures = nullptr;
};

/// <summary>
/// スクロール量の管理及びステージ編集のマネージャ
/// </summary>
class StageEditStageEditor :public Obj
{
public:
	StageEditStageEditor() :Obj(OT_BACK, 1.0f)
	{
		Init();
	}

	~StageEditStageEditor()
	{
		delete m_pStageEditStars;
		m_rGameLib.ReleaseTex();
	}

	inline VOID Init()
	{
		m_pStageEditStars	= new StageEditStars(m_scrollBottom);
	}

	inline VOID Update()
	{
		const INT BEATS_NUM_IN_MEASURE = 4;

		FLOAT scrollMax		= static_cast<FLOAT>(m_WND_SIZE.m_y * BEATS_NUM_IN_MEASURE * m_pStageEditStars->MeasuresNum());
		FLOAT SCROLL_SPEED	= 20.0f;

		if (m_rGameLib.KeyboardIsHeld(DIK_UP))
		{
			m_scrollBottom = (m_scrollBottom + SCROLL_SPEED > scrollMax) ? scrollMax : m_scrollBottom + SCROLL_SPEED;
		}

		if (m_rGameLib.KeyboardIsHeld(DIK_DOWN))
		{
			m_scrollBottom = (m_scrollBottom - SCROLL_SPEED < 0.0f) ? 0.0f : m_scrollBottom - SCROLL_SPEED;
		}

		m_pStageEditStars->Update();
	}

	inline VOID Render()
	{
		m_pStageEditStars->Render();
	}

private:
	StageEditStars* m_pStageEditStars		= nullptr;

	FLOAT m_scrollBottom = 0.0f;
	INT m_noteNum = 0;
	INT m_measureNum = 0;
};

#endif //! STAGE_EDIT_SCENE_OBJ_H
