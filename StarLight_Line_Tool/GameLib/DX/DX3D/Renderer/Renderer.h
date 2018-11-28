/**
* @file Renderer.h
* @brief 描画クラスのヘッダ
* @author Harutaka Tsujino
*/

#ifndef RENDERER_H
#define RENDERER_H

#include <windows.h>

#include <d3dx9.h>

#include "../CustomVertexEditor/Data/CustomVertex.h"
#include "../FbxStorage/FbxRelated/FbxRelated.h"
#include "../FbxStorage/FbxRelated/FbxModel/FbxModel.h"

/**
* @brief FBX(予定)とCustomVertexの描画クラス
*/
class Renderer
{
public:
	Renderer(const LPDIRECT3DDEVICE9& rpDX3DDev) :m_rpDX3D_DEV(rpDX3DDev) {};
	~Renderer() {};

	/**
	* @brief FBXの描画を行う
	* @param rFBXModel FBXのクラス モデルを読み込んだ後でないといけない
	* @param rMatWorld 拡大回転移動行列をまとめた行列
	* @param pTexture モデルに張り付けるテクスチャのポインタ デフォルトで存在している場合はnullptr
	*/
	inline VOID Render(const FbxRelated& rFBXModel,
		const D3DXMATRIX& rWorld, const LPDIRECT3DTEXTURE9 pTexture = nullptr) const
	{
		m_rpDX3D_DEV->SetTransform(D3DTS_WORLD, &rWorld);

		m_rpDX3D_DEV->SetTexture(0, pTexture);

		for (FbxModel* pI : rFBXModel.m_pModel)
		{
			pI->DrawFbx();
		}
	}

	/**
	* @brief CustomVertexの描画を行う
	* @param pCustomVertices 描画する矩形の頂点データの先頭ポインタ
	* @param pTexture ポリゴンに張り付けるテクスチャのポインタ
	*/
	inline VOID Render(const CustomVertex* pCustomVertices, const LPDIRECT3DTEXTURE9 pTexture = nullptr) const
	{
		m_rpDX3D_DEV->SetFVF(
						D3DFVF_XYZRHW |
						D3DFVF_DIFFUSE |
						D3DFVF_TEX1);

		m_rpDX3D_DEV->SetTexture(0, pTexture);

		m_rpDX3D_DEV->DrawPrimitiveUP(
						D3DPT_TRIANGLEFAN,
						2,
						pCustomVertices, sizeof(CustomVertex));
	}

public:
	const LPDIRECT3DDEVICE9& m_rpDX3D_DEV = nullptr;
};

#endif //! RENDERER_H
