/**
* @file		FbxModel.h
* @brief	FbxModelクラスヘッダ
* @author	kawaguchi
*/

#ifndef FBXMODEL_H
#define FBXMODEL_H

#include <fbxsdk.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <iostream>
#include <list>
#include <vector>

#define MY_FVF (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2)

/**
* Fbxモデルのデータを詰めるクラス
*/
class FbxModel
{
	friend class FbxRelated;

private:
	typedef struct VERTEX
	{
		D3DXVECTOR3  Vec;					//!< 頂点データ
		D3DXVECTOR3  Normal;				//!< 法線ベクトル
		FLOAT tu;
		FLOAT tv;
	}Vertex;

	typedef struct COLOR_RGBAF
	{
		FLOAT r;
		FLOAT g;
		FLOAT b;
		FLOAT a;
	}ColorRGBA;

	typedef struct UV_SET
	{
		std::string uvSetName;
		std::vector<D3DXVECTOR2*> uvBuffer;
	}UvSet;

	typedef struct TEXTURE_DATA
	{
		LPCSTR m_TextureName;
		LPDIRECT3DTEXTURE9 m_pTexture;
	}TextureData;

	typedef struct FBXMODELDATA
	{
		INT polygonCount = 0;						//!<	ポリゴン数
		INT vertexCount = 0;						//!<	総頂点数（コントロールポイント）
		INT indexCount = 0;							//!<	インデックス数（）
		INT fileTextureCount = 0;					//!<	テクスチャ数
		INT materialCount = 0;						//!<	マテリアル数
		INT UvLayerCount = 0;						//!<	UVレイヤーテクスチャ数
		INT uvIndexCount = 0;						//!<	UVのインデックス数

		INT* pIndexBuffer = NULL;					//!<	インデックスバッファ
		INT* pPolygonSize = NULL;					//!<	ポリゴン数

		Vertex* pVertex = NULL;						//!<	頂点座標
		ColorRGBA* pVertexColor = NULL;				//!<	頂点カラー
		UvSet uvSet;								//!<	UV情報
		TextureData* pTmpTexture;					//!<	ネームの仮置き場
		std::vector<TextureData*> pTextureData;		//!<	テクスチャデータ
		std::vector<D3DMATERIAL9> MaterialData;		//!<	マテリアルデータ

	}FbxModelData;

	FbxModelData* m_pFbxModelData;					//!<	全モデルデータ
	IDirect3DDevice9*			m_pDevice;			//!<	Direct3Dのデバイス

public:
	FLOAT maxX, maxY, maxZ, minX, minY, minZ, maxR;

	FbxModel(LPDIRECT3DDEVICE9& m_rpDX3DDev);
	~FbxModel();
	VOID DrawFbx();

	VOID SetAmbient(const D3DXVECTOR4* pARGB);											//!<	モデルを発光させる関数
	VOID SetDiffuse(const D3DXVECTOR4* pARGB);
	VOID SetEmissive(const D3DXVECTOR4* pARGB);
	VOID SetSpecular(const D3DXVECTOR4* pARGB);
	VOID SetPower(float power);

};

#endif	//	FBXMODEL_H
