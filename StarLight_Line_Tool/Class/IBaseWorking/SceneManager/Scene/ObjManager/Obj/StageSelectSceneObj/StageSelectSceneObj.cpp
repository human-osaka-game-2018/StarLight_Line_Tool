#include "StageSelectSceneObj.h"

#include <windows.h>
#include <tchar.h>
#include <math.h>

#include <d3dx9.h>

#include "../../../Enum/SCENE_KIND.h"
#include "../../../../SceneManager.h"

VOID StageSelectSceneStageList::Update()
{
	SceneManager& rSceneManager = SceneManager::GetInstance();

	if (UpKeyIsPressed() &&
		m_backIsSelected)
	{
		m_backIsSelected = FALSE;
	}

	if (ReturnKeyIsPressed() &&
		m_backIsSelected)
	{
		rSceneManager.SetNextScene(SK_TITLE);

		return;
	}

	if (m_backIsSelected) return;

	if (ReturnKeyIsPressed() &&
		m_deg == 0.0f &&
		m_lengthMulti == 0.0f)
	{
		m_lengthMulti = -1.0f;

		return;
	}

	if (m_lengthMulti != 0.0f || m_isDecided) return;

	if (DownKeyIsPressed())
	{
		m_backIsSelected = TRUE;

		return;
	}

	const FLOAT ROTATE_SPEED = 1.5f;

	if (m_deg > 0.0f) m_deg += ROTATE_SPEED;		//! すでに回転されていたらさらに回転させる
	if (m_deg < 0.0f) m_deg += -ROTATE_SPEED;

	if (m_deg != 0) return;

	if (RightKeyIsPressed())m_deg = ROTATE_SPEED;	//! 回転の起動
	if (LeftKeyIsPressed())m_deg = -ROTATE_SPEED;
}

VOID StageSelectSceneStageList::Render()
{
	if (m_isDecided) m_rGameLib.AddtionBlendMode();

	const D3DXVECTOR3 ICONS_CENTER = { m_WND_SIZE.m_x * 0.5f, m_WND_SIZE.m_y * 0.4f, m_Z };	//! 現物合わせ
	const FLOAT ICONS_CIRCLE_RADIUS_MAX = 230.0f;											//! 複数のアイコンがなす円の半径の最大値
	static FLOAT iconsCircleRadius = ICONS_CIRCLE_RADIUS_MAX;								//! 複数のアイコンがなす円の半径

	const INT DECIDE_STAGE_FRAMES = 60;
	iconsCircleRadius += m_lengthMulti * ICONS_CIRCLE_RADIUS_MAX / DECIDE_STAGE_FRAMES;		//! DECIDE_STAGE_FRAMESで半径が最大値に達する
	iconsCircleRadius = min(max(iconsCircleRadius, 0), ICONS_CIRCLE_RADIUS_MAX);

	if (iconsCircleRadius == ICONS_CIRCLE_RADIUS_MAX ||
		!iconsCircleRadius)
	{
		m_lengthMulti = 0.0f;

		m_isDecided = !iconsCircleRadius;
	}

	D3DXMATRIX rotate;
	const FLOAT DEG_GAP = 360.0f / m_STAGE_ICONS_MAX;										//! アイコンとアイコンの角度
	FLOAT* pDeg = nullptr;

	D3DXVECTOR3* pCenter = nullptr;
	FLOAT halfScale = 0.0f;

	const RectSize ILLUST_SIZE = { 2048, 1024 };
	const FLOAT ICON_ILLUST_SIZE = 303.0f;
	const INT ICONS_ILLUST_ROWS_MAX = 6;

	FLOAT ICON_SCALE_MULTI = 0.0f;

	StageIconData stageIconDatas[m_STAGE_ICONS_MAX];
	CustomVertex stageIcon[4];

	for (int i = 0; i < m_STAGE_ICONS_MAX; ++i)
	{
		if (i != m_selectingStage && m_isDecided) continue;

		pDeg = &stageIconDatas[i].m_deg;
		*pDeg = DEG_GAP * (i - m_selectingStage) + m_deg;
		D3DXMatrixRotationZ(&rotate, D3DXToRadian(*pDeg));

		pCenter = &stageIconDatas[i].m_objData.m_center;
		pCenter->y = iconsCircleRadius;
		D3DXVec3TransformCoord(pCenter, pCenter, &rotate);
		*pCenter += ICONS_CENTER;
		if (i == m_selectingStage) pCenter->z -= 0.1f;

		ICON_SCALE_MULTI = cos(D3DXToRadian(*pDeg)) + 2.0f;													//! 1～3までの拡大率を角度によって決める

		halfScale = (((ICONS_CIRCLE_RADIUS_MAX - iconsCircleRadius) * 0.01f) + 17.5f) * ICON_SCALE_MULTI;	//! (最大値との半径の差 * 半径の差が大きくなりすぎるので縮小倍率 + 半径の最小の値) * 角度によるアイコンの拡大率
		stageIconDatas[i].m_objData.m_halfScale = { halfScale, halfScale, 0.0f };

		stageIconDatas[i].m_objData.m_texUV =
		{
			ICON_ILLUST_SIZE * (i % ICONS_ILLUST_ROWS_MAX)		/ ILLUST_SIZE.m_x,
			ICON_ILLUST_SIZE * (i / ICONS_ILLUST_ROWS_MAX)		/ ILLUST_SIZE.m_y,
			ICON_ILLUST_SIZE * (i % ICONS_ILLUST_ROWS_MAX + 1)	/ ILLUST_SIZE.m_x,
			ICON_ILLUST_SIZE * (i / ICONS_ILLUST_ROWS_MAX + 1)	/ ILLUST_SIZE.m_y
		};

		m_rGameLib.CreateRect(stageIcon, stageIconDatas[i].m_objData);

		m_rGameLib.Render(stageIcon, m_rGameLib.GetTex(_T("Icons")));

		if (m_deg != DEG_GAP && m_deg != -DEG_GAP) continue;
		m_selectingStage -= static_cast<INT>(m_deg / DEG_GAP);												//! 角度が60を超えるとm_selectingStageを次の値に変える
		m_deg = 0.0f;

		if (m_selectingStage < 0) m_selectingStage = m_STAGE_ICONS_MAX + m_selectingStage;					//! m_selectingStageがマイナスに行ったときの対処
		m_selectingStage %= m_STAGE_ICONS_MAX;
	}

	m_rGameLib.DefaultBlendMode();

	if (iconsCircleRadius == 0.0f) return;

	ObjData backButtonData;
	backButtonData.m_center		= { m_WND_SIZE.m_x * 0.5f, m_WND_SIZE.m_y * 0.96f, m_Z };					//! 現物合わせ
	backButtonData.m_halfScale	= { m_WND_SIZE.m_x * 0.05f, m_WND_SIZE.m_y * 0.04f, 0.0f };					//! 現物合わせ

	backButtonData.m_aRGB = D3DCOLOR_ARGB(
							static_cast<UCHAR>(iconsCircleRadius),
							255, 255, 255);

	backButtonData.m_texUV =
	{
		-(m_backIsSelected -1) / 2.0f,
		0.0f,
		(!m_backIsSelected + 1.0f) / 2.0f,
		1.0f
	};

	CustomVertex backButton[4];
	m_rGameLib.CreateRect(backButton, backButtonData);

	m_rGameLib.Render(backButton, m_rGameLib.GetTex(_T("BackButton")));
}

VOID StageSelectSceneLevelSelecter::Update()
{
	m_shouldActivateStageSelect = FALSE;

	if (!m_rIsDecided)
	{
		m_sceneTranlationAlpha = 0;

		return;
	}

	//if (m_sceneTranlationAlpha) return;

	if (UpKeyIsPressed())
	{
		m_backIsSelected = TRUE;

		return;
	}

	if (DownKeyIsPressed())
	{
		m_backIsSelected = FALSE;

		return;
	}

	if (ReturnKeyIsPressed() && m_backIsSelected)
	{
		m_shouldActivateStageSelect = TRUE;

		return;
	}

	if (m_backIsSelected) return;

	if (RightKeyIsPressed())
	{
		m_level = (m_level < SLK_HARD) ? ++m_level : SLK_HARD;

		return;
	}

	if (LeftKeyIsPressed())
	{
		m_level = (m_level > SLK_EASY) ? --m_level : SLK_EASY;

		return;
	}

	if (ReturnKeyIsPressed())
	{
		//ステージの決定
		m_sceneTranlationAlpha = 1;
	}
}

VOID StageSelectSceneLevelSelecter::Render()
{
	ObjData backData;
	backData.m_center		= { m_WND_SIZE.m_x * 0.5f, m_WND_SIZE.m_y * 0.5f, m_Z };
	backData.m_halfScale	= { m_WND_SIZE.m_x * 0.5f, m_WND_SIZE.m_y * 0.5f, 0.0f };

	static INT alpha = 0;
	alpha += 10 * ((m_rIsDecided) ? 1 : -1);
	alpha = min(max(alpha, 0), 255);

	if (!alpha)
	{
		m_backIsSelected = FALSE;	//! 0か1しか入れてはいけない

		return;
	}

	backData.m_aRGB = D3DCOLOR_ARGB(alpha, 255, 255, 255);

	CustomVertex back[4];
	m_rGameLib.CreateRect(back, backData);

	m_rGameLib.Render(back, m_rGameLib.GetTex(_T("LevelBack")));

	ObjData selectData;
	selectData.m_center		= { m_WND_SIZE.m_x * 0.22f + m_WND_SIZE.m_x * 0.28f * m_level, m_WND_SIZE.m_y * 0.7f, m_Z };	//! 現物合わせ
	selectData.m_halfScale	= { m_WND_SIZE.m_x * 0.1f, m_WND_SIZE.m_y * 0.05f, 0.0f };										//! 現物合わせ

	selectData.m_aRGB = D3DCOLOR_ARGB(alpha, 255, 255, 255);
	if(m_backIsSelected) selectData.m_aRGB = D3DCOLOR_ARGB(0, 255, 255, 255);

	CustomVertex select[4];
	m_rGameLib.CreateRect(select, selectData);

	m_rGameLib.Render(select, m_rGameLib.GetTex(_T("LevelSelectFrame")));

	ObjData backButtonData;
	backButtonData.m_center		= { m_WND_SIZE.m_x * 0.14f, m_WND_SIZE.m_y * 0.17f, m_Z };									//! 現物合わせ
	backButtonData.m_halfScale	= { m_WND_SIZE.m_y * 0.05f, m_WND_SIZE.m_y * 0.05f, 0.0f };									//! 現物合わせ

	backButtonData.m_aRGB = D3DCOLOR_ARGB(alpha, 0, 0, 0);
	if (m_backIsSelected) backButtonData.m_aRGB = D3DCOLOR_ARGB(alpha, 255, 255, 255);

	CustomVertex backButton[4];
	m_rGameLib.CreateRect(backButton, backButtonData);

	m_rGameLib.Render(backButton, m_rGameLib.GetTex(_T("LevelBackButton")));

	if (!m_sceneTranlationAlpha) return;

	ObjData sceneTranslationData;
	sceneTranslationData.m_center		= { m_WND_SIZE.m_x * 0.5f, m_WND_SIZE.m_y * 0.5f, m_Z };
	sceneTranslationData.m_halfScale	= { m_WND_SIZE.m_x * 0.5f, m_WND_SIZE.m_y * 0.5f, 0.0f };

	sceneTranslationData.m_aRGB = D3DCOLOR_ARGB(m_sceneTranlationAlpha, 255, 255, 255);

	CustomVertex sceneTranslation[4];
	m_rGameLib.CreateRect(sceneTranslation, sceneTranslationData);

	m_rGameLib.Render(sceneTranslation, nullptr);

	m_sceneTranlationAlpha += 2 * ((m_sceneTranlationAlpha) ? 1 : -1);
	m_sceneTranlationAlpha = min(max(m_sceneTranlationAlpha, 0), 255);
}
