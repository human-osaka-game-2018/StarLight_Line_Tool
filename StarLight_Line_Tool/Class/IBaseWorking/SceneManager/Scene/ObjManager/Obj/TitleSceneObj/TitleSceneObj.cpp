/// @file TitleSceneObj.cpp
/// <summary>
/// タイトルシーンで用いるオブジェクト継承クラスのソース
/// </summary>
/// @author Harutaka-Tsujino

#include "TitleSceneObj.h"

#include <windows.h>
#include <tchar.h>

#include "../Obj.h"
#include "../../../../../../../GameLib/GameLib.h"
#include "../../../../SceneManager.h"
#include "../../../../../../../GameLib/Wnd/Data/RectSize.h"
#include "../../../Enum/SCENE_KIND.h"

VOID TitleMenu::Render()
{
	ObjData data;
	CustomVertex menu[4];
	const FLOAT CENTER_MENU_SCALE_MULTI = 1.5f;

	for (INT i = 0; i < MK_MAX; ++i)
	{
		data.m_center		= { m_WND_SIZE.m_x * 0.5f, m_WND_SIZE.m_y * (0.65f + 0.07f * i), m_Z };	//! 現物合わせ
		data.m_halfScale	= { m_WND_SIZE.m_x * 0.055f, m_WND_SIZE.m_y * 0.029f ,0.0f };			//! 現物合わせ
		if (i == m_CENTER_MENU) data.m_halfScale *= CENTER_MENU_SCALE_MULTI;

		data.m_aRGB = D3DCOLOR_ARGB(200, 255, 255, 255);											//! 現物合わせ

		m_rGameLib.CreateRect(menu, data);

		switch (m_menuReel[i])
		{
		case MK_NEW_GAME:
			m_rGameLib.Render(menu, m_rGameLib.GetTex(_T("NewGame")));

			break;

		case MK_LOAD_GAME:
			m_rGameLib.Render(menu, m_rGameLib.GetTex(_T("LoadGame")));

			break;

		case MK_END_GAME:
			m_rGameLib.Render(menu, m_rGameLib.GetTex(_T("EndGame")));

			break;

		default:
			break;
		}
	}
}

VOID TitleMenu::SelectMenu()
{
	if (UpKeyIsPressed())
	{
		RotateMenuDown();
	}

	if (DownKeyIsPressed())
	{
		RotateMenuUp();
	}

	if (!ReturnKeyIsPressed()) return;

	SceneManager& rSceneManager = SceneManager::GetInstance();
	switch (m_menuReel[m_CENTER_MENU])
	{
	case MK_NEW_GAME:
		rSceneManager.SetNextScene(SK_STAGE_SELECT);

		break;

	case MK_LOAD_GAME:
		rSceneManager.SetNextScene(SK_STAGE_EDIT);

		break;

	case MK_END_GAME:
		rSceneManager.SetNextScene(SK_SAVE_DATA);

		break;

	default:
		break;
	}
}

VOID TitleCometEffect::Render()
{
	m_isInitInFrame = FALSE;

	//m_rGameLib.AddtionBlendMode();

	const INT EFFECT_DATAS_MAX = 3;
	static EffectData effectDatas[EFFECT_DATAS_MAX];

	const D3DXVECTOR3 ROTATE_RELATIVE_POS(0.0f, 0.0f, 0.0f);

	FLOAT effectTop		= 0;
	FLOAT effectBottom	= 0;

	for (INT i = 0; i < EFFECT_DATAS_MAX; ++i)
	{
		D3DXVec3Add(
			&effectDatas[i].m_data.m_center,
			&effectDatas[i].m_data.m_center,
			&effectDatas[i].m_movement);

		m_rGameLib.CreateRect(effectDatas[i].m_vertices, effectDatas[i].m_data);

		effectTop		= effectDatas[i].m_vertices[0].m_pos.y;
		effectBottom	= effectDatas[i].m_vertices[3].m_pos.y;

		if (effectBottom > 0.0f && effectTop < m_WND_SIZE.m_y)										//! 画面内にいれば描画する
		{
			m_rGameLib.Render(effectDatas[i].m_vertices);

			continue;
		}

		if (effectTop > m_WND_SIZE.m_y) effectDatas[i].m_isInit = FALSE;							//! 画面の下までエフェクトが動いたら初期化する

		InitEffect(&effectDatas[i]);
	}

	//m_rGameLib.DefaultBlendMode();
}

VOID TitleCometEffect::InitEffect(EffectData* pEffectDatas)
{
	if (!m_rGameLib.KeyboardAnyKeyIsPressed()	||													//! ボタンが押されないとエフェクトを流さないまた一つしか一フレームに初期化しない
		pEffectDatas->m_isInit					|| 
		m_isInitInFrame) return;

	m_isInitInFrame = TRUE;

	static const INT EFFECT_COLORS_MAX = 4;
	static const DWORD EFFECT_COLORS[EFFECT_COLORS_MAX] =											//! オレンジ系統
	{
		D3DCOLOR_ARGB(220, 0xc0, 58, 00),
		D3DCOLOR_ARGB(220, 0xe0, 70, 00),
		D3DCOLOR_ARGB(220, 0xff, 80, 00),
		D3DCOLOR_ARGB(220, 0xff, 0xa0, 20)
	};

	pEffectDatas->m_data.m_center = {																//! エフェクトの初期位置は乱数を用いて画面の範囲で決定する
		static_cast<float>(rand() % m_WND_SIZE.m_x + m_WND_SIZE.m_x * 1.5f),
		-static_cast<float>(rand() % (m_WND_SIZE.m_y * 2) + m_WND_SIZE.m_y * 1.0f),
		m_Z };
	pEffectDatas->m_data.m_halfScale = { m_WND_SIZE.m_x * 0.01f, m_WND_SIZE.m_y * 2.0f, 0.0f };	//! 現物合わせ

	pEffectDatas->m_data.m_aRGB = EFFECT_COLORS[rand() % EFFECT_COLORS_MAX];

	pEffectDatas->m_data.m_deg.z = 45.0f;															//! 星の入射角
	const D3DXVECTOR3 MOVEMENT(0.0f, 30.0f, 0.0f);													//! 後でこれを星の入射角分回転させて入射角度と移動方向を合わせる
	D3DXMATRIX rotate;
	D3DXMatrixRotationZ(&rotate, D3DXToRadian(pEffectDatas->m_data.m_deg.z));
	D3DXVec3TransformCoord(
		&pEffectDatas->m_movement,
		&MOVEMENT,
		&rotate);

	pEffectDatas->m_isInit = TRUE;
}
