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
	StageEditBack(const INT& rBeatsNum, const FLOAT& rScrollBottom) :Obj(OT_BACK, 1.0f), m_rBeatsNum(rBeatsNum), m_rScrollBottom(rScrollBottom)
	{
		Init();
	}

	~StageEditBack()
	{
		m_rGameLib.ReleaseTex();
	}

	inline VOID Init() const
	{
		m_rGameLib.CreateTex(_T("16Back"), _T("2DTextures/StageEdit/16BeatsBack.png"));
		m_rGameLib.CreateTex(_T("8Back"), _T("2DTextures/StageEdit/8BeatsBack.png"));
		m_rGameLib.CreateTex(_T("4Back"), _T("2DTextures/StageEdit/4BeatsBack.png"));
		m_rGameLib.CreateTex(_T("2Back"), _T("2DTextures/StageEdit/2BeatsBack.png"));
		m_rGameLib.CreateTex(_T("1Back"), _T("2DTextures/StageEdit/1BeatBack.png"));
	}

	inline VOID Update() {};

	VOID Render();

private:
	const INT& m_rBeatsNum;

	const FLOAT& m_rScrollBottom;
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
		m_rGameLib.ReleaseTex();
	}

	inline VOID Init()
	{
		ZeroMemory(m_starNums, sizeof(INT) * STAR_MAX);

		m_rGameLib.CreateTex(_T("StarSelectIcon"), _T("2DTextures/StageEdit/StarSelectIcon.png"));
	}

	VOID Update();

	VOID Render();

private:
	struct StarData
	{
		INT starType = STAR_SCORE;
		ObjData m_objData;
		CustomVertex m_customVertices[4];
	};

	const FLOAT& m_rScrollBottom;

	INT m_selectingStarType = STAR_DAMAGE;
	CustomVertex m_select[4 * STAR_MAX];

	INT m_starNums[STAR_MAX];	//! Init()で初期化

	std::vector<StarData*> m_pStarDataVec;
};

/// <summary>
/// ステージの節 節単位でBPMを変えられる
/// </summary>
class StageEditMeusurs :public Obj
{
public:
	StageEditMeusurs(FLOAT& rScrollBottom) :Obj(OT_TRANSPARENCY, 0.9f), m_rScrollBottom(rScrollBottom)
	{
		Init();
	}

	~StageEditMeusurs()
	{
		for (Meusur* pI : m_pMeusurVec)
		{
			delete pI;
		}

		m_rGameLib.ReleaseTex();
	}

	inline VOID Init()
	{
		m_pMeusurVec.push_back(new Meusur);

		m_rGameLib.CreateTex(_T("AddMeusur"), _T("2DTextures/StageEdit/AddMeusurButton.png"));
		m_rGameLib.CreateTex(_T("DeleteMeusur"), _T("2DTextures/StageEdit/DeleteMeusurButton.png"));
		m_rGameLib.CreateTex(_T("BPMButton"), _T("2DTextures/StageEdit/BPMNumButton.png"));
	}

	VOID Update();

	VOID Render();

	inline INT MeusursNum() const
	{
		return m_meusursNum;
	}

private:
	struct Meusur
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

	VOID RenderMeusur(INT elementNum);

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

	INT m_meusursNum = 1;
	std::vector<Meusur*> m_pMeusurVec;

	CustomVertex m_meusurAdder[4];
	CustomVertex m_meusurDeleter[4];
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

private:
	const FLOAT& m_rScrollBottom;

	INT m_noteNum = 16;
	CustomVertex m_noteNumButton[4 * YV_MAX];

	StageEditBack* m_pStageEditBack = nullptr;
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
		delete m_pStageEditMeusurs;
		delete m_pStageEditBeates;
		m_rGameLib.ReleaseTex();
	}

	inline VOID Init()
	{
		m_pStageEditBeates	= new StageEditNote(m_scrollBottom);
		m_pStageEditMeusurs = new StageEditMeusurs(m_scrollBottom);
		m_pStageEditStars	= new StageEditStars(m_scrollBottom);
	}

	inline VOID Update()
	{
		const INT BEATS_NUM_IN_MEUSUR = 4;

		FLOAT scrollMax		= static_cast<FLOAT>(m_WND_SIZE.m_y * BEATS_NUM_IN_MEUSUR * m_pStageEditMeusurs->MeusursNum());
		FLOAT SCROLL_SPEED	= 20.0f;

		if (m_rGameLib.KeyboardIsHeld(DIK_UP))
		{
			m_scrollBottom = (m_scrollBottom + SCROLL_SPEED > scrollMax) ? scrollMax : m_scrollBottom + SCROLL_SPEED;
		}

		if (m_rGameLib.KeyboardIsHeld(DIK_DOWN))
		{
			m_scrollBottom = (m_scrollBottom - SCROLL_SPEED < 0.0f) ? 0.0f : m_scrollBottom - SCROLL_SPEED;
		}

		m_pStageEditBeates->Update();
		m_pStageEditMeusurs->Update();
		m_pStageEditStars->Update();
	}

	inline VOID Render()
	{
		m_pStageEditBeates->Render();
		m_pStageEditMeusurs->Render();
		m_pStageEditStars->Render();
	}

private:
	StageEditStars* m_pStageEditStars		= nullptr;
	StageEditNote* m_pStageEditBeates		= nullptr;
	StageEditMeusurs* m_pStageEditMeusurs	= nullptr;

	FLOAT m_scrollBottom = 0.0f;
	INT m_noteNum = 0;
	INT m_meusurNum = 0;
};

#endif //! STAGE_EDIT_SCENE_OBJ_H
