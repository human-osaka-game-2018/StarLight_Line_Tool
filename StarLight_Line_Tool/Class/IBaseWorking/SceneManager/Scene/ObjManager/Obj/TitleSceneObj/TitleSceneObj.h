/// @file TitleSceneObj.h
/// <summary>
/// タイトルシーンで用いるオブジェクト継承クラスのヘッダ
/// </summary>
/// @author Harutaka-Tsujino

#ifndef TITLE_SCENE_OBJ_H
#define TITLE_SCENE_OBJ_H

#include <windows.h>
#include <tchar.h>

#include "../Obj.h"
#include "../../../../../../../GameLib/GameLib.h"
#include "../../../../../../../GameLib/Wnd/Data/RectSize.h"
#include "../../../Enum/SCENE_KIND.h"

class TitleBack :public Obj
{
public:
	TitleBack() :Obj(OT_BACK, 1.0f)
	{
		Init();
	}

	~TitleBack()
	{
		m_rGameLib.ReleaseTex();
	}

	inline VOID Init() const
	{
		m_rGameLib.CreateTex(_T("Back"), _T("2DTextures/back.png"));
	}

	inline VOID Update() {};

	inline VOID Render()
	{
		m_rGameLib.SetCameraTransform();

		ObjData data;
		data.m_center		= { m_WND_SIZE.m_x * 0.5f, m_WND_SIZE.m_y * 0.5f, m_Z };
		data.m_halfScale	= { m_WND_SIZE.m_x * 0.5f, m_WND_SIZE.m_y * 0.5f, 0.0f };

		CustomVertex back[4];
		m_rGameLib.CreateRect(back, data);

		m_rGameLib.Render(back, m_rGameLib.GetTex(_T("Back")));
	}
};

class TitleLogo :public Obj
{
public:
	TitleLogo() :Obj(OT_TRANSPARENCY, 0.98f)
	{
		Init();
	}

	~TitleLogo()
	{
		m_rGameLib.ReleaseTex();
	}

	inline VOID Init() const
	{
		m_rGameLib.CreateTex(_T("Logo"), _T("2DTextures/Title/TitleLogo.png"));
	}

	inline VOID Update() {};

	inline VOID Render()
	{
		ObjData data;
		data.m_center		= { m_WND_SIZE.m_x * 0.5f, m_WND_SIZE.m_y * 0.35f, m_Z };	//! 現物合わせ
		data.m_halfScale	= { m_WND_SIZE.m_x * 0.29f, m_WND_SIZE.m_y * 0.2f, 0.0f };	//! 現物合わせ

		CustomVertex logo[4];
		m_rGameLib.CreateRect(logo, data);

		m_rGameLib.Render(logo, m_rGameLib.GetTex(_T("Logo")));
	}
};

class TitleMenu :public Obj
{
public:
	TitleMenu() :Obj(OT_TRANSPARENCY, 0.98f)
	{
		Init();
	}

	~TitleMenu()
	{
		m_rGameLib.ReleaseTex();
	}

	inline VOID Init() const
	{
		m_rGameLib.CreateTex(_T("NewGame"), _T("2DTextures/Title/TitleMenuNewGame.png"));
		m_rGameLib.CreateTex(_T("LoadGame"), _T("2DTextures/Title/TitleMenuLoadGame.png"));
		m_rGameLib.CreateTex(_T("EndGame"), _T("2DTextures/Title/TitleMenuEndGame.png"));
	}	

	inline VOID Update()
	{
		SelectMenu();
	}

	inline VOID Render();

private:
	enum MENU_KIND
	{
		MK_NEW_GAME,
		MK_LOAD_GAME,
		MK_END_GAME,
		MK_MAX
	};

	inline VOID RotateMenuUp()
	{
		MENU_KIND menuReelTmp[MK_MAX] = { m_menuReel[1], m_menuReel[2],m_menuReel[0] };
		memcpy(
			&m_menuReel, 
			&menuReelTmp, 
			sizeof(MENU_KIND) * MK_MAX);
	}

	inline VOID RotateMenuDown()
	{
		MENU_KIND menuReelTmp[MK_MAX] = { m_menuReel[2], m_menuReel[0], m_menuReel[1] };
		memcpy(
			&m_menuReel, 
			&menuReelTmp, 
			sizeof(MENU_KIND) * MK_MAX);
	}

	VOID SelectMenu();

	MENU_KIND m_menuReel[MK_MAX] = { MK_END_GAME, MK_NEW_GAME, MK_LOAD_GAME };
	const INT m_CENTER_MENU = 1;
};

class TitleCometEffect :public Obj
{
public:
	TitleCometEffect() :Obj(OT_TRANSPARENCY, 0.99f)
	{
		Init();
	}

	~TitleCometEffect() 
	{
		m_rGameLib.ReleaseTex();
	}

	inline VOID Init()
	{
	}

	inline VOID Update() {};

	VOID Render();

private:
	struct EffectData
	{
	public:
		BOOL m_isInit = FALSE;
		ObjData m_data;
		D3DXVECTOR3 m_movement = { 0.0f, 0.0f, 0.0f };
		CustomVertex m_vertices[4];
	};

	VOID InitEffect(EffectData* pEffectDatas);

	BOOL m_isInitInFrame = FALSE;
};

#endif // !TITLE_SCENE_OBJ_H
