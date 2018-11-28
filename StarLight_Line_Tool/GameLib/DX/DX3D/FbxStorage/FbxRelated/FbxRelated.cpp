/**
* @file		FbxRelated.cpp
* @brief	FbxRelatedクラス実装
* @author	kawaguchi
*/

#include <fbxsdk.h>
#include <cstdio>
#include <cstdlib>
#include <crtdbg.h>
#include "FbxRelated.h"

#define _CRTDBG_MAP_ALLOC
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

#pragma comment(lib,"libfbxsdk-md.lib")

FbxRelated::FbxRelated(LPDIRECT3DDEVICE9& rpDX3DDev) :m_rpDX3DDev(rpDX3DDev)
{
	m_pFbxManager = NULL;
	m_pFbxScene = NULL;
	m_pModel.push_back(new FbxModel(m_rpDX3DDev));
	m_pModel[0]->m_pFbxModelData = new FbxModel::FbxModelData;
}

FbxRelated::~FbxRelated()
{
	Release();
}

VOID FbxRelated::Release()
{
	if (m_pFbxScene)
	{
		m_pFbxScene->Destroy();
		m_pFbxScene = NULL;
	}

	if (m_pFbxManager)
	{
		m_pFbxManager->Destroy();
		m_pFbxManager = NULL;
	}

	/*
	if (!m_pModel->m_pFbxModelData->pVertexColor)
	{
	delete m_pModel->m_pFbxModelData->pVertexColor;
	}
	*/

	for (INT j = 0; j < m_modelDataCount; ++j)
	{
		for (INT i = 0; i < m_pModel[j]->m_pFbxModelData->UvLayerCount; i++)
		{
			if (!m_pModel[m_modelDataCount - 1]->m_pFbxModelData->uvSet.uvBuffer.capacity())
			{
				continue;
			}

			delete m_pModel[j]->m_pFbxModelData->uvSet.uvBuffer[i];
		}

		if (m_pModel[j]->m_pFbxModelData->fileTextureCount)
		{
			if (!m_pModel[j]->m_pFbxModelData->pTmpTexture)
			{
				delete m_pModel[j]->m_pFbxModelData->pTmpTexture;
			}
		}

		delete m_pModel[j]->m_pFbxModelData->pIndexBuffer;
		delete m_pModel[j]->m_pFbxModelData->pVertex;
		delete m_pModel[j]->m_pFbxModelData->pPolygonSize;
		delete m_pModel[j]->m_pFbxModelData;
		delete m_pModel[j];
	}

	std::vector<FbxModel*>().swap(m_pModel);
}

VOID FbxRelated::TriangulateRecursive(FbxNode* pNode, FbxScene* pScene)
{
	FbxNodeAttribute* pNodeAttribute = pNode->GetNodeAttribute();

	if (pNodeAttribute)
	{
		FbxNodeAttribute::EType type = pNodeAttribute->GetAttributeType();
		if (type == FbxNodeAttribute::eMesh ||
			type == FbxNodeAttribute::eNurbs ||
			type == FbxNodeAttribute::eNurbsSurface ||
			type == FbxNodeAttribute::ePatch)
		{
			FbxGeometryConverter lConverter(pNode->GetFbxManager());
			if (lConverter.Triangulate(pScene, true) == false)
			{
				MessageBox(NULL, TEXT("三角形化に失敗しました。"), TEXT("エラー"), MB_OK);
			}
		}
	}

	const INT lChildCount = pNode->GetChildCount();
	for (INT lChildIndex = 0; lChildIndex < lChildCount; ++lChildIndex)
	{
		// 子ノードを探索。 
		TriangulateRecursive(pNode->GetChild(lChildIndex), pScene);
	}
}

bool FbxRelated::InitializeFbxSdkObjects()
{
	m_pFbxManager = fbxsdk::FbxManager::Create();

	if (!m_pFbxManager)
	{
		FBXSDK_printf("Error: Unable to create FBX Manager!\n");
		return false;
	}
	else
	{
		FBXSDK_printf("Autodesk FBX SDK version %s\n", m_pFbxManager->GetVersion());
	}

	fbxsdk::FbxIOSettings* pIOSetting = fbxsdk::FbxIOSettings::Create(m_pFbxManager, IOSROOT);
	m_pFbxManager->SetIOSettings(pIOSetting);

	m_pFbxScene = fbxsdk::FbxScene::Create(m_pFbxManager, "");
	if (!m_pFbxScene)
	{
		FBXSDK_printf("Error: Unable to create FBX scene!\n");
		return false;
	}
	return true;
}

bool FbxRelated::LoadFbx(const char* pName)
{
	INT sFileFormat = -1;

	// FBX SDK オブジェクトの初期化。 
	if (!InitializeFbxSdkObjects())
	{
		return false;
	}

	// インポータ作成。 
	fbxsdk::FbxImporter* m_pFbxImporter;

	m_pFbxImporter = FbxImporter::Create(m_pFbxManager, "");
	if (!m_pFbxImporter)
	{
		FBXSDK_printf("Error: Unable to create FBX importer!\n");
		return false;
	}

	// ファイルを指定したインポート(リーダー)ファイル形式で検出します。 
	if (!m_pFbxManager->GetIOPluginRegistry()->DetectReaderFileFormat(pName, sFileFormat))
	{
		// 検出できないファイル形式です。 FbxImporter::eFBX_BINARY 形式でトライします。 
		sFileFormat = m_pFbxManager->GetIOPluginRegistry()->FindReaderIDByDescription("FBX binary (*.fbx)");
	}

	// FBXファイルを読み込む。 
	if (!m_pFbxImporter->Initialize(pName, sFileFormat))
	{
		FBXSDK_printf("Error: Unable to create FBX initialize!\n");
		return false;
	}

	// 読み込んだFBXファイルからシーンデータを取り出す。 
	if (!m_pFbxImporter->Import(m_pFbxScene))
	{
		FBXSDK_printf("Error: Unable to create FBX import!\n");
		return false;
	}

	m_pFbxImporter->Destroy();
	m_pFbxImporter = NULL;

	// 三角形化(データによっては、四角形ポリゴンとか混ざっている場合がある) 
	TriangulateRecursive(m_pFbxScene->GetRootNode(), m_pFbxScene);

	//	ルートノードを取得
	fbxsdk::FbxNode* pRootNode = m_pFbxScene->GetRootNode();

	if (NULL != pRootNode)
	{
		//	ルートノードの子ノード数を取得
		INT childCount = pRootNode->GetChildCount();

		//	子ノードの数だけ探査をする
		for (INT i = 0; childCount > i; i++)
		{
			GetMesh(pRootNode->GetChild(i));
		}
	}

	return true;
}

VOID FbxRelated::GetMesh(fbxsdk::FbxNode* pNode)
{
	//	ノードも属性を取得
	fbxsdk::FbxNodeAttribute* pAttr = pNode->GetNodeAttribute();

	if (NULL != pAttr)
	{
		//	属性の判別
		switch (pAttr->GetAttributeType())
		{
			//	メッシュノード発見
		case fbxsdk::FbxNodeAttribute::eMesh:
		{
			fbxsdk::FbxMesh* pMesh = pNode->GetMesh();
			GetPosition(pMesh);

			//	頂点の法線座標を取得
			GetVertexNormal(pMesh);

			//	UV座標を取得
			GetVertexUV(pMesh);

			//	マテリアルとテクスチャ名を取得
			GetMaterialData(pMesh);

			//	頂点カラーを取得
			GetVertexColor(pMesh);
		}

		break;
		}
	}

	//	子ノードの再起探査
	INT childCount = pNode->GetChildCount();

	for (INT i = 0; childCount > i; i++)
	{
		GetMesh(pNode->GetChild(i));
	}
}

VOID FbxRelated::GetPosition(fbxsdk::FbxMesh* pMesh)
{
	if (!m_modelDataCount)
	{
		m_modelDataCount++;
	}

	else
	{
		m_pModel.push_back(new FbxModel(m_rpDX3DDev));
		m_pModel[m_modelDataCount]->m_pFbxModelData = new FbxModel::FbxModelData;
		m_modelDataCount++;
	}

	//	ポリゴン数を取得
	m_pModel[m_modelDataCount - 1]->m_pFbxModelData->polygonCount = pMesh->GetPolygonCount();

	//	ポリゴンサイズを取得
	m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pPolygonSize = new INT[m_pModel[m_modelDataCount - 1]->m_pFbxModelData->polygonCount];

	for (INT i = 0; m_pModel[m_modelDataCount - 1]->m_pFbxModelData->polygonCount > i; i++)
	{
		m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pPolygonSize[i] = pMesh->GetPolygonSize(i);
	}

	//	総頂点数を取得
	m_pModel[m_modelDataCount - 1]->m_pFbxModelData->vertexCount = pMesh->GetControlPointsCount();

	//	インデックス数を取得
	m_pModel[m_modelDataCount - 1]->m_pFbxModelData->indexCount = pMesh->GetPolygonVertexCount();

	//	頂点バッファの取得
	fbxsdk::FbxVector4* pVertex = pMesh->GetControlPoints();

	//	インデックスバッファの取得
	INT* pIndex = pMesh->GetPolygonVertices();

	//	頂点座標のキャストとハードコピー
	D3DXVECTOR3* pTmpVertex = new D3DXVECTOR3[m_pModel[m_modelDataCount - 1]->m_pFbxModelData->vertexCount];

	for (INT i = 0; m_pModel[m_modelDataCount - 1]->m_pFbxModelData->vertexCount > i; i++)
	{
		//	i番目の頂点の座標Xを取得
		pTmpVertex[i].x = (FLOAT)pVertex[i][0];

		if (m_pModel[m_modelDataCount - 1]->maxX < pTmpVertex[i].x)
		{
			m_pModel[m_modelDataCount - 1]->maxX = pTmpVertex[i].x;
		}
		if (m_pModel[m_modelDataCount - 1]->minX > pTmpVertex[i].x)
		{
			m_pModel[m_modelDataCount - 1]->minX = pTmpVertex[i].x;
		}

		//	i番目の頂点の座標Yを取得
		pTmpVertex[i].y = (FLOAT)pVertex[i][1];

		if (m_pModel[m_modelDataCount - 1]->maxY < pTmpVertex[i].y)
		{
			m_pModel[m_modelDataCount - 1]->maxY = pTmpVertex[i].y;
		}
		if (m_pModel[m_modelDataCount - 1]->minY > pTmpVertex[i].y)
		{
			m_pModel[m_modelDataCount - 1]->minY = pTmpVertex[i].y;
		}

		//	i番目の頂点の座標Zを取得
		pTmpVertex[i].z = (FLOAT)pVertex[i][2];

		if (m_pModel[m_modelDataCount - 1]->maxZ < pTmpVertex[i].z)
		{
			m_pModel[m_modelDataCount - 1]->maxZ = pTmpVertex[i].z;
		}
		if (m_pModel[m_modelDataCount - 1]->minZ > pTmpVertex[i].z)
		{
			m_pModel[m_modelDataCount - 1]->minZ = pTmpVertex[i].z;
		}

		if (m_pModel[m_modelDataCount - 1]->maxR < pTmpVertex[i].x)
		{
			m_pModel[m_modelDataCount - 1]->maxR = pTmpVertex[i].x;
		}
		if (m_pModel[m_modelDataCount - 1]->maxR < pTmpVertex[i].y)
		{
			m_pModel[m_modelDataCount - 1]->maxR = pTmpVertex[i].y;
		}
		if (m_pModel[m_modelDataCount - 1]->maxR < pTmpVertex[i].z)
		{
			m_pModel[m_modelDataCount - 1]->maxR = pTmpVertex[i].z;
		}
	}

	m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pVertex = new FbxModel::Vertex[m_pModel[m_modelDataCount - 1]->m_pFbxModelData->indexCount];

	for (INT i = 0; m_pModel[m_modelDataCount - 1]->m_pFbxModelData->indexCount > i; i++)
	{
		//	i番目の頂点の座標Xを取得
		m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pVertex[i].Vec.x = pTmpVertex[pIndex[i]].x;

		//	i番目の頂点の座標Yを取得
		m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pVertex[i].Vec.y = pTmpVertex[pIndex[i]].y;

		//	i番目の頂点の座標Zを取得
		m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pVertex[i].Vec.z = pTmpVertex[pIndex[i]].z;
	}

	//	インデックスバッファのハードコピー
	m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pIndexBuffer = new INT[m_pModel[m_modelDataCount - 1]->m_pFbxModelData->indexCount];

	for (INT i = 0; m_pModel[m_modelDataCount - 1]->m_pFbxModelData->indexCount > i; i++)
	{
		m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pIndexBuffer[i] = pIndex[i];
	}
	delete pTmpVertex;
}

VOID FbxRelated::GetVertexNormal(fbxsdk::FbxMesh* pMesh)
{
	//	法線セット数を取得
	INT normalLayerCount = pMesh->GetElementNormalCount();

	//	レイヤー数だけ回る
	for (INT i = 0; normalLayerCount > i; i++)
	{
		//	法線セットを取得
		fbxsdk::FbxGeometryElementNormal* pNormal = pMesh->GetElementNormal(i);

		//	マッピングモードの取得
		fbxsdk::FbxGeometryElement::EMappingMode mapping = pNormal->GetMappingMode();

		//	リファレンスモードの取得
		fbxsdk::FbxGeometryElement::EReferenceMode reference = pNormal->GetReferenceMode();

		//	マッピングモードの判別
		switch (mapping)
		{
		case fbxsdk::FbxGeometryElement::eByControlPoint:

			//	リファレンスモードの判別
			switch (reference)
			{
			case fbxsdk::FbxGeometryElement::eDirect:
			{
				//	法線数を取得
				INT normalCount = pNormal->GetDirectArray().GetCount();

				//	eDirect の場合データは順番に格納されているのでそのまま保持
				for (INT i = 0; normalCount > i; i++)
				{
					//	法線の取得
					m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pVertex[i].Normal.x = (FLOAT)pNormal->GetDirectArray().GetAt(i)[0];
					m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pVertex[i].Normal.y = (FLOAT)pNormal->GetDirectArray().GetAt(i)[1];
					m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pVertex[i].Normal.z = (FLOAT)pNormal->GetDirectArray().GetAt(i)[2];
				}
			}
			break;

			case fbxsdk::FbxGeometryElement::eIndexToDirect:
				break;

			default:
				break;
			}

			break;

		case fbxsdk::FbxGeometryElement::eByPolygonVertex:
			//	リファレンスモードの判別
			switch (reference)
			{
			case fbxsdk::FbxGeometryElement::eDirect:
			{
				//	法線数を取得
				INT normalCount = pNormal->GetDirectArray().GetCount();

				//	eDirect の場合データは順番に格納されているのでそのまま保持
				for (INT i = 0; normalCount > i; i++)
				{
					//	法線の取得
					m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pVertex[i].Normal.x = (FLOAT)pNormal->GetDirectArray().GetAt(i)[0];
					m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pVertex[i].Normal.y = (FLOAT)pNormal->GetDirectArray().GetAt(i)[1];
					m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pVertex[i].Normal.z = (FLOAT)pNormal->GetDirectArray().GetAt(i)[2];
				}
			}
			break;

			case fbxsdk::FbxGeometryElement::eIndexToDirect:
				break;

			default:
				break;
			}

			break;


			break;

		default:
			break;
		}
	}
}

//	UV取得関数
VOID FbxRelated::GetVertexUV(fbxsdk::FbxMesh* pMesh)
{
	//	UVセット数を取得
	m_pModel[m_modelDataCount - 1]->m_pFbxModelData->UvLayerCount = pMesh->GetElementUVCount();

	for (INT i = 0; m_pModel[m_modelDataCount - 1]->m_pFbxModelData->UvLayerCount > i; i++)
	{
		//	UVバッファを取得
		fbxsdk::FbxGeometryElementUV* pUV = pMesh->GetElementUV(i);

		//	マッピングモードの取得
		fbxsdk::FbxGeometryElement::EMappingMode mapping = pUV->GetMappingMode();

		//	リファレンスモードの取得
		fbxsdk::FbxGeometryElement::EReferenceMode reference = pUV->GetReferenceMode();

		//	UV数を取得
		INT uvCount = pUV->GetDirectArray().GetCount();

		//	マッピングモードの判別
		switch (mapping)
		{
		case fbxsdk::FbxGeometryElement::eByControlPoint:
			break;

		case fbxsdk::FbxGeometryElement::eByPolygonVertex:

			//	リファレンスモードの判別
			switch (reference)
			{
			case fbxsdk::FbxGeometryElement::eDirect:
				break;

			case fbxsdk::FbxGeometryElement::eIndexToDirect:
			{
				fbxsdk::FbxLayerElementArrayTemplate<INT>* pUvIndex = &pUV->GetIndexArray();

				m_pModel[m_modelDataCount - 1]->m_pFbxModelData->uvIndexCount = pUvIndex->GetCount();

				//	UVを保持
				std::vector<D3DXVECTOR2*> temp;

				INT uVCount = 0;

				temp.push_back(new D3DXVECTOR2[uVCount = m_pModel[m_modelDataCount - 1]->m_pFbxModelData->uvIndexCount]);

				for (INT j = 0; uVCount > j; j++)
				{
					fbxsdk::FbxVector2&  rFbxVector2 = pUV->GetDirectArray().GetAt(pUvIndex->GetAt(j));

					temp[0][j].x = (FLOAT)rFbxVector2[0];
					temp[0][j].y = 1.f - (FLOAT)rFbxVector2[1];

					FbxModel::FBXMODELDATA*& pModeldata = m_pModel[m_modelDataCount - 1]->m_pFbxModelData;

					pModeldata->uvSet.uvBuffer.push_back(temp[0]);
				}

				//	UVSet名を取得
				m_pModel[m_modelDataCount - 1]->m_pFbxModelData->uvSet.uvSetName = pUV->GetName();
			}
			break;

			default:
				break;
			}
			break;

		case fbxsdk::FbxGeometryElement::eByEdge:
			break;

		case fbxsdk::FbxGeometryElement::eByPolygon:
			break;

		default:
			break;
		}
	}

	if (!m_pModel[m_modelDataCount - 1]->m_pFbxModelData->uvSet.uvBuffer.capacity())
	{
		return;
	}

	for (INT i = 0; i < (m_pModel[m_modelDataCount - 1]->m_pFbxModelData->indexCount); ++i)
	{
		//	コードを短縮するためのバッファのバッファ
		D3DXVECTOR2* pUVBufferBuffer = m_pModel[m_modelDataCount - 1]->m_pFbxModelData->uvSet.uvBuffer[0];

		m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pVertex[i].tu = pUVBufferBuffer[i].x;///////////////////////////////---エラーVector---///////////////////////////////
		m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pVertex[i].tv = pUVBufferBuffer[i].y;
	}

}

VOID FbxRelated::GetMaterialData(fbxsdk::FbxMesh* pMesh)
{
	//	メッシュからノードを取得
	fbxsdk::FbxNode* pNode = pMesh->GetNode();

	//	マテリアルの数を取得
	m_pModel[m_modelDataCount - 1]->m_pFbxModelData->materialCount = pNode->GetMaterialCount();

	//	テクスチァの数をカウントする
	static INT fileTextureCount = 0;

	//	マテリアルの数だけ繰り返す
	for (INT i = 0; m_pModel[m_modelDataCount - 1]->m_pFbxModelData->materialCount > i; i++)
	{
		//	マテリアルを取得
		fbxsdk::FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i);

		D3DMATERIAL9 MaterialData;

		memset(&MaterialData, 0, sizeof(D3DMATERIAL9));

		BOOL isLmbert = 0;
		if (isLmbert = pMaterial->GetClassId().Is(fbxsdk::FbxSurfacePhong::ClassId))
		{
			// Phongにダウンキャスト
			fbxsdk::FbxSurfacePhong* phong = (fbxsdk::FbxSurfacePhong*)pMaterial;

			// アンビエント
			MaterialData.Ambient.r = (FLOAT)phong->Ambient.Get().mData[0] * (FLOAT)phong->AmbientFactor.Get();
			MaterialData.Ambient.g = (FLOAT)phong->Ambient.Get().mData[1] * (FLOAT)phong->AmbientFactor.Get();
			MaterialData.Ambient.b = (FLOAT)phong->Ambient.Get().mData[2] * (FLOAT)phong->AmbientFactor.Get();
			GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sAmbient);

			// ディフューズ
			MaterialData.Diffuse.r = (FLOAT)phong->Diffuse.Get().mData[0] * (FLOAT)phong->DiffuseFactor.Get();
			MaterialData.Diffuse.g = (FLOAT)phong->Diffuse.Get().mData[1] * (FLOAT)phong->DiffuseFactor.Get();
			MaterialData.Diffuse.b = (FLOAT)phong->Diffuse.Get().mData[2] * (FLOAT)phong->DiffuseFactor.Get();
			GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sDiffuse);

			// エミッシブ
			MaterialData.Emissive.r = (FLOAT)phong->Emissive.Get().mData[0] * (FLOAT)phong->EmissiveFactor.Get();
			MaterialData.Emissive.g = (FLOAT)phong->Emissive.Get().mData[1] * (FLOAT)phong->EmissiveFactor.Get();
			MaterialData.Emissive.b = (FLOAT)phong->Emissive.Get().mData[2] * (FLOAT)phong->EmissiveFactor.Get();
			GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sEmissive);

			// 透過度
			FLOAT alpha = (FLOAT)phong->TransparencyFactor.Get();
			MaterialData.Ambient.a = 1.f;
			MaterialData.Diffuse.a = 1.0f - alpha;
			MaterialData.Emissive.a = 1.f;
			MaterialData.Specular.a = 1.f;
			GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sTransparentColor);

			// スペキュラ
			MaterialData.Specular.r = (FLOAT)phong->Specular.Get().mData[0] * (FLOAT)phong->SpecularFactor.Get();
			MaterialData.Specular.g = (FLOAT)phong->Specular.Get().mData[1] * (FLOAT)phong->SpecularFactor.Get();
			MaterialData.Specular.b = (FLOAT)phong->Specular.Get().mData[2] * (FLOAT)phong->SpecularFactor.Get();
			GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sSpecular);

			// 光沢
			//			shininess_ = (FLOAT)phong->GetShininess().Get();		??????????????

			// 反射
			//			reflectivity_ = (FLOAT)phong->GetReflectionFactor().Get();		??????????????

			float buf =(FLOAT)phong->Reflection.Get().mData[0] * (FLOAT)phong->ReflectionFactor.Get();

			MaterialData.Power = (FLOAT)phong->Shininess.Get();

			GetTextureName(phong, fbxsdk::FbxSurfaceMaterial::sNormalMap);

			m_pModel[m_modelDataCount - 1]->m_pFbxModelData->MaterialData.push_back(MaterialData);
		}
		else if (isLmbert = pMaterial->GetClassId().Is(fbxsdk::FbxSurfaceLambert::ClassId))
		{
			// Lambertにダウンキャスト
			fbxsdk::FbxSurfaceLambert* lambert = (fbxsdk::FbxSurfaceLambert*)pMaterial;

			// アンビエント
			MaterialData.Ambient.r = (FLOAT)lambert->Ambient.Get().mData[0] * (FLOAT)lambert->AmbientFactor.Get();
			MaterialData.Ambient.g = (FLOAT)lambert->Ambient.Get().mData[1] * (FLOAT)lambert->AmbientFactor.Get();
			MaterialData.Ambient.b = (FLOAT)lambert->Ambient.Get().mData[2] * (FLOAT)lambert->AmbientFactor.Get();
			GetTextureName(lambert, fbxsdk::FbxSurfaceMaterial::sAmbient);

			// ディフューズ
			MaterialData.Diffuse.r = (FLOAT)lambert->Diffuse.Get().mData[0] * (FLOAT)lambert->DiffuseFactor.Get();
			MaterialData.Diffuse.g = (FLOAT)lambert->Diffuse.Get().mData[1] * (FLOAT)lambert->DiffuseFactor.Get();
			MaterialData.Diffuse.b = (FLOAT)lambert->Diffuse.Get().mData[2] * (FLOAT)lambert->DiffuseFactor.Get();
			GetTextureName(lambert, fbxsdk::FbxSurfaceMaterial::sDiffuse);

			// エミッシブ
			MaterialData.Emissive.r = (FLOAT)lambert->Emissive.Get().mData[0] * (FLOAT)lambert->EmissiveFactor.Get();
			MaterialData.Emissive.g = (FLOAT)lambert->Emissive.Get().mData[1] * (FLOAT)lambert->EmissiveFactor.Get();
			MaterialData.Emissive.b = (FLOAT)lambert->Emissive.Get().mData[2] * (FLOAT)lambert->EmissiveFactor.Get();
			GetTextureName(lambert, fbxsdk::FbxSurfaceMaterial::sEmissive);

			// 透過度
			FLOAT alpha = (FLOAT)lambert->TransparencyFactor.Get();
			MaterialData.Ambient.a = 1.f;
			MaterialData.Diffuse.a = 1.0f - alpha;
			MaterialData.Emissive.a = 1.f;
			GetTextureName(lambert, fbxsdk::FbxSurfaceMaterial::sTransparentColor);

			GetTextureName(lambert, fbxsdk::FbxSurfaceMaterial::sNormalMap);

			m_pModel[m_modelDataCount - 1]->m_pFbxModelData->MaterialData.push_back(MaterialData);
		}
	}
}

VOID FbxRelated::GetTextureName(fbxsdk::FbxSurfaceMaterial* pMaterial, const char* pMatAttr)
{
	//	プロパティを取得
	fbxsdk::FbxProperty prop = pMaterial->FindProperty(pMatAttr);

	//	FbxLayerTexture の数を取得
	INT layeredTextureCount = prop.GetSrcObjectCount<fbxsdk::FbxLayeredTexture>();

	//	アタッチされたテクスチャがFbxLayeredTexture の場合
	if (0 < layeredTextureCount)
	{
		//	アタッチされたテクスチャの数だけ繰り返す
		for (INT i = 0; layeredTextureCount > i; i++)
		{
			//	テクスチャを取得
			fbxsdk::FbxLayeredTexture* pLayeredTexture = prop.GetSrcObject<fbxsdk::FbxLayeredTexture>(i);

			//	レイヤー数を取得
			INT textureCount = pLayeredTexture->GetSrcObjectCount<fbxsdk::FbxFileTexture>();

			//	レイヤー数だけ繰り返す
			for (INT j = 0; textureCount > j; j++)
			{
				//	テクスチャを取得
				fbxsdk::FbxFileTexture* pFbxFileTexture = prop.GetSrcObject<fbxsdk::FbxFileTexture>(j);

				if (pFbxFileTexture)
				{
					//	テクスチャ名を取得
					//std::string textureName = pFbxFileTexture->GetName();
					const char* textureName = pFbxFileTexture->GetRelativeFileName();

					//	UVSet名を取得
					std::string UVSetName = pFbxFileTexture->UVSet.Get().Buffer();

					//	UVSet名を比較し対応しているテクスチャなら保持してデータクラスにUV座標をいれる
					if (m_pModel[m_modelDataCount - 1]->m_pFbxModelData->uvSet.uvSetName == UVSetName)
					{
						m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pTmpTexture = new FbxModel::TextureData();
						m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pTmpTexture->m_TextureName = textureName;
						m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pTextureData.push_back(m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pTmpTexture);

						if (FAILED(D3DXCreateTextureFromFileA(
							m_pModel[m_modelDataCount - 1]->m_pDevice,
							m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pTextureData[m_pModel[m_modelDataCount - 1]->m_pFbxModelData->fileTextureCount]->m_TextureName,
							&m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pTextureData[m_pModel[m_modelDataCount - 1]->m_pFbxModelData->fileTextureCount]->m_pTexture)))
						{
							m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pTextureData[m_pModel[m_modelDataCount - 1]->m_pFbxModelData->fileTextureCount]->m_pTexture = NULL;
						}
						m_pModel[m_modelDataCount - 1]->m_pFbxModelData->fileTextureCount++;
					}
				}
			}
		}
	}

	else
	{
		//	テクスチャ数を取得
		INT fileTextureCount = prop.GetSrcObjectCount<fbxsdk::FbxFileTexture>();

		if (0 < fileTextureCount)
		{
			//	テクスチャの数だけ繰り返す
			for (INT i = 0; fileTextureCount > i; i++)
			{
				//	テクスチャを取得
				fbxsdk::FbxFileTexture* pFbxFileTexture = prop.GetSrcObject<fbxsdk::FbxFileTexture>(i);

				if (pFbxFileTexture)
				{
					//	テクスチャ名を取得
					//	std::string textureName = texture->GetName();
					const char* textureName = pFbxFileTexture->GetRelativeFileName();

					//	UVSet名を取得
					std::string UVSetName = pFbxFileTexture->UVSet.Get().Buffer();

					//	UVSet名を比較し対応しているテクスチャなら保持
					if (m_pModel[m_modelDataCount - 1]->m_pFbxModelData->uvSet.uvSetName == UVSetName)
					{
						m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pTmpTexture = new FbxModel::TextureData();
						m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pTmpTexture->m_TextureName = textureName;
						m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pTextureData.push_back(m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pTmpTexture);

						if (FAILED(D3DXCreateTextureFromFileA(
							m_pModel[m_modelDataCount - 1]->m_pDevice,
							m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pTextureData[m_pModel[m_modelDataCount - 1]->m_pFbxModelData->fileTextureCount]->m_TextureName,
							&m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pTextureData[m_pModel[m_modelDataCount - 1]->m_pFbxModelData->fileTextureCount]->m_pTexture)))
						{
							m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pTextureData[m_pModel[m_modelDataCount - 1]->m_pFbxModelData->fileTextureCount]->m_pTexture = NULL;
						}
						m_pModel[m_modelDataCount - 1]->m_pFbxModelData->fileTextureCount++;

					}
				}
			}
		}
	}
}

VOID FbxRelated::GetVertexColor(fbxsdk::FbxMesh* pMesh)
{
	//	頂点カラーセット数を取得
	INT vColorLayerCount = pMesh->GetElementVertexColorCount();

	//	レイヤー数だけ回る
	for (INT i = 0; vColorLayerCount > i; i++)
	{
		//	法線セットを取得
		fbxsdk::FbxGeometryElementVertexColor* pColor = pMesh->GetElementVertexColor(i);

		//	マッピングモードの取得
		fbxsdk::FbxGeometryElement::EMappingMode mappingMode = pColor->GetMappingMode();

		//	リファレンスモードの取得
		fbxsdk::FbxGeometryElement::EReferenceMode referenceMode = pColor->GetReferenceMode();

		//	マッピングモードの判別
		switch (mappingMode)
		{
		case fbxsdk::FbxGeometryElement::eByControlPoint:
			break;

		case fbxsdk::FbxGeometryElement::eByPolygon:
			break;

		case fbxsdk::FbxGeometryElement::eByPolygonVertex:

			//	リファレンスモードの判別
			switch (referenceMode)
			{
			case fbxsdk::FbxGeometryElement::eIndexToDirect:
			{
				fbxsdk::FbxLayerElementArrayTemplate<INT>* pIndex = &pColor->GetIndexArray();
				INT indexCount = pIndex->GetCount();

				m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pVertexColor = new FbxModel::ColorRGBA[indexCount];

				//	頂点の数だけ頂点カラーを取得
				for (INT j = 0; indexCount > j; j++)
				{
					m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pVertexColor[j].r = (FLOAT)pColor->GetDirectArray().GetAt(pIndex->GetAt(j))[0];
					m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pVertexColor[j].g = (FLOAT)pColor->GetDirectArray().GetAt(pIndex->GetAt(j))[1];
					m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pVertexColor[j].b = (FLOAT)pColor->GetDirectArray().GetAt(pIndex->GetAt(j))[2];
					m_pModel[m_modelDataCount - 1]->m_pFbxModelData->pVertexColor[j].a = (FLOAT)pColor->GetDirectArray().GetAt(pIndex->GetAt(j))[3];
				}
			}
			break;

			case fbxsdk::FbxGeometryElement::eDirect:
				break;

			case fbxsdk::FbxGeometryElement::eIndex:
				break;

			default:
				break;
			}
			break;

		default:
			break;
		}
	}
}

VOID FbxRelated::SetEmissive(const D3DXVECTOR4* pARGB)
{
	for (FbxModel* pI : m_pModel)
	{
		pI->SetEmissive(pARGB);
	}
}

VOID FbxRelated::SetAmbient(const D3DXVECTOR4* pARGB)
{
	for (FbxModel* pI : m_pModel)
	{
		pI->SetAmbient(pARGB);
	}
}

VOID FbxRelated::SetDiffuse(const D3DXVECTOR4* pARGB) 
{
	for (FbxModel* pI : m_pModel)
	{
		pI->SetDiffuse(pARGB);
	}
}

VOID FbxRelated::SetSpecular(const D3DXVECTOR4* pARGB)
{
	for (FbxModel* pI : m_pModel)
	{
		pI->SetSpecular(pARGB);
	}
}

VOID FbxRelated::SetPower(float power)
{
	for (FbxModel* pI : m_pModel)
	{
		pI->SetPower(power);
	}
}
