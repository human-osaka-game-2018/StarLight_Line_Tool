/**
* @file Camera.h
* @brief カメラクラスのヘッダ
* @author Harutaka Tsujino
*/

#ifndef CAMERA_H
#define CAMERA_H

#include <windows.h>

#include <d3dx9.h>

/**
* @brief カメラクラス
*/
class Camera
{
public:
	Camera(const LPDIRECT3DDEVICE9& rpDX3DDev) :m_rpDX3D_DEV(rpDX3DDev) {};
	~Camera() {};

	/**
	* @brief 現在のカメラの位置を取得する
	* @param[out] pCameraPos カメラ位置を入れる
	*/
	inline VOID GetCameraPos(D3DXVECTOR3* pCameraPos) const
	{
		pCameraPos->x = m_cameraPos.x;
		pCameraPos->y = m_cameraPos.y;
		pCameraPos->z = m_cameraPos.z;
	}

	/**
	* @brief 現在のカメラの位置を変更する
	* @param x x座標
	* @param y y座標
	* @param z z座標
	*/
	inline VOID SetCameraPos(FLOAT x, FLOAT y, FLOAT z)
	{
		m_cameraPos.x = x;
		m_cameraPos.y = y;
		m_cameraPos.z = z;
	}

	inline VOID SetCameraPos(const D3DXVECTOR3& rCameraPos)
	{
		m_cameraPos.x = rCameraPos.x;
		m_cameraPos.y = rCameraPos.y;
		m_cameraPos.z = rCameraPos.z;
	}

	/**
	* @brief 現在のカメラの注視点を取得する
	* @param[out] pEyePoint カメラの注視点を入れる
	*/
	inline VOID GetEyePt(D3DXVECTOR3* pEyePoint) const
	{
		pEyePoint->x = m_eyePt.x;
		pEyePoint->y = m_eyePt.y;
		pEyePoint->z = m_eyePt.z;
	}

	/**
	* @brief 現在のカメラの注視点位置を変更する
	* @param x x座標
	* @param y y座標
	* @param z z座標
	*/
	inline VOID SetEyePt(FLOAT x, FLOAT y, FLOAT z)
	{
		m_eyePt.x = x;
		m_eyePt.y = y;
		m_eyePt.z = z;
	}

	inline VOID SetEyePt(const D3DXVECTOR3& rEyePt)
	{
		m_eyePt.x = rEyePt.x;
		m_eyePt.y = rEyePt.y;
		m_eyePt.z = rEyePt.z;
	}

	/**
	* @brief カメラのビュー行列を取得する
	* @param[out] pView ビュー行列を入れる
	*/
	inline VOID GetView(D3DXMATRIX* pView) const
	{
		*pView = m_view;
	}

	/**
	* @brief カメラのプロジェクション行列を取得する
	* @param[out] pProj プロジェクション行列を入れる
	*/
	inline VOID GetProj(D3DXMATRIX* pProj) const
	{
		*pProj = m_proj;
	}

	/**
	* @brief カメラのSetTransformを行う
	*/
	VOID SetTransform();

	/**
	* @brief 引数の行列をビルボード化する
	* @param[in,out] pWorld ビルボード化する行列のポインタ
	*/
	inline VOID TransBillBoard(D3DXMATRIX* pWorld) const
	{
		D3DXMATRIX viewInverse;
		D3DXMatrixInverse(
			&viewInverse,
			NULL,
			&m_view);

		D3DXMatrixMultiply(
			pWorld,
			pWorld,
			&viewInverse);
	}

private:
	const LPDIRECT3DDEVICE9& m_rpDX3D_DEV = nullptr;

	D3DXVECTOR3 m_cameraPos			= { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 m_eyePt				= { 0.0f,0.0f,1.0f };
	D3DXVECTOR3 m_cameraOverhead	= { 0.0f,1.0f,0.0f };

	D3DXMATRIX m_view;
	D3DXMATRIX m_proj;
};

#endif //! CAMERA_H
