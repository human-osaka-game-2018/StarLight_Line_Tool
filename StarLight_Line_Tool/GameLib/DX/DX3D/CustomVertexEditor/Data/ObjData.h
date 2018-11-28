/**
* @file ObjData.h
* @brief 物体の状態構造体とテクスチャの座標構造体のヘッダ
* @author Harutaka-Tsujino
*/

#ifndef OBJ_DATA_H
#define OBJ_DATA_H

#include <windows.h>

#include <d3dx9.h>

/**
* @brief テクスチャの座標
*/
struct TexUV
{
public:
	FLOAT m_startTU = 0.0f;
	FLOAT m_startTV = 0.0f;
	FLOAT m_endTU = 1.0f;
	FLOAT m_endTV = 1.0f;
};

/**
* @brief 物体の状態
*/
struct ObjData
{
public:
	D3DXVECTOR3 m_center	= { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 m_halfScale = { 0.0f,0.0f,0.0f };

	D3DXVECTOR3 m_deg = { 0.0f,0.0f,0.0f };	//! x y z軸の順番

	DWORD m_aRGB = 0xFFFFFFFF;
	TexUV m_texUV;
};

#endif //! OBJ_DATA_H
