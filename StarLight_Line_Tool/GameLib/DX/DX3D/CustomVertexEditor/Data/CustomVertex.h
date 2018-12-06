/**
* @file CustomVertex.h
* @brief 頂点データ構造体のヘッダ
* @author Harutaka-Tsujino
*/

#ifndef CUSTOM_VERTEX_H
#define CUSTOM_VERTEX_H

#include <windows.h>
#include <d3dx9.h>

/**
* @brief 頂点データ
*/
struct CustomVertex
{
public:
	D3DXVECTOR3 m_pos;

	FLOAT m_rHW = 1.0f;			//! 重みの逆数 基本1

	DWORD m_aRGB = 0xFFFFFFFF;

	D3DXVECTOR2 m_texUV;		//! テクスチャの座標
};

#endif // !CUSTOM_VERTEX_H
