/// <filename>
/// Collision.h
/// </filename>
/// <summary>
/// 当たり判定クラスのヘッダ
/// </summary>
/// <author>
/// Harutaka-Tsujino
/// </author>

#ifndef COLLISION_H
#define COLLISION_H

#include <windows.h>

#include <d3dx9.h>

#include "../DX/DX3D/CustomVertexEditor/Data/ObjData.h"
#include "../DX/DX3D/CustomVertexEditor/Data/CustomVertex.h"

/// <summary>
/// 衝突しているかを判断する
/// </summary>
class Collision
{
public:
	Collision() {};
	~Collision() {};

	inline BOOL CollidesCircles(const ObjData& A, const ObjData& B) const
	{
		FLOAT aRadius = D3DXVec3Length(&A.m_halfScale);

		FLOAT bRadius = D3DXVec3Length(&B.m_halfScale);

		D3DXVECTOR3 distanceVec = A.m_center - B.m_center;
		FLOAT distance = D3DXVec3Length(&distanceVec);

		if (distance < aRadius + bRadius) return TRUE;

		return FALSE;
	}

	inline BOOL CollidesCircles(const CustomVertex* pA, const CustomVertex* pB) const
	{
		D3DXVECTOR3 aSideVec = pA[1].m_pos - pA[0].m_pos;
		FLOAT aRadius = D3DXVec3Length(&aSideVec) / 2;
		D3DXVECTOR3 aCenterVec = pA[0].m_pos + (pA[2].m_pos - pA[0].m_pos) / 2;

		D3DXVECTOR3 bSideVec = pB[1].m_pos - pB[0].m_pos;
		FLOAT bRadius = D3DXVec3Length(&bSideVec) / 2;
		D3DXVECTOR3 bCenterVec = pB[0].m_pos + (pB[2].m_pos - pB[0].m_pos) / 2;

		D3DXVECTOR3 distanceVec = aCenterVec - bCenterVec;
		FLOAT distance = D3DXVec3Length(&distanceVec);

		if (distance < aRadius + bRadius) return TRUE;

		return FALSE;
	}

	//矩形同士の衝突判定
	inline BOOL CollidesRects(const CustomVertex* pObjA, const CustomVertex* pObjB) const
	{
		BOOL collidesX =
			(pObjA[0].m_pos.x <= pObjB[0].m_pos.x && pObjB[0].m_pos.x <= pObjA[1].m_pos.x) ||
			(pObjA[0].m_pos.x <= pObjB[1].m_pos.x && pObjB[1].m_pos.x <= pObjA[1].m_pos.x) ||
			(pObjB[0].m_pos.x <= pObjA[0].m_pos.x && pObjA[1].m_pos.x <= pObjB[1].m_pos.x);

		BOOL collidesY =
			(pObjA[0].m_pos.y <= pObjB[0].m_pos.y && pObjB[0].m_pos.y <= pObjA[3].m_pos.y) ||
			(pObjA[0].m_pos.y <= pObjB[3].m_pos.y && pObjB[3].m_pos.y <= pObjA[3].m_pos.y) ||
			(pObjB[0].m_pos.y <= pObjA[0].m_pos.y && pObjA[3].m_pos.y <= pObjB[3].m_pos.y);

		if (collidesX && collidesY) return TRUE;

		return FALSE;
	}
};

#endif // !COLLISION_H
