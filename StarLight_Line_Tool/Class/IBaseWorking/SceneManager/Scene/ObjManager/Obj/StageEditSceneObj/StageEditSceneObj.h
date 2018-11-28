#ifndef STAGE_EDIT_SCENE_OBJ_H
#define STAGE_EDIT_SCENE_OBJ_H

#include <windows.h>
#include <tchar.h>
#include <math.h>

#include <vector>

#include <d3dx9.h>

#include "../Obj.h"

class StageEditBack :public Obj
{
public:
	StageEditBack() :Obj(OT_BACK, 1.0f)
	{
		Init();
	}

	~StageEditBack()
	{
		m_rGameLib.ReleaseTex();
	}

	inline VOID Init() const
	{
		m_rGameLib.CreateTex(_T("Back"), _T("2DTextures/StageEdit/Back.png"));
	}

	inline VOID Update() {};

	inline VOID Render()
	{
		m_rGameLib.SetCameraTransform();

		ObjData data;
		data.m_center		= { m_WND_SIZE.m_x * 0.5f, m_WND_SIZE.m_y * 0.5f, m_Z };	//! åªï®çáÇÌÇπ
		data.m_halfScale	= { m_WND_SIZE.m_x * 0.5f, m_WND_SIZE.m_y * 0.5f, 0.0f };	//! åªï®çáÇÌÇπ

		CustomVertex back[4];
		m_rGameLib.CreateRect(back, data);

		m_rGameLib.Render(back, m_rGameLib.GetTex(_T("Back")));
	}
};

class StageEditStars :public Obj
{
public:
	StageEditStars() :Obj(OT_TRANSPARENCY, 0.9f)
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
	enum STAR_TYPE
	{
		STAR_DAMAGE,
		STAR_SCORE,
		STAR_CLEAR,
		STAR_MAX
	};

	struct StarData
	{
		ObjData m_objData;
		CustomVertex m_customVertices[4];
		INT starType = STAR_SCORE;
	};

	inline BOOL CollidesCircles(CustomVertex* pA, CustomVertex* pB) const
	{
		D3DXVECTOR3 aSidelVec = pA[1].m_pos - pA[0].m_pos;
		FLOAT aRadius = D3DXVec3Length(&aSidelVec) / 2;
		D3DXVECTOR3 aCenterVec = pA[0].m_pos + (pA[2].m_pos - pA[0].m_pos) / 2;

		D3DXVECTOR3 bSidelVec = pB[1].m_pos - pB[0].m_pos;
		FLOAT bRadius = D3DXVec3Length(&bSidelVec) / 2;
		D3DXVECTOR3 bCenterVec = pB[0].m_pos + (pB[2].m_pos - pB[0].m_pos) / 2;

		D3DXVECTOR3 distanceVec = aCenterVec - bCenterVec;
		FLOAT distance = D3DXVec3Length(&distanceVec);

		if (distance < aRadius + bRadius) return TRUE;

		return FALSE;
	}

	FLOAT m_scrollBottom = 0.0f;

	INT m_selectingStarType = STAR_SCORE;
	CustomVertex m_select[4 * STAR_MAX];

	INT m_starNums[STAR_MAX];	//! Init()Ç≈èâä˙âª

	std::vector<StarData*> m_pStarDataVec;
};

#endif //! STAGE_EDIT_SCENE_OBJ_H
