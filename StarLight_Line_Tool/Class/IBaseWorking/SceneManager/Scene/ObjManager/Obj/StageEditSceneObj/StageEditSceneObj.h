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
#include "Struct\StarData.h"
#include "Struct\StarIcon.h"
#include "Struct\MeasureBPM.h"
#include "StageFile\StageFile.h"

#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

/// <summary>
/// ステージ作成時の背景 分によって画像を切り替え スクロールする 
/// </summary>
class StageEditBack :public Obj
{
public:
	StageEditBack(const INT& rBeatsNum, const FLOAT& rScrollBottom) :Obj(OT_BACK, 1.0f),
		m_rNoteNum(rBeatsNum), m_rScrollBottom(rScrollBottom)
	{
		Init();
	}

	~StageEditBack()
	{
		m_rGameLib.ReleaseTex();
	}

	inline VOID Init() const
	{
		m_rGameLib.CreateTex(_T("Back"), _T("2DTextures/StageEdit/StageEditBack.jpg"));
		m_rGameLib.CreateTex(_T("16Back"), _T("2DTextures/StageEdit/16BeatsBack.png"));
		m_rGameLib.CreateTex(_T("8Back"), _T("2DTextures/StageEdit/8BeatsBack.png"));
		m_rGameLib.CreateTex(_T("4Back"), _T("2DTextures/StageEdit/4BeatsBack.png"));
		m_rGameLib.CreateTex(_T("2Back"), _T("2DTextures/StageEdit/2BeatsBack.png"));
		m_rGameLib.CreateTex(_T("1Back"), _T("2DTextures/StageEdit/1BeatBack.png"));
	}

	inline VOID Update() {};

	/// <summary>
	/// 背景を分数によって切替 スクロールに合わせてTUTVをずらす
	/// </summary>
	VOID Render();

private:
	const INT& m_rNoteNum;			//! 分数

	const FLOAT& m_rScrollBottom;	//! スクロール量と同じ+
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

	/// <summary>
	/// 分を変更する
	/// </summary>
	VOID Update();

	/// <summary>
	/// 分のボタンなどを描画する
	/// </summary>
	/// <returns></returns>
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
/// 節のBPMの変更
/// </summary>
class StageEditMeasureBPM :public Obj
{
public:
	StageEditMeasureBPM(const FLOAT& rScrollBottom, const INT& rMeasuresNum) :Obj(OT_TRANSPARENCY, 0.9f),
		m_rScrollBottom(rScrollBottom), m_MEASURES_NUM(rMeasuresNum)
	{
		Init();
	}

	~StageEditMeasureBPM()
	{
		for (MeasureBPM* pI : m_pMeasureBPMVec)
		{
			delete pI;
		}

		m_rGameLib.ReleaseTex();
	}

	inline VOID Init()
	{
		StageFile& rStageFile =  StageFile::GetInstance();

		rStageFile.ReadMeasureBPM(&m_pMeasureBPMVec);

		m_rGameLib.CreateTex(_T("BPMButton"), _T("2DTextures/StageEdit/BPMNumButton.png"));
	}

	VOID Update();

	VOID Render()
	{
		for (INT i = 0; i < m_MEASURES_NUM; ++i)
		{
			RenderMeasureBPM(i);
			RenderBPMSetter(i);
		}
	}

	INT CurrentMeasureBPM() const;

	inline const BOOL IsChanged() const
	{
		return m_isChanged;
	}

	inline const std::vector<MeasureBPM*>& GetPMeasureBPMVec() const
	{
		return m_pMeasureBPMVec;
	}

private:
	VOID SetBPMDealWithClickedSetter(INT elementNum, const CustomVertex* pCursor);

	VOID RenderMeasureBPM(INT elementNum);

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

	const INT& m_MEASURES_NUM;
	std::vector<MeasureBPM*> m_pMeasureBPMVec;
};

/// <summary>
/// ステージの節の追加 削除
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
		delete m_pStageEditMeasureBPM;
		m_rGameLib.ReleaseTex();
	}

	inline VOID Init()
	{
		m_pStageEditMeasureBPM = new StageEditMeasureBPM(m_rScrollBottom, m_measuresNum);

		StageFile& rStageFile = StageFile::GetInstance();

		rStageFile.ReadMeasureNum(&m_measuresNum);

		m_rGameLib.CreateTex(_T("AddMeasure"), _T("2DTextures/StageEdit/AddMeasureButton.png"));
		m_rGameLib.CreateTex(_T("DeleteMeasure"), _T("2DTextures/StageEdit/DeleteMeasureButton.png"));
	}

	VOID Update();

	VOID Render();

	inline const BOOL IsChanged() const
	{
		BOOL isChanged = (m_pStageEditMeasureBPM->IsChanged() || m_isChanged);

		return isChanged;
	}

	INT CurrentMeasureBPM() const
	{
		return m_pStageEditMeasureBPM->CurrentMeasureBPM();
	}

	inline INT GetMeasuresNum() const
	{
		return m_measuresNum;
	}

	inline const std::vector<MeasureBPM*>& GetPMeasureBPMVec() const
	{
		return m_pStageEditMeasureBPM->GetPMeasureBPMVec();
	}

private:
	const FLOAT& m_rScrollBottom;

	BOOL m_isChanged = FALSE;

	INT m_measuresNum = 0;

	CustomVertex m_measureAdder[4];
	CustomVertex m_measureDeleter[4];

	StageEditMeasureBPM* m_pStageEditMeasureBPM = nullptr;
};

/// <summary>
/// ステージに置く星の種類を決める
/// </summary>
class StageEditStarSelecter :public Obj
{
public:
	StageEditStarSelecter() :Obj(OT_TRANSPARENCY, 0.9f)
	{
		Init();
	}

	~StageEditStarSelecter()
	{
		m_rGameLib.ReleaseTex();
	}

	inline VOID Init()
	{
		StageFile& rStageFile = StageFile::GetInstance();

		rStageFile.ReadStarNums(m_starIcons);

		m_rGameLib.CreateTex(_T("StarSelectIcon"), _T("2DTextures/StageEdit/StarSelectIcon.png"));
	}

	VOID Update();

	VOID Render();

	inline const BOOL IsChanged() const
	{
		return m_isChanged;
	}

	inline INT StarType()
	{
		return m_selectingStarType;
	}

	inline VOID SetStarNum(INT starType, BOOL isIncreased)
	{
		m_starIcons[starType].m_num += ((isIncreased) ? 1 : -1);
	}

	inline const StarIcon* GetStarIcons() const
	{
		return m_starIcons;
	}

private:
	VOID RenderStarNum(INT StarIconArrayNum);

	BOOL m_isChanged = FALSE;

	INT m_selectingStarType = STAR_DAMAGE;
	
	StarIcon m_starIcons[STAR_MAX];
};

/// <summary>
/// ステージに配置する星
/// </summary>
class StageEditStars :public Obj
{
public:
	StageEditStars(const FLOAT& rScrollBottom) :Obj(OT_TRANSPARENCY, 0.9f), 
		m_rScrollBottom(rScrollBottom), m_STAR_HALF_SCALE(m_WND_SIZE.m_y * 0.04f)
	{
		Init();
	}

	~StageEditStars()
	{
		for (StarData* pI : m_pStarDataVec)
		{
			delete pI;
		}
		
		delete m_pStageEditStarSelecter;
		delete m_pStageEditMeasures;
		delete m_pStageEditNote;
		m_rGameLib.ReleaseTex();
	}

	inline VOID Init()
	{
		m_pStageEditNote = new StageEditNote(m_rScrollBottom);

		m_pStageEditMeasures = new StageEditMeasures(m_rScrollBottom);

		m_pStageEditStarSelecter = new StageEditStarSelecter();

		StageFile& rStageFile = StageFile::GetInstance();

		rStageFile.ReadStarData(&m_pStarDataVec);

		m_rGameLib.CreateTex(_T("StarNote"), _T("2DTextures/StageEdit/StarNote.png"));
		m_rGameLib.CreateTex(_T("Vector"), _T("2DTextures/StageEdit/Vector.png"));
	}

	VOID Update();

	VOID Render();

	INT GetMeasuresNum()
	{
		return m_pStageEditMeasures->GetMeasuresNum();
	}

	VOID StartPreview(FLOAT ySpeed);

	VOID EndPreview()
	{
		for (StarData* pI : m_pStarDataVec)
		{
			pI->m_xMovement = NULL;
		}
	}

	inline INT CurrentMeasureBPM() const
	{
		return m_pStageEditMeasures->CurrentMeasureBPM();
	}
	
	inline BOOL CanChangeVector()
	{
		FLOAT starYPos = NULL;

		for (StarData* pI : m_pStarDataVec)
		{
			starYPos = pI->m_objData.m_center.y;
			if (starYPos < -m_STAR_HALF_SCALE ||
				starYPos > m_WND_SIZE.m_y + m_STAR_HALF_SCALE) continue;

			if (pI->m_isActiveChangingVector) return TRUE;
		}

		return FALSE;
	}

	inline const std::vector<StarData*>& GetPStarDataVec()
	{
		return m_pStarDataVec;
	}

	inline const std::vector<MeasureBPM*>& GetPMeasureBPMVec() const
	{
		return m_pStageEditMeasures->GetPMeasureBPMVec();
	}

	inline INT GetMeasuresNum() const
	{
		return m_pStageEditMeasures->GetMeasuresNum();
	}

	inline const StarIcon* GetStarIcons() const
	{
		return m_pStageEditStarSelecter->GetStarIcons();
	}

private:
	VOID SetStarDealWithCollidesCursorDivide(INT noteNum, POINT cursorPoint);
	
	VOID ChangeStarDataDeg();

	VOID DeleteClickedStarData(CustomVertex* pCusrsor);

	VOID RenderStarData();

	VOID RenderStarVector();

	const FLOAT& m_rScrollBottom;

	const FLOAT m_STAR_HALF_SCALE;

	std::vector<StarData*> m_pStarDataVec;

	INT m_beatElementNum	= NULL;

	StageEditNote* m_pStageEditNote = nullptr;
	StageEditMeasures* m_pStageEditMeasures = nullptr;
	StageEditStarSelecter* m_pStageEditStarSelecter = nullptr;
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

		m_rGameLib.CreateTex(_T("Preview"), _T("2DTextures/StageEdit/Preview.png"));
		m_rGameLib.CreateTex(_T("WriteFile"), _T("2DTextures/StageEdit/WriteFileButton.jpg"));
	}

	VOID Update();

	VOID Render();

private:
	FLOAT m_scrollBottom = 0.0f;
	INT m_noteNum		= 0;
	INT m_measuresNum = 0;

	BOOL m_previews = FALSE;
	CustomVertex m_preview[4];

	CustomVertex m_writeFileButton[4];

	StageEditStars* m_pStageEditStars = nullptr;
};

#endif //! STAGE_EDIT_SCENE_OBJ_H
