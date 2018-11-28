/// <summary>
/// セーブデーターシーンで扱うObj継承クラスのヘッダ
/// </summary>

#ifndef SAVE_DATA_SCENE_OBJ_H
#define SAVE_DATA_SCENE_OBJ_H

#include <windows.h>
#include <tchar.h>
#include <math.h>

#include <d3dx9.h>

#include "../Obj.h"
#include "../../../../SceneManager.h"

class SaveDataBack :public Obj
{
public:
	SaveDataBack() :Obj(OT_BACK, 1.0f)
	{
		Init();
	}

	~SaveDataBack()
	{
		m_rGameLib.ReleaseTex();
	}

	inline VOID Init() const
	{
		m_rGameLib.CreateTex(_T("Back"), _T("2DTextures/SaveData/SaveData_Back.png"));
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

class SaveDataSaveDatas :public Obj
{
public:
	SaveDataSaveDatas() :Obj(OT_TRANSPARENCY, 0.9f)
	{
		Init();
	}

	~SaveDataSaveDatas()
	{
		m_rGameLib.ReleaseTex();
	}

	inline VOID Init() const
	{
		m_rGameLib.CreateTex(_T("Frame"), _T("2DTextures/SaveData/SaveData_Frame01.png"));
		m_rGameLib.CreateTex(_T("DetailFrame"), _T("2DTextures/SaveData/SaveData_Frame02.png"));
	}

	inline VOID Update() 
	{
		if (ReturnKeyIsPressed())
		{
			SceneManager& rSceneManager = SceneManager::GetInstance();

			rSceneManager.SetNextScene(SK_STAGE_SELECT);
		}
	}

	VOID Render();

private:
	const INT m_SAVE_DATAS_MAX = 3;	//! 仮にこのクラスに持たせているがSaveDataManagerを作りにそれに持たせる予定

	INT m_selectingData = 0;
};

#endif //! SAVE_DATA_SCENE_OBJ_H
