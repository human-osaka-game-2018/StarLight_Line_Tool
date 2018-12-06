/**
* @file		FbxModel.cpp
* @brief	FbxModelクラス実装
* @author	kawaguchi
*/

#include <fbxsdk.h>
#include <cstdio>
#include <cstdlib>
#include <crtdbg.h>
#include "FbxModel.h"

#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

#pragma comment(lib,"libfbxsdk-md.lib")

FbxModel::FbxModel(LPDIRECT3DDEVICE9& m_rpDX3DDev)
{
	m_pDevice = m_rpDX3DDev;

	m_pFbxModelData = NULL;
}

FbxModel::~FbxModel()
{
}

VOID FbxModel::DrawFbx()
{
	m_pDevice->SetFVF(MY_FVF);

	for (D3DMATERIAL9& i : m_pFbxModelData->MaterialData)
	{
		if (!m_pFbxModelData->MaterialData.size())continue;

		m_pDevice->SetMaterial(&i);
	}

	for (unsigned int n = 0; n < m_pFbxModelData->pTextureData.size(); n++)
	{
		m_pDevice->SetTexture(n, m_pFbxModelData->pTextureData[n]->m_pTexture);
	}

	m_pDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,
		m_pFbxModelData->polygonCount,
		m_pFbxModelData->pVertex,
		sizeof(Vertex));
}

VOID FbxModel::SetEmissive(const D3DXVECTOR4* pARGB)
{
	for (D3DMATERIAL9& i : m_pFbxModelData->MaterialData)
	{
		i.Emissive.a = pARGB->x;
		i.Emissive.r = pARGB->y;
		i.Emissive.g = pARGB->z;
		i.Emissive.b = pARGB->w;
	}
}

VOID FbxModel::SetAmbient(const D3DXVECTOR4* pARGB)
{
	for (D3DMATERIAL9& i : m_pFbxModelData->MaterialData)
	{
		i.Ambient.a = pARGB->x;
		i.Ambient.r = pARGB->y;
		i.Ambient.g = pARGB->z;
		i.Ambient.b = pARGB->w;
	}
}

VOID FbxModel::SetDiffuse(const D3DXVECTOR4* pARGB)
{
	for (D3DMATERIAL9& i : m_pFbxModelData->MaterialData)
	{
		i.Diffuse.a = pARGB->x;
		i.Diffuse.r = pARGB->y;
		i.Diffuse.g = pARGB->z;
		i.Diffuse.b = pARGB->w;
	}
}

VOID FbxModel::SetSpecular(const D3DXVECTOR4* pARGB)
{
	for (D3DMATERIAL9& i : m_pFbxModelData->MaterialData)
	{
		i.Specular.a = pARGB->x;
		i.Specular.r = pARGB->y;
		i.Specular.g = pARGB->z;
		i.Specular.b = pARGB->w;
	}
}

VOID FbxModel::SetPower(float power)
{
	for (D3DMATERIAL9& i: m_pFbxModelData->MaterialData)
	{
		i.Power = power;
	}
}
