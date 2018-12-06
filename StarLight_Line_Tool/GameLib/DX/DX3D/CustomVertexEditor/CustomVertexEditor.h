/**
* @file CustomVertexEditor.h
* @brief 頂点データ構造体とその構造体の編集を行うクラスのヘッダ
* @author Harutaka Tsujino
*/

#ifndef CUSTOM_VERTEX_EDITOR_H
#define CUSTOM_VERTEX_EDITOR_H

#include <windows.h>

#include <d3dx9.h>

#include "Data\CustomVertex.h"
#include "Data\ObjData.h"

/**
* @brief 頂点データ構造体とその構造体の編集を行うクラス
*/
class CustomVertexEditor
{
public:
	CustomVertexEditor(const LPDIRECT3DDEVICE9& rpDX3DDev) :m_rpDX3D_DEV(rpDX3DDev) {};
	~CustomVertexEditor() {};

	/**
	* @brief XYZ順に回転を行う
	* @param[in,out] pCustomVertices 頂点データ配列の先頭アドレス
	* @param rDeg 回転をさせる度数法での角度
	* @param rRelativeRotateCenter どれほど回転の中心が矩形の中心よりずれているか
	* @detail 回転行列を作成し、矩形の中心を求め回転の中心を原点に移動させ、回転行列を用いて回転を行い原点へ移動させた分元に戻す
	*/
	inline VOID RotateXYZ(CustomVertex* pCustomVertices, const D3DXVECTOR3& rDeg, const D3DXVECTOR3& rRelativeRotateCenter) const
	{
		RotateX(
			pCustomVertices, 
			rDeg.x, 
			rRelativeRotateCenter);

		RotateY(
			pCustomVertices, 
			rDeg.y, 
			rRelativeRotateCenter);

		RotateZ(pCustomVertices, 
			rDeg.z, 
			rRelativeRotateCenter);
	}

	inline VOID RotateXYZ(CustomVertex* pCustomVertices, const D3DXVECTOR3& rDeg) const
	{
		D3DXVECTOR3 relativeRotateCenter(0.0f, 0.0f, 0.0f);

		RotateX(
			pCustomVertices,
			rDeg.x,
			relativeRotateCenter);

		RotateY(
			pCustomVertices,
			rDeg.y,
			relativeRotateCenter);

		RotateZ(pCustomVertices,
			rDeg.z,
			relativeRotateCenter);
	}

	/**
	* @brief x軸で回転させる
	* @param[in,out] pCustomVertices 頂点データ配列の先頭アドレス
	* @param 度数法での角度
	* @param rRelativeRotateCenter どれほど回転の中心が矩形の中心よりずれているか
	*/
	inline VOID RotateX(CustomVertex* pCustomVertices, FLOAT deg, const D3DXVECTOR3& rRelativeRotateCenter) const
	{
		if (deg == 0) return;

		D3DXMATRIX rotate;
		D3DXMatrixRotationX(&rotate, D3DXToRadian(deg));
		Rotate(
			pCustomVertices, 
			rRelativeRotateCenter, 
			rotate);
	}

	inline VOID RotateY(CustomVertex* pCustomVertices, FLOAT deg, const D3DXVECTOR3& rRelativeRotateCenter) const
	{
		if (deg == 0) return;

		D3DXMATRIX rotate;
		D3DXMatrixRotationY(&rotate, D3DXToRadian(deg));
		Rotate(
			pCustomVertices, 
			rRelativeRotateCenter, 
			rotate);
	}

	inline VOID RotateZ(CustomVertex* pCustomVertices, FLOAT deg, const D3DXVECTOR3& rRelativeRotateCenter) const
	{
		if (deg == 0) return;

		D3DXMATRIX rotate;
		D3DXMatrixRotationZ(&rotate, D3DXToRadian(deg));
		Rotate(
			pCustomVertices, 
			rRelativeRotateCenter, 
			rotate);
	}

	/**
	* @brief 矩形を拡縮させる
	* @param[in,out] pCustomVertices 頂点データ配列の先頭アドレス
	* @param rScaleRate 拡縮率
	* @detail 矩形の中心を求め幅と高さを割り出し、拡縮率を幅と高さに掛け合わせ、矩形の中心点から再構成させる
	*/
	VOID Rescale(CustomVertex* pCustomVertices, const D3DXVECTOR2& rScaleRate) const;

	/**
	* @brief 矩形を移動させる
	* @param[in,out] pCustomVertices 頂点データ配列の先頭アドレス
	* @param rMovement 移動量
	*/
	inline VOID Move(CustomVertex* pCustomVertices, const D3DXVECTOR3& rMovement) const
	{
		for (int i = 0; i < m_RECT_VERTICES_NUM; ++i)
		{
			D3DXVec3Add(
				&pCustomVertices[i].m_pos, 
				&pCustomVertices[i].m_pos, 
				&rMovement);
		}
	}

	/**
	* @brief 矩形を引数の位置に再配置させる
	* @param[in,out] pCustomVertices 頂点データ配列の先頭アドレス
	* @param rPos 矩形を移動させる座標
	*/
	inline VOID Locale(CustomVertex* pCustomVertices, const D3DXVECTOR3& rPos) const
	{
		for (int i = 0; i < m_RECT_VERTICES_NUM; ++i)
		{
			pCustomVertices[i].m_pos = rPos;
		}
	}

	/**
	* @brief テクスチャ座標を引数の座標に変更させる
	* @param[in,out] pCustomVertices 頂点データ配列の先頭アドレス
	* @param startTU x方向のテクスチャ座標の始まりの値
	* @param startTV y方向のテクスチャ座標の始まりの値
	* @param endTU x方向のテクスチャ座標の終わりの値
	* @param endTV y方向のテクスチャ座標の終わりの値
	*/
	inline VOID SetTexUV(CustomVertex* pCustomVertices,
		FLOAT startTU = 0.0f, FLOAT startTV = 0.0f, FLOAT endTU = 1.0f, FLOAT endTV = 1.0f) const
	{
		for (int i = 0; i < m_RECT_VERTICES_NUM; ++i)
		{
			pCustomVertices[i].m_texUV.x = (i % 3) ? endTU : startTU;
			pCustomVertices[i].m_texUV.y = (i / 2) ? endTV : startTV;
		}
	}

	/**
	* @brief テクスチャ座標を引数の座標に変更させる
	* @param[in,out] pCustomVertices 頂点データ配列の先頭アドレス
	* @param aRGB アルファ値入りのカラーコード ARGB
	*/
	inline VOID SetARGB(CustomVertex *pCustomVertices, DWORD aRGB) const
	{
		for (int i = 0; i < m_RECT_VERTICES_NUM; ++i)
		{
			pCustomVertices[i].m_aRGB = aRGB;
		}
	}

	/**
	* @brief 頂点データ構造体を引数の値から作成する
	* @param[out] pCustomVertices 頂点データ配列の先頭アドレス
	* @param rCenter 矩形の中心
	* @param rHalfScale 矩形の高さ幅の半分
	* @param aRGB 頂点の色カラーコード
	* @param startTU x方向のテクスチャ座標の始まりの値
	* @param startTV y方向のテクスチャ座標の始まりの値
	* @param endTU x方向のテクスチャ座標の終わりの値
	* @param endTV y方向のテクスチャ座標の終わりの値
	*/
	inline VOID Create(CustomVertex* pCustomVertices, const D3DXVECTOR3& rCenter, const D3DXVECTOR3& rHalfScale,
		DWORD aRGB = 0xFFFFFFFF, FLOAT startTU = 0.0f, FLOAT startTV = 0.0f, FLOAT endTU = 1.0f, FLOAT endTV = 1.0f) const
	{
		for (int i = 0; i < m_RECT_VERTICES_NUM; ++i)
		{
			pCustomVertices[i].m_pos = rCenter;
			pCustomVertices[i].m_pos.x += (i % 3) ? rHalfScale.x : -rHalfScale.x;
			pCustomVertices[i].m_pos.y += (i / 2) ? rHalfScale.y : -rHalfScale.y;

			pCustomVertices[i].m_aRGB = aRGB;
		}

		SetTexUV(
			pCustomVertices, 
			startTU, startTV,
			endTU, endTV);
	}

	/**
	* @brief 頂点データ構造体を引数の値から作成する 回転も行う
	* @param[out] pCustomVertices 頂点データ配列の先頭アドレス
	* @param rObj オブジェクトの状態構造体
	*/
	inline VOID Create(CustomVertex *pCustomVertices, const ObjData& rObj) const
	{
		Create(
			pCustomVertices,
			rObj.m_center, rObj.m_halfScale,
			rObj.m_aRGB,
			rObj.m_texUV.m_startTU, rObj.m_texUV.m_startTV,
			rObj.m_texUV.m_endTU, rObj.m_texUV.m_endTV);

		RotateXYZ(pCustomVertices, rObj.m_deg);
	}

public:
	const LPDIRECT3DDEVICE9& m_rpDX3D_DEV = nullptr;

	static const INT m_RECT_VERTICES_NUM = 4;	//! 矩形を構成する頂点の数

	VOID Rotate(CustomVertex* pCustomVertices, const D3DXVECTOR3& rRelativeRotateCenter, const D3DXMATRIX& rRotate) const;
};

#endif //! CUSTOM_VERTEX_EDITOR_H
